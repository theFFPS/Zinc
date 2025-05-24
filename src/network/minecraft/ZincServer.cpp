#include <network/minecraft/ZincServer.h>
#include <network/minecraft/channels/DefaultChannels.h>
#include <registry/DefaultRegistries.h>
#include <external/JSON.h>
#include <util/Logger.h>
#include <util/crypto/Random.h>
#include <util/crypto/MCSHA1.h>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <fstream>
#include <ZincConstants.h>
#include <ZincConfig.h>

namespace zinc {

Logger m_zincLogger = Logger("ZincServer");

ZincServer g_zincServer;

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
    if (!isConnected(client->getTCPConnection().getFd())) {
        std::lock_guard lock(m_mutex);
        std::string ip = client->getTCPConnection().getIP();
        if (m_rateLimiterData.contains(ip)) {
            if (m_rateLimiterData[ip] + g_zincConfig.m_core.m_security.m_rateLimit < time(nullptr)) m_rateLimiterData[ip] = time(nullptr);
            else client->m_kickAtLogin = 1;
        } else m_rateLimiterData.insert({ ip, time(nullptr) });
        if (m_concurrentAccounts.contains(ip)) m_concurrentAccounts[ip]++;
        else m_concurrentAccounts.insert({ ip, 1 });
        if (m_concurrentAccounts[ip] > g_zincConfig.m_core.m_security.m_maxConcurrentAccount) client->m_kickAtLogin = 2;
        m_clients.insert({ client->getTCPConnection().getFd(), client });
    }
}
bool ZincServer::isConnected(evutil_socket_t fd) const {
    return m_clients.contains(fd);
}
void ZincServer::removeClient(evutil_socket_t fd) {
    if (isConnected(fd)) {
        std::lock_guard lock(m_mutex);
        std::string ip = m_clients[fd]->getTCPConnection().getIP();
        if (m_concurrentAccounts[ip]) m_concurrentAccounts[ip]--;
        if (!m_concurrentAccounts[ip]) m_concurrentAccounts.erase(ip);
        if (m_clients[fd]->m_loginFinished) m_onlinePlayers--;
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
            connection->m_info.m_networkInfo.m_protocolVersion = packet.getData().readVarNumeric<int>();
            connection->m_info.m_networkInfo.m_serverAddr = packet.getData().readString();
            connection->m_info.m_networkInfo.m_serverPort = packet.getData().readNumeric<unsigned short>();
            connection->setState((ZincConnection::State) packet.getData().readVarNumeric<int>());
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
                    { "max", g_zincConfig.m_core.m_network.m_maxPlayerCount },
                    { "online", g_zincServer.m_onlinePlayers },
                    { "sample", {} }
                } },
                { "description", {
                    { "text", g_zincConfig.m_core.m_network.m_motd }
                } },
                { "favicon", "data:image/png;base64," + g_zincConfig.m_core.m_network.m_iconBase64 },
                { "enforcesSecureChat", true }
            }.dump());
            connection->send(replyPacket);
        } else connection->send(packet);
        break;
    }
    case ZincConnection::State::Login: {
        switch(packet.getId()) {
        /* LOGIN START */ case 0: {
            if (connection->m_kickAtLogin) {
                switch (connection->m_kickAtLogin) {
                case 1: connection->sendLoginError("You have been rate limited. Try again later"); return;
                case 2: connection->sendLoginError("You have reached a maximum amount of concurrent account"); return;
                default: connection->sendLoginError("Unknown error"); return;
                }
            }
            if (g_zincServer.m_onlinePlayers >= g_zincConfig.m_core.m_network.m_maxPlayerCount) {
                connection->sendLoginError("Server is full");
                return;
            }
            if (connection->m_info.m_networkInfo.m_protocolVersion < LATEST_MINECRAFT_VERSION_PROTOCOL) {
                connection->sendLoginError("You are using outdated version");
            } else if (connection->m_info.m_networkInfo.m_protocolVersion > LATEST_MINECRAFT_VERSION_PROTOCOL) {
                connection->sendLoginError("Server is using outdated version");
            } else {
                connection->m_info.m_playerInfo.m_playerName = packet.getData().readString();
                connection->m_info.m_playerInfo.m_playerUUID = packet.getData().readUUID();
                if (connection->m_info.m_playerInfo.m_playerName.size() > 16) connection->sendLoginError("Player name can't be longer than 16 characters");
                else {
                    if (g_zincConfig.m_core.m_network.m_threshold > 0) connection->setupCompression();
                    replyPacket.setId(1);
                    replyPacket.getData().writeString("");
                    replyPacket.getData().writePrefixedArray<unsigned char>(g_zincServer.getRSA().getPublicKeyDER(), &ByteBuffer::writeUnsignedByte);
                    connection->m_info.m_networkInfo.m_verifyToken = RandomUtil::randomBytes(64);
                    replyPacket.getData().writePrefixedArray<unsigned char>(connection->m_info.m_networkInfo.m_verifyToken, &ByteBuffer::writeUnsignedByte);
                    replyPacket.getData().writeByte(g_zincConfig.m_core.m_security.m_onlineMode);
                    connection->send(replyPacket);
                }
            }
            break;
        }
        /* ENCRYPTION RESPONSE */ case 1: {
            std::vector<unsigned char> sharedSecret = packet.getData().readPrefixedArray<unsigned char>(&ByteBuffer::readUnsignedByte);
            std::vector<unsigned char> verifyToken = packet.getData().readPrefixedArray<unsigned char>(&ByteBuffer::readUnsignedByte);
            sharedSecret = g_zincServer.getRSA().decrypt(sharedSecret);
            if (g_zincServer.getRSA().decrypt(verifyToken) == connection->m_info.m_networkInfo.m_verifyToken) {
                connection->m_info.m_networkInfo.m_verifyToken.clear();
                connection->setupEncryption(sharedSecret);
                if (g_zincConfig.m_core.m_security.m_onlineMode) {
                    MCSHA1 sha;
                    sha.update(std::vector<char>({}));
                    sha.update(sharedSecret);
                    sha.update(g_zincServer.getRSA().getPublicKeyDER());
                    sha.final();
                    std::string url = "https://sessionserver.mojang.com/session/minecraft/hasJoined?username=" + 
                                    connection->m_info.m_playerInfo.m_playerName + "&serverId=" + sha.digestJava();
                    bool gotData = false;
                    for (int i = 0; i < 5; i++) {
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
                            gotData = true;
                        } catch (std::exception) { continue; }
                    }
                    if (!gotData) {
                        connection->sendLoginError("Server is unable to access mojang session servers");
                        return;
                    }
                }
                if (g_zincCookieRequests.contains(ZincConnection::State::Login)) 
                    for (const std::string& cookieRequest : g_zincCookieRequests[ZincConnection::State::Login]) 
                        connection->sendCookieRequest(cookieRequest);
                if (g_zincServerInitPluginChannels.contains(ZincConnection::State::Login)) 
                    for (const auto& pluginRequest : g_zincServerInitPluginChannels[ZincConnection::State::Login]) 
                        connection->sendPluginMessage(Identifier(pluginRequest.first), pluginRequest.second(connection));
                replyPacket.setId(2);
                replyPacket.getData().writeUUID(connection->m_info.m_playerInfo.m_playerUUID);
                replyPacket.getData().writeString(connection->m_info.m_playerInfo.m_playerName);
                replyPacket.getData().writePrefixedArray<ZincConnectionProperty>(connection->m_info.m_playerInfo.m_properties, &writeZincConnectionProperty);
                connection->send(replyPacket);
            } else {
                connection->sendLoginError("Server received invalid encryption response");
            }
            break;
        }
        /* LOGIN PLUGIN RESPONSE */ case 2: {
            std::vector<unsigned char> id = packet.getData().readArray<unsigned char>(&ByteBuffer::readUnsignedByte, 4);
            if (connection->isLoginPluginChannelOpened(id)) {
                g_zincServerPluginChannels[connection->getLoginPluginChannel(id)](packet.getData(), connection);
                connection->closeLoginPluginChannel(id);
            } else {
                connection->sendLoginError("Server received invalid login plugin channel id");
            }
            break;
        }
        /* LOGIN ACK */ case 3: {
            connection->setState(ZincConnection::State::Config);
            connection->m_mutex.lock();
            connection->m_loginFinished = true;
            connection->m_mutex.unlock();
            std::lock_guard lock (g_zincServer.m_mutex);
            g_zincServer.m_onlinePlayers++;
            break;
        }
        /* COOKIE RESPONSE */ case 4: {
            Identifier cookieId = packet.getData().readIdentifier();
            std::optional<std::vector<char>> bytes = packet.getData().readPrefixedOptional(&ByteBuffer::readPrefixedByteArray);
            if (bytes.has_value()) {
                ByteBuffer cookieRawData = bytes.value();
                std::optional<std::vector<char>> cookieData = connection->extractCookieData(cookieRawData)
                    .readPrefixedOptional<std::vector<char>>(&ByteBuffer::readPrefixedByteArray);
                g_zincCookieResponseParsers[cookieId.toString()](cookieData, connection);
            } else g_zincCookieResponseParsers[cookieId.toString()](bytes, connection);
            break;
        }
        default: break;
        }
        break;
    }
    case ZincConnection::State::Config: {
        switch (packet.getId()) {
        /* CONFIG INFO */ case 0: {
            connection->m_info.m_settingsInfo.m_locale = packet.getData().readString();
            connection->m_info.m_settingsInfo.m_renderDistance = packet.getData().readByte();
            connection->m_info.m_settingsInfo.m_chatMode = (ZincConnectionInfo::SettingsInfo::ChatMode) packet.getData().readVarNumeric<int>();
            connection->m_info.m_settingsInfo.m_enableChatColors = packet.getData().readByte();
            connection->m_info.m_settingsInfo.m_skinParts = packet.getData().readUnsignedByte();
            connection->m_info.m_settingsInfo.m_mainHand = (ZincConnectionInfo::SettingsInfo::MainHand) packet.getData().readVarNumeric<int>();
            connection->m_info.m_settingsInfo.m_enableTextFiltering = packet.getData().readByte();
            connection->m_info.m_settingsInfo.m_idkWhyIEvenListItHereButOk__enableServerListing = packet.getData().readByte();
            connection->m_info.m_settingsInfo.m_particleStatus = (ZincConnectionInfo::SettingsInfo::ParticleStatus) packet.getData().readVarNumeric<int>();
            if (connection->m_info.m_settingsInfo.m_renderDistance > g_zincConfig.m_core.m_optimizations.m_viewDistance)
                connection->m_info.m_settingsInfo.m_renderDistance = g_zincConfig.m_core.m_optimizations.m_viewDistance;
            replyPacket.setId(6);
            connection->send(replyPacket);
            replyPacket.setId(4);
            connection->m_info.m_networkInfo.m_lastKeepAlive = time(nullptr);
            connection->m_info.m_networkInfo.m_verifyToken = RandomUtil::randomBytes(4);
            replyPacket.getData().writeNumeric<int64_t>(connection->m_info.m_networkInfo.m_lastKeepAlive);
            connection->send(replyPacket);
            replyPacket.getData().clear();
            replyPacket.setId(5);
            replyPacket.getData().writeArray<unsigned char>(connection->m_info.m_networkInfo.m_verifyToken, &ByteBuffer::writeUnsignedByte);
            connection->send(replyPacket);
            replyPacket.getData().clear();
            replyPacket.setId(12);
            replyPacket.getData().writePrefixedArray<Identifier>({ Identifier("minecraft", "vanilla") }, &ByteBuffer::writeIdentifier);
            connection->send(replyPacket);
            replyPacket.getData().clear();
            if (g_zincCookieRequests.contains(ZincConnection::State::Config)) 
                for (const std::string& cookieRequest : g_zincCookieRequests[ZincConnection::State::Config]) 
                    connection->sendCookieRequest(cookieRequest);
            if (g_zincServerInitPluginChannels.contains(ZincConnection::State::Config)) 
                for (const auto& pluginMessage : g_zincServerInitPluginChannels[ZincConnection::State::Config]) 
                    connection->sendPluginMessage(Identifier(pluginMessage.first), pluginMessage.second(connection));
            break;
        }
        /* COOKIE RESPONSE */ case 1: {
            Identifier cookieId = packet.getData().readIdentifier();
            std::optional<std::vector<char>> bytes = packet.getData().readPrefixedOptional(&ByteBuffer::readPrefixedByteArray);
            if (bytes.has_value()) {
                ByteBuffer cookieRawData = bytes.value();
                std::optional<std::vector<char>> cookieData = connection->extractCookieData(cookieRawData)
                    .readPrefixedOptional<std::vector<char>>(&ByteBuffer::readPrefixedByteArray);
                g_zincCookieResponseParsers[cookieId.toString()](cookieData, connection);
            } else g_zincCookieResponseParsers[cookieId.toString()](bytes, connection);
            break;
        }
        /* PLUGIN MESSAGE C2S */ case 2: {
            g_zincServerPluginChannels[packet.getData().readIdentifier().toString()](packet.getData(), connection);
            break;
        }
        /* CONFIG ACK */ case 3: {
            connection->setState(ZincConnection::State::Play);
            break;
        }
        /* KEEP ALIVE */ case 4: {
            int64_t keepAlive = packet.getData().readNumeric<int64_t>();
            if (connection->m_info.m_networkInfo.m_lastKeepAlive != keepAlive) connection->sendLoginError("Server received invalid keep alive packet");
            else {
                connection->m_info.m_networkInfo.m_lastKeepAlive = -1;
                connection->sendDisconnect(TextComponentBuilder().text("Currently WIP").build());
            }
            break;
        }
        /* PING */ case 5: {
            std::vector<unsigned char> ping = packet.getData().readArray<unsigned char>(&ByteBuffer::readUnsignedByte, 4);
            if (connection->m_info.m_networkInfo.m_verifyToken != ping) connection->sendLoginError("Server received invalid ping packet");
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
