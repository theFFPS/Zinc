#include <type/Slot.h>
#include <util/crypto/CRC32.h>
#include <type/ByteBuffer.h>

namespace zinc {

std::vector<char> Slot::toBytes() const {
    ByteBuffer buffer;
    buffer.writeVarInt(m_itemCount);
    if (m_itemCount > 0) {
        buffer.writeVarInt(m_itemId);
        buffer.writeVarInt(m_componentsToAdd.size());
        buffer.writeVarInt(m_componentsToRemove.size());
        for (const ComponentWrapper& component : m_componentsToAdd) {
            buffer.writeByteArray(component.m_dataAndType);
        }
        for (const int& component : m_componentsToRemove) buffer.writeVarInt(component);
    }
    return buffer.getBytes();
}
std::vector<char> Slot::toBytesHashed() const {
    ByteBuffer buffer;
    buffer.writeByte(m_itemCount > 0);
    if (m_itemCount > 0) {
        buffer.writeVarInt(m_itemId);
        buffer.writeVarInt(m_itemCount);
        buffer.writeVarInt(m_componentsToAdd.size());
        for (const ComponentWrapper& component : m_componentsToAdd) {
            buffer.writeVarInt(component.m_type);
            buffer.writeNumeric<unsigned>(crc32((unsigned char*) component.m_dataAndType.data(), component.m_dataAndType.size()));
        }
        buffer.writeVarInt(m_componentsToRemove.size());
        for (const int& component : m_componentsToRemove) buffer.writeVarInt(component);
    }
    return buffer.getBytes();
}

}