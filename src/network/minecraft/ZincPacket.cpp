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
    ByteBuffer data;
    data.m_isBigEndian = m_data.m_isBigEndian;
    data.m_internalBuffer.write(m_data.getBytes().data(), m_data.size());
    data.m_internalBuffer.toggleBlockRecycle(m_data.m_internalBuffer.areBlocksRecycled());
    return data;
}
void ZincPacket::setId(const int& id) {
    m_id = id;
}
void ZincPacket::setData(const ByteBuffer& data) {
    m_data.m_isBigEndian = data.m_isBigEndian;
    m_data.m_internalBuffer.write(data.getBytes().data(), data.size());
    m_data.m_internalBuffer.toggleBlockRecycle(data.m_internalBuffer.areBlocksRecycled());
}
bool ZincPacket::operator==(const ZincPacket& packet) const {
    return m_data == packet.getData() && m_id == packet.getId();
}
bool ZincPacket::operator!=(const ZincPacket& packet) const {
    return !operator==(packet);
}

}