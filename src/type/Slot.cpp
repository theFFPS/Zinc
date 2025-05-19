#include <type/Slot.h>
#include <util/crypto/CRC32.h>
#include <type/ByteBuffer.h>

namespace zinc {

bool ComponentWrapper::operator==(const ComponentWrapper& component) const {
    return component.m_dataAndType == m_dataAndType && component.m_type == m_type;
}
bool ComponentWrapper::operator!=(const ComponentWrapper& component) const {
    return !operator==(component);
}

std::vector<char> Slot::toBytes() const {
    ByteBuffer buffer;
    buffer.writeVarNumeric<int>(m_itemCount);
    if (m_itemCount > 0) {
        buffer.writeVarNumeric<int>(m_itemId);
        buffer.writeVarNumeric<int>(m_componentsToAdd.size());
        buffer.writeVarNumeric<int>(m_componentsToRemove.size());
        for (const ComponentWrapper& component : m_componentsToAdd) {
            buffer.writeByteArray(component.m_dataAndType);
        }
        for (const int& component : m_componentsToRemove) buffer.writeVarNumeric<int>(component);
    }
    return buffer.getBytes();
}
std::vector<char> Slot::toBytesHashed() const {
    ByteBuffer buffer;
    buffer.writeByte(m_itemCount > 0);
    if (m_itemCount > 0) {
        buffer.writeVarNumeric<int>(m_itemId);
        buffer.writeVarNumeric<int>(m_itemCount);
        buffer.writeVarNumeric<int>(m_componentsToAdd.size());
        for (const ComponentWrapper& component : m_componentsToAdd) {
            buffer.writeVarNumeric<int>(component.m_type);
            buffer.writeNumeric<unsigned>(crc32((unsigned char*) component.m_dataAndType.data(), component.m_dataAndType.size()));
        }
        buffer.writeVarNumeric<int>(m_componentsToRemove.size());
        for (const int& component : m_componentsToRemove) buffer.writeVarNumeric<int>(component);
    }
    return buffer.getBytes();
}
bool Slot::operator==(const Slot& slot) const {
    return slot.m_itemId == m_itemId && slot.m_itemCount == m_itemCount && slot.m_componentsToAdd == m_componentsToAdd && 
            slot.m_componentsToRemove == m_componentsToRemove;
}
bool Slot::operator!=(const Slot& slot) const {
    return !operator==(slot);
}

}