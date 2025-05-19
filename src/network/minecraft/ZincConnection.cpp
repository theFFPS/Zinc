#include <network/minecraft/ZincConnection.h>
#include <util/TCPUtil.h>
#include <util/ZLibUtil.h>
#include <ZincConfig.h>
#include <util/crypto/Base64.h>

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
}
void ZincConnection::setIsCompressed(const bool& isCompressed) {
    m_isCompressed = isCompressed;
}
void ZincConnection::setIsEncrypted(const bool& isEncrypted) {
    m_isEncrypted = isEncrypted;
}
void ZincConnection::setupEncryption(const std::vector<unsigned char>& secret) {
    m_encrypt.setKey(secret);
    m_encrypt.setIV(secret);
    m_decrypt.setKey(secret);
    m_decrypt.setIV(secret);
    setIsEncrypted(true);
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
        if (dataLength < g_zincConfig.m_threshold) {
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
            tmpData.writeVarNumeric<int>(packet.getData().size());
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
bool ZincConnection::operator==(const ZincConnection& connection) const {
    return m_tcpConnection == connection.getTCPConnection();
}
bool ZincConnection::operator!=(const ZincConnection& connection) const {
    return m_tcpConnection != connection.getTCPConnection();
}

}