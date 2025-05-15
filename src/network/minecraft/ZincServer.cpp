#include <network/minecraft/ZincServer.h>
#include <network/minecraft/channels/DefaultChannels.h>
#include <external/JSON.h>
#include <util/Logger.h>
#include <util/crypto/Random.h>
#include <util/crypto/MCSHA1.h>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <ZincConstants.h>
#include <ZincConfig.h>

namespace zinc {

Logger m_zincLogger = Logger("ZincServer");

ZincServer g_zincServer;
std::map<std::string, ZincPacket(*)(ByteBuffer&, ZincConnection*)> g_zincServerPluginChannels = {
    { Identifier("minecraft", "brand").toString(), BrandChannel }
};
std::map<std::string, void(*)(std::optional<std::vector<char>>&, ZincConnection*)> g_zincCookieResponseParsers;
std::map<ZincConnection::State, std::vector<std::string>> g_zincCookieRequests = {
    { ZincConnection::State::Login, {} },
    { ZincConnection::State::Config, {} },
    { ZincConnection::State::Play, {} }
};

void ZincServer::start() {
    m_rsa.generateKeys(1024);
    m_zincLogger.info("Zinc listening on port " + std::to_string(m_port));
    m_started = true;
    cURLpp::initialize();
    m_server.start();
}
void ZincServer::stop() {
    if (!m_started) return;
    m_zincLogger.info("Zinc stopped");
    cURLpp::terminate();
    m_server.stop();
}
void ZincServer::setPort(const int& port) {
    m_port = port;
    m_server.setPort(port);
}
RSAWrapper& ZincServer::getRSA() {
    return m_rsa;
}
RSAWrapper ZincServer::getRSA() const {
    return m_rsa;
}
void ZincServer::addClient(ZincConnection* client) {
    if (!isConnected(client->getTCPConnection().getFd())) m_clients.insert({ client->getTCPConnection().getFd(), client });
}
bool ZincServer::isConnected(evutil_socket_t fd) const {
    return m_clients.contains(fd);
}
void ZincServer::removeClient(evutil_socket_t fd) {
    if (isConnected(fd)) {
        m_clients[fd]->getTCPConnection().close();
        delete m_clients[fd];
        m_clients.erase(fd);
    }
}
ZincConnection* ZincServer::getClient(evutil_socket_t fd) {
    if (!isConnected(fd)) m_zincLogger.error("Attempted to get unknown client", true); 
    return m_clients[fd];
}
void ZincServer::onAccept(evconnlistener* listener, evutil_socket_t fd, struct sockaddr* addr, int socklen, void* ptr) {
    bufferevent* bev = bufferevent_socket_new((event_base*) ptr, fd, BEV_OPT_CLOSE_ON_FREE);
    if (!bev) {
        m_zincLogger.error("Failed to create bufferevent");
        return;
    }
    ZincConnection* connection = new ZincConnection();
    connection->getTCPConnection().setAddr(addr);
    connection->getTCPConnection().setFd(fd);
    connection->getTCPConnection().setBuffer(bev);
    g_zincServer.addClient(connection);
    m_zincLogger.debug("Client [" + connection->getTCPConnection().getIP() + "] connected!"); 
    bufferevent_setcb(bev, onRead, nullptr, onEvent, ptr);
    bufferevent_enable(bev, EV_READ);
}
std::vector<unsigned char> ZincServer::openLoginPluginChannel(const std::string& channel) {
    std::vector<unsigned char> id = RandomUtil::randomBytes(4);
    while (isLoginPluginChannelOpened(id)) {
        id = RandomUtil::randomBytes(4);
    }
    m_openedLoginPluginChannels.insert({ id, channel });
    return id;
}
std::string ZincServer::getLoginPluginChannel(const std::vector<unsigned char>& id) {
    if (!isLoginPluginChannelOpened(id)) return "minecraft:brand";
    return m_openedLoginPluginChannels[id];
}
bool ZincServer::isLoginPluginChannelOpened(const std::vector<unsigned char>& id) {
    return m_openedLoginPluginChannels.contains(id);
}
void ZincServer::closeLoginPluginChannel(const std::vector<unsigned char>& id) {
    if (isLoginPluginChannelOpened(id)) m_openedLoginPluginChannels.erase(id);
}
void ZincServer::onRead(bufferevent* bev, void* ptr) {
    if (!g_zincServer.isConnected(bufferevent_getfd(bev))) return;
    ZincConnection* connection = g_zincServer.getClient(bufferevent_getfd(bev));
    ZincPacket packet = connection->read();
    ZincPacket replyPacket;
    if (packet.getId() < 0) {
        if (packet.getId() != -1) {
            // send error
        }
        return;
    }
    m_zincLogger.info("Got packet with id " + std::to_string(packet.getId()) + " and data size " + std::to_string(packet.getData().size()));
    switch (connection->getState()) {
    case ZincConnection::State::Handshake: {
        if (!packet.getId()) {
            connection->m_info.m_networkInfo.m_protocolVersion = packet.getData().readVarInt();
            connection->m_info.m_networkInfo.m_serverAddr = packet.getData().readString();
            connection->m_info.m_networkInfo.m_serverPort = packet.getData().readNumeric<unsigned short>();
            connection->setState((ZincConnection::State) packet.getData().readVarInt());
            if (connection->getState() != ZincConnection::State::Status &&
                connection->getState() != ZincConnection::State::Login &&
                connection->getState() != ZincConnection::State::Transfer) connection->setState(ZincConnection::State::Status);
        }
        break;
    }
    case ZincConnection::State::Status: {
        if (!packet.getId()) {
            replyPacket.setId(0);
            replyPacket.getData().writeString(nlohmann::json{
                { "version", {
                    { "name", LATEST_MINECRAFT_VERSION },
                    { "protocol", LATEST_MINECRAFT_VERSION_PROTOCOL }
                } },
                { "players", {
                    { "max", g_zincConfig.m_maxPlayerCount },
                    { "online", g_zincServer.m_onlinePlayers },
                    { "sample", {} }
                } },
                { "description", {
                    { "text", g_zincConfig.m_motd }
                } },
                { "favicon", "data:image/png;base64," + g_zincConfig.m_iconBase64 },
                { "enforcesSecureChat", true }
            }.dump());
            connection->send(replyPacket);
        } else connection->send(packet);
        break;
    }
    case ZincConnection::State::Login: {
        switch(packet.getId()) {
        /* LOGIN START */ case 0: {
            if (connection->m_info.m_networkInfo.m_protocolVersion < LATEST_MINECRAFT_VERSION_PROTOCOL) {
                replyPacket.setId(0);
                replyPacket.getData().writeString(
                    "[\"\",{\"text\":\"Zinc Login Error\",\"bold\":true,\"color\":\"dark_red\"},{\"text\":\"\n\"},"
                    "{\"text\":\"You are using outdated version\",\"color\":\"red\"}]");
            } else if (connection->m_info.m_networkInfo.m_protocolVersion > LATEST_MINECRAFT_VERSION_PROTOCOL) {
                replyPacket.setId(0);
                replyPacket.getData().writeString(
                    "[\"\",{\"text\":\"Zinc Login Error\",\"bold\":true,\"color\":\"dark_red\"},{\"text\":\"\n\"},"
                    "{\"text\":\"Server is using outdated version\",\"color\":\"red\"}]");
            } else {
                connection->m_info.m_playerInfo.m_playerName = packet.getData().readString();
                connection->m_info.m_playerInfo.m_playerUUID = packet.getData().readUUID();
                if (connection->m_info.m_playerInfo.m_playerName.size() > 16) {
                    replyPacket.setId(0);
                    replyPacket.getData().writeString(
                        "[\"\",{\"text\":\"Zinc Login Error\",\"bold\":true,\"color\":\"dark_red\"},{\"text\":\"\n\"},"
                        "{\"text\":\"Player name can't be int64_ter than 16 characters\",\"color\":\"red\"}]");
                } else {
                    replyPacket.setId(3);
                    replyPacket.getData().writeVarInt(g_zincConfig.m_threshold);
                    connection->send(replyPacket);
                    connection->setIsCompressed(true);
                    replyPacket.setData({});
                    replyPacket.setId(1);
                    replyPacket.getData().writeString("");
                    replyPacket.getData().writePrefixedArray<unsigned char>(g_zincServer.getRSA().getPublicKeyDER(), &ByteBuffer::writeUnsignedByte);
                    connection->m_info.m_networkInfo.m_verifyToken = RandomUtil::randomBytes(64);
                    replyPacket.getData().writePrefixedArray<unsigned char>(connection->m_info.m_networkInfo.m_verifyToken, &ByteBuffer::writeUnsignedByte);
                    replyPacket.getData().writeByte(g_zincConfig.m_onlineMode);
                }
            }
            connection->send(replyPacket);
            break;
        }
        /* ENCRYPTION RESPONSE */ case 1: {
            std::vector<unsigned char> sharedSecret = packet.getData().readPrefixedArray<unsigned char>(&ByteBuffer::readUnsignedByte);
            std::vector<unsigned char> verifyToken = packet.getData().readPrefixedArray<unsigned char>(&ByteBuffer::readUnsignedByte);
            sharedSecret = g_zincServer.getRSA().decrypt(sharedSecret);
            if (g_zincServer.getRSA().decrypt(verifyToken) == connection->m_info.m_networkInfo.m_verifyToken) {
                connection->m_info.m_networkInfo.m_verifyToken.clear();
                connection->setupEncryption(sharedSecret);
                if (g_zincConfig.m_onlineMode) {
                    MCSHA1 sha;
                    sha.update(std::vector<char>({}));
                    sha.update(sharedSecret);
                    sha.update(g_zincServer.getRSA().getPublicKeyDER());
                    sha.final();
                    std::string url = "https://sessionserver.mojang.com/session/minecraft/hasJoined?username=" + 
                                    connection->m_info.m_playerInfo.m_playerName + "&serverId=" + sha.digestJava();
                    try {
                        std::stringstream ss;
                        ss << curlpp::options::Url(url);
                        nlohmann::json JSON = nlohmann::json::parse(ss.str());
                        ss.clear();
                        connection->m_info.m_playerInfo.m_playerUUID = uuids::uuid::from_string(std::string(JSON["id"])).value();
                        connection->m_info.m_playerInfo.m_playerName = JSON["name"];
                        for (auto const& propertyJSON : JSON["properties"]) {
                            ZincConnectionProperty property;
                                property.m_name = propertyJSON["name"];
                                property.m_value = propertyJSON["value"];
                            if (propertyJSON.contains("signature"))
                                property.m_signature = propertyJSON["signature"];
                            connection->m_info.m_playerInfo.m_properties.push_back(property);
                        }
                        if (g_zincCookieRequests.contains(ZincConnection::State::Login)) 
                            for (const auto& cookieRequest : g_zincCookieRequests[ZincConnection::State::Login]) {
                                replyPacket.setId(5);
                                replyPacket.getData().writeIdentifier(cookieRequest);
                                connection->send(replyPacket);
                                replyPacket.setData({});
                            }
                        // TODO: request login plugin channels
                    } catch (std::exception e) {
                        replyPacket.setId(0);
                        replyPacket.getData().writeString(
                            "[\"\",{\"text\":\"Zinc Login Error\",\"bold\":true,\"color\":\"dark_red\"},{\"text\":\"\n\"},"
                            "{\"text\":\"Server is unable to access mojang session servers\",\"color\":\"red\"}]");
                        connection->send(replyPacket);
                        return;
                    }
                }
                replyPacket.setId(2);
                replyPacket.getData().writeUUID(connection->m_info.m_playerInfo.m_playerUUID);
                replyPacket.getData().writeString(connection->m_info.m_playerInfo.m_playerName);
                replyPacket.getData().writePrefixedArray<ZincConnectionProperty>(connection->m_info.m_playerInfo.m_properties, &writeZincConnectionProperty);
                replyPacket.getData().writeByte(0);
            } else {
                replyPacket.setId(0);
                replyPacket.getData().writeString(
                    "[\"\",{\"text\":\"Zinc Login Error\",\"bold\":true,\"color\":\"dark_red\"},{\"text\":\"\n\"},"
                    "{\"text\":\"Server received invalid encryption response\",\"color\":\"red\"}]");
            }
            connection->send(replyPacket);
            break;
        }
        /* LOGIN PLUGIN RESPONSE */ case 2: {
            std::vector<unsigned char> id = packet.getData().readArray<unsigned char>(&ByteBuffer::readUnsignedByte, 4);
            if (g_zincServer.isLoginPluginChannelOpened(id)) {
                // TODO: call login plugin channel parser
                g_zincServer.closeLoginPluginChannel(id);
            } else {
                replyPacket.setId(0);
                replyPacket.getData().writeString(
                    "[\"\",{\"text\":\"Zinc Login Error\",\"bold\":true,\"color\":\"dark_red\"},{\"text\":\"\n\"},"
                    "{\"text\":\"Server received invalid login plugin channel id\",\"color\":\"red\"}]");
                connection->send(replyPacket);
            }
            break;
        }
        /* LOGIN ACK */ case 3: {
            connection->setState(ZincConnection::State::Config);
            break;
        }
        /* COOKIE RESPONSE */ case 4: {
            Identifier cookieId = packet.getData().readIdentifier();
            std::optional<std::vector<char>> bytes = packet.getData().readPrefixedOptional(&ByteBuffer::readPrefixedByteArray);
            if (g_zincCookieResponseParsers.contains(cookieId.toString())) g_zincCookieResponseParsers[cookieId.toString()](bytes, connection);
            break;
        }
        default: break;
        }
        break;
    }
    case ZincConnection::State::Config: {
        switch (packet.getId()) {
        case 0: {
            connection->m_info.m_settingsInfo.m_locale = packet.getData().readString();
            connection->m_info.m_settingsInfo.m_renderDistance = packet.getData().readByte();
            connection->m_info.m_settingsInfo.m_chatMode = (ZincConnectionInfo::SettingsInfo::ChatMode) packet.getData().readVarInt();
            connection->m_info.m_settingsInfo.m_enableChatColors = packet.getData().readByte();
            connection->m_info.m_settingsInfo.m_skinParts = packet.getData().readUnsignedByte();
            connection->m_info.m_settingsInfo.m_mainHand = (ZincConnectionInfo::SettingsInfo::MainHand) packet.getData().readVarInt();
            connection->m_info.m_settingsInfo.m_enableTextFiltering = packet.getData().readByte();
            connection->m_info.m_settingsInfo.m_idkWhyIEvenListItHereButOk__enableServerListing = packet.getData().readByte();
            connection->m_info.m_settingsInfo.m_particleStatus = (ZincConnectionInfo::SettingsInfo::ParticleStatus) packet.getData().readVarInt();
            if (connection->m_info.m_settingsInfo.m_renderDistance > g_zincConfig.m_viewDistance)
                connection->m_info.m_settingsInfo.m_renderDistance = g_zincConfig.m_viewDistance;
            replyPacket.setId(6);
            connection->send(replyPacket);
            replyPacket.setId(4);
            connection->m_info.m_networkInfo.m_lastKeepAlive = time(nullptr);
            connection->m_info.m_networkInfo.m_verifyToken = RandomUtil::randomBytes(4);
            replyPacket.getData().writeNumeric<int64_t>(connection->m_info.m_networkInfo.m_lastKeepAlive);
            connection->send(replyPacket);
            replyPacket.setData({});
            replyPacket.setId(5);
            replyPacket.getData().writeArray<unsigned char>(connection->m_info.m_networkInfo.m_verifyToken, &ByteBuffer::writeUnsignedByte);
            connection->send(replyPacket);
            replyPacket.setData({});
            replyPacket.setId(12);
            replyPacket.getData().writePrefixedArray<Identifier>({ Identifier("minecraft", "vanilla") }, &ByteBuffer::writeIdentifier);
            connection->send(replyPacket);
            replyPacket.setData({});
            break;
        }
        case 2: {
            connection->send(g_zincServerPluginChannels[packet.getData().readIdentifier().toString()](packet.getData(), connection));
            break;
        }
        case 3: {
            connection->setState(ZincConnection::State::Config);
            break;
        }
        case 4: {
            int64_t keepAlive = packet.getData().readNumeric<int64_t>();
            if (connection->m_info.m_networkInfo.m_lastKeepAlive != keepAlive) {
                // send error via disconnect 2
            } else connection->m_info.m_networkInfo.m_lastKeepAlive = -1;
            break;
        }
        case 5: {
            std::vector<unsigned char> ping = packet.getData().readArray<unsigned char>(&ByteBuffer::readUnsignedByte, 4);
            if (connection->m_info.m_networkInfo.m_verifyToken != ping) {
                // send error via disconnect 2
            }
            break;
        }
        default: break;
        }
        break;
    }
    default: break;
    }
}
void ZincServer::onEvent(bufferevent *bev, short events, void *ctx) {
    int fd = bufferevent_getfd(bev);
    m_zincLogger.debug("Event triggered: " + std::to_string(events) + " on fd " + std::to_string(fd));
    if (!g_zincServer.isConnected(bufferevent_getfd(bev))) return;
    ZincConnection* connection = g_zincServer.getClient(bufferevent_getfd(bev));
    if (events & BEV_EVENT_EOF) {
        m_zincLogger.info("Client [" + connection->getTCPConnection().getIP() + "] disconnected!"); 
        g_zincServer.removeClient(fd);
    } else if (events & BEV_EVENT_ERROR) {
        m_zincLogger.error("Client [" + connection->getTCPConnection().getIP() + "] encountered libevent error!"); 
        g_zincServer.removeClient(fd);
    } else if (events & BEV_EVENT_TIMEOUT) {
        m_zincLogger.info("Client [" + connection->getTCPConnection().getIP() + "] timed out!"); 
        g_zincServer.removeClient(fd);
    }
}

}
