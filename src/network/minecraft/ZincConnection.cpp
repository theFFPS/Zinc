#include <network/minecraft/ZincConnection.h>
#include <network/minecraft/ZincServer.h>
#include <util/TCPUtil.h>
#include <util/ZLibUtil.h>
#include <ZincConfig.h>
#include <util/crypto/Base64.h>
#include <util/crypto/Random.h>
#include <external/JSON.h>

namespace zinc {

void writeZincConnectionProperty(const ZincConnectionProperty& property, ByteBuffer& buffer) {
    buffer.writeString(property.m_name);
    buffer.writeString(property.m_value);
    buffer.writePrefixedOptional<std::string>(property.m_signature, &ByteBuffer::writeString);
}

TCPConnection& ZincConnection::getTCPConnection() {
    return m_tcpConnection;
}
TCPConnection ZincConnection::getTCPConnection() const {
    return m_tcpConnection;
}
ZincConnection::State& ZincConnection::getState() {
    return m_state;
}
ZincConnection::State ZincConnection::getState() const {
    return m_state;
}
bool& ZincConnection::getIsCompressed() {
    return m_isCompressed;
}
bool ZincConnection::getIsCompressed() const {
    return m_isCompressed;
}
bool& ZincConnection::getIsEncrypted() {
    return m_isEncrypted;
}
bool ZincConnection::getIsEncrypted() const {
    return m_isEncrypted;
}
void ZincConnection::setState(const State& state) {
    m_state = state;
    Logger("ZincConnection").debug("switch state to " + std::to_string((int) state));
}
void ZincConnection::setIsCompressed(const bool& isCompressed) {
    m_isCompressed = isCompressed;
}
void ZincConnection::setIsEncrypted(const bool& isEncrypted) {
    m_isEncrypted = isEncrypted;
}
void ZincConnection::setupCompression() {
    if (m_state != State::Login) return;
    ZincPacket packet;
    packet.setId(3);
    packet.getData().writeVarNumeric<int>(g_zincConfig.m_core.m_network.m_threshold);
    send(packet);
    setIsCompressed(true);
}
void ZincConnection::setupEncryption(const std::vector<unsigned char>& secret) {
    m_encrypt.setKey(secret);
    m_encrypt.setIV(secret);
    m_decrypt.setKey(secret);
    m_decrypt.setIV(secret);
    m_isEncrypted = true;
}
ZincPacket ZincConnection::read() {
    m_mutex.lock();
    ByteBuffer tmpBuffer = m_tcpConnection.read(), buffer;
    m_mutex.unlock();
    if (m_isEncrypted) {
        buffer.writeArray<unsigned char>(
            m_decrypt.decryptCFB8(tmpBuffer.readArray<unsigned char>(&ByteBuffer::readUnsignedByte, tmpBuffer.size())),
            &ByteBuffer::writeUnsignedByte
        );
    } else {
        buffer.m_internalBuffer.write(tmpBuffer.getBytes().data(), tmpBuffer.size());
    }
    tmpBuffer.clear();
    size_t length = buffer.readVarNumeric<int>();
    int packetId = -1;
    ByteBuffer dataBuffer;
    if (length > buffer.size()) {
        return ZincPacket(-1); // wait for more data
    } else TCPUtil::drain(m_tcpConnection.getBuffer(), buffer.getVarNumericLength<int>(length) + length);
    if (m_isCompressed) {
        size_t dataLength = buffer.readVarNumeric<int>();
        if (dataLength) {
            tmpBuffer.writeBytes(buffer.readBytes(length - buffer.getVarNumericLength<int>(dataLength)));
            ByteBuffer buffer = ZLibUtil::uncompress(tmpBuffer, dataLength);
            tmpBuffer.clear();
            packetId = buffer.readVarNumeric<int>();
            dataBuffer.writeBytes(buffer.readBytes(dataLength - buffer.getVarNumericLength<int>(packetId)));
        } else {
            packetId = buffer.readVarNumeric<int>();
            dataBuffer.writeBytes(buffer.readBytes(length - (buffer.getVarNumericLength<int>(packetId) + 1))); // 1 = size of varint(0)
        }
    } else {
        packetId = buffer.readVarNumeric<int>();
        dataBuffer.writeBytes(buffer.readBytes(length - buffer.getVarNumericLength<int>(packetId)));
    }
    buffer.clear();
    return ZincPacket(packetId, dataBuffer);
}
void ZincConnection::send(const ZincPacket& packet) {
    ByteBuffer tmpData, data;
    int dataLength = tmpData.getVarNumericLength<int>(packet.getId()) + packet.getData().size();
    if (m_isCompressed) {
        if (dataLength < g_zincConfig.m_core.m_network.m_threshold) {
            tmpData.writeVarNumeric<int>(1 + dataLength); // size of varint(0) + dataLength
            tmpData.writeVarNumeric<int>(0);
            tmpData.writeVarNumeric<int>(packet.getId());
            tmpData.writeBytes(packet.getData().getBytes());
        } else {
            tmpData.writeVarNumeric<int>(packet.getId());
            tmpData.writeBytes(packet.getData().getBytes());
            ByteBuffer compressedData = ZLibUtil::compress(tmpData);
            tmpData.clear();
            tmpData.writeVarNumeric<int>(tmpData.getVarNumericLength<int>(dataLength) + compressedData.size());
            tmpData.writeVarNumeric<int>(dataLength);
            tmpData.writeByteArray(compressedData.getBytes());
            compressedData.clear();
        }
    } else {
        tmpData.writeVarNumeric<int>(dataLength);
        tmpData.writeVarNumeric<int>(packet.getId());
        tmpData.writeBytes(packet.getData().getBytes());
    }
    if (m_isEncrypted) {
        data.writeArray<unsigned char>(
            m_encrypt.encryptCFB8(tmpData.readArray<unsigned char>(&ByteBuffer::readUnsignedByte, tmpData.size())),
            &ByteBuffer::writeUnsignedByte
        );
    } else data.writeBytes(tmpData.getBytes());
    tmpData.clear();
    m_mutex.lock();
    m_tcpConnection.send(data);
    m_mutex.unlock();
    Logger("ZincConnection").debug("Sent packet with id " + std::to_string(packet.getId()));
}
ByteBuffer ZincConnection::extractCookieData(ByteBuffer& cookieRawData) {
    ByteBuffer cookieData;
    ByteBuffer errorBuffer;
        errorBuffer.writeByte(false);
    std::string stringPayload = cookieRawData.readString();
    // TODO: add signature support
    try {
        std::string stringJSON = stringPayload; // change this in the future
        nlohmann::json JSON = nlohmann::json::parse(stringJSON);
        if (JSON["lifetime"] < time(nullptr)) return errorBuffer;
        cookieData.writeByte(true);
        cookieData.writePrefixedArray<unsigned char>(Base64::decode(JSON["data"]), &ByteBuffer::writeUnsignedByte);
        return cookieData;
    } catch (const std::exception& /* ignore it */) {
        return errorBuffer;
    }
}
void ZincConnection::sendCookieRequest(const Identifier& cookieId) {
    ZincPacket packet;
    switch (m_state) {
    case State::Login: packet.setId(5); break;
    case State::Config: packet.setId(0); break;
    case State::Play: packet.setId(0x15); break;
    default: return;
    }
    packet.getData().writeIdentifier(cookieId);
    send(packet);
}
void ZincConnection::storeCookie(const Identifier& cookieId, const std::vector<char>& payload, long lifetime) {
    ZincPacket packet;
    switch (m_state) {
    case State::Config: packet.setId(0x0A); break;
    case State::Play: packet.setId(0x71); break;
    default: return;
    }
    packet.getData().writeIdentifier(cookieId);
    nlohmann::json JSON = {
        { "lifetime", time(nullptr) + lifetime },
        { "data", Base64::encode(payload) }, // TODO: implement encryption
        { "nonce", Base64::encode(RandomUtil::randomBytes(32)) }
    };
    std::string stringJSON = JSON.dump();
    std::string stringPayload = stringJSON; // TODO: implement signing
    packet.getData().writeString(stringPayload);
    send(packet);
}
void ZincConnection::storeCookie(const Identifier& cookieId, const ByteBuffer& payload, long lifetime) {
    storeCookie(cookieId, payload.getBytes(), lifetime);
}
void ZincConnection::sendPluginMessage(const Identifier& pluginChannel, const std::vector<char>& data) {
    ZincPacket packet;
    switch (m_state) {
    case State::Login: {
        packet.setId(4);
        packet.getData().writeVarNumeric<int>(openLoginPluginChannel(pluginChannel.toString()));
        break;
    }
    case State::Config: packet.setId(1); break;
    case State::Play: packet.setId(0x18); break;
    default: return;
    }
    packet.getData().writeIdentifier(pluginChannel);
    packet.getData().writeByteArray(data);
    send(packet);
}
void ZincConnection::sendPluginMessage(const Identifier& pluginChannel, const ByteBuffer& data) {
    sendPluginMessage(pluginChannel, data.getBytes());
}
void ZincConnection::sendDisconnect(const TextComponent& text) {
    ZincPacket packet;
    switch (m_state) {
    case State::Login: packet.setId(0); break;
    case State::Config: packet.setId(2); break;
    case State::Play: packet.setId(0x1C); break;
    default: return;
    }
    if (m_state == State::Login) packet.getData().writeString(text.encodeJSON());
    else packet.getData().writeTextComponent(text);
    send(packet);
}
void ZincConnection::sendLoginError(const std::string& errorMessage) {
    sendDisconnect(TextComponentBuilder()
        .text("Zinc Login Error").color("dark_red").bold()
        .append(TextComponentBuilder().text("\n").build())
        .append(TextComponentBuilder().text(errorMessage).bold(false).color("red").build()).build());
}
int ZincConnection::openLoginPluginChannel(const std::string& channel) {
    std::lock_guard lock(m_mutex);
    std::vector<unsigned char> id = RandomUtil::randomBytes(3);
    while (isLoginPluginChannelOpened(id)) {
        id = RandomUtil::randomBytes(3);
    }
    m_openedLoginPluginChannels.insert({ id, channel });
    int result{};
    std::memcpy(&result, id.data(), 3);
    return result;
}
std::string ZincConnection::getLoginPluginChannel(const std::vector<unsigned char>& id) {
    if (!isLoginPluginChannelOpened(id)) return "minecraft:brand";
    return m_openedLoginPluginChannels[id];
}
bool ZincConnection::isLoginPluginChannelOpened(const std::vector<unsigned char>& id) {
    return m_openedLoginPluginChannels.contains(id);
}
void ZincConnection::closeLoginPluginChannel(const std::vector<unsigned char>& id) {
    std::lock_guard lock(m_mutex);
    if (isLoginPluginChannelOpened(id)) m_openedLoginPluginChannels.erase(id);
}
bool ZincConnection::operator==(const ZincConnection& connection) const {
    return m_tcpConnection == connection.getTCPConnection();
}
bool ZincConnection::operator!=(const ZincConnection& connection) const {
    return m_tcpConnection != connection.getTCPConnection();
}

}