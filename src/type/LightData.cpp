#include <type/LightData.h>
#include <type/ByteBuffer.h>

namespace zinc {

std::vector<char> LightData::toBytes() const {
    ByteBuffer buffer;
    buffer.writeBitSet(m_skyLightMask);
    buffer.writeBitSet(m_blockLightMask);
    buffer.writeBitSet(m_emptySkyLightMask);
    buffer.writeBitSet(m_emptyBlockLightMask);
    buffer.writePrefixedArray<std::vector<char>>(m_skyLightArrays, &ByteBuffer::writePrefixedByteArray);
    buffer.writePrefixedArray<std::vector<char>>(m_blockLightArrays, &ByteBuffer::writePrefixedByteArray);
    return buffer.getBytes();
}
bool LightData::operator==(const LightData& data) const {
    return data.toBytes() == toBytes();
}
bool LightData::operator!=(const LightData& data) const {
    return data.toBytes() == toBytes();
}

}