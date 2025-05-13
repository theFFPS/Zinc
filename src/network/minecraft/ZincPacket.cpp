#include <network/minecraft/ZincPacket.h>

namespace zinc {

int& ZincPacket::getId() {
    return m_id;
}
int ZincPacket::getId() const {
    return m_id;
}
ByteBuffer& ZincPacket::getData() {
    return m_data;
}
ByteBuffer ZincPacket::getData() const {
    return ByteBuffer(m_data.getBytes(), m_data.getReadPointer(), m_data.getIsBigEndian());
}
void ZincPacket::setId(const int& id) {
    m_id = id;
}
void ZincPacket::setData(const ByteBuffer& data) {
    m_data.setIsBigEndian(data.getIsBigEndian());
    m_data.setBytes(data.getBytes());
    m_data.setReadPointer(data.getReadPointer());
}
bool ZincPacket::operator==(const ZincPacket& packet) const {
    return m_data == packet.getData() && m_id == packet.getId();
}
bool ZincPacket::operator!=(const ZincPacket& packet) const {
    return !operator==(packet);
}

}