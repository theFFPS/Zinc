#include <type/NBT.h>
#include <type/ByteBuffer.h>

namespace zinc {

NBTTag::NBTTag() {}
NBTTag::NBTTag(ByteBuffer *buffer, NBTTagType expectedType, bool expectTagName, bool isTypeKnown, bool isInList) {
    if (!isTypeKnown) m_type = (NBTTagType) buffer->readByte(); 
    else m_type = expectedType;
    if (expectTagName && expectedType != NBTTagType::End) {
        short size = buffer->readShort();
        m_tagName = std::string(size, '\0');
        buffer->readBytes(m_tagName.data(), size);
    }
    switch (m_type) {
    case NBTTagType::Byte: m_byteValue = buffer->readByte(); break;
    case NBTTagType::Short: m_shortValue = buffer->readShort(); break;
    case NBTTagType::Int: m_intValue = buffer->readInt(); break;
    case NBTTagType::Long: m_longValue = buffer->readLong(); break;
    case NBTTagType::Float: m_floatValue = buffer->readFloat(); break;
    case NBTTagType::Double: m_doubleValue = buffer->readDouble(); break;
    case NBTTagType::ByteArray: m_byteArrayValue = buffer->readByteArray(buffer->readInt()); break;
    case NBTTagType::String: {
        short size = buffer->readShort();
        m_stringValue = std::string(size, '\0');
        buffer->readBytes(m_stringValue.data(), size);
        break;
    }
    case NBTTagType::List: {
        NBTTagType elementType = (NBTTagType) buffer->readByte();
        int size = buffer->readInt();
        for (int i = 0; i < size; i++) m_childrenTagsValue.push_back(NBTTag(buffer, elementType, false, true, true));
        break;
    }
    case NBTTagType::Compound: {
        while (true) {
            NBTTagType elementType = (NBTTagType) buffer->readByte();
            if (!isInList) {
                if (elementType == NBTTagType::End) break;
            } else if (elementType == NBTTagType::End || elementType == NBTTagType::Compound) break;
            m_childrenTagsValue.push_back(NBTTag(buffer, elementType, true, true));
        }
        break;
    }
    case NBTTagType::IntArray: m_intArrayValue = buffer->readArray<int>(buffer->readInt(), &ByteBuffer::readInt); break;
    case NBTTagType::LongArray: m_longArrayValue = buffer->readArray<long>(buffer->readInt(), &ByteBuffer::readLong); break;
    default: break;
    }
}
std::vector<char> NBTTag::encode(bool ignoreNametag, bool isInList) {
    ByteBuffer result;
    if (!isInList) result.writeByte((char) m_type);
    if (!ignoreNametag && m_type != NBTTagType::End) {
        result.writeShort(m_tagName.size());
        result.writeByteArray(std::vector<char>(m_tagName.begin(), m_tagName.end()));
    }
    switch (m_type) {
    case NBTTagType::Byte: result.writeByte(m_byteValue); break;
    case NBTTagType::Short: result.writeShort(m_shortValue); break;
    case NBTTagType::Int: result.writeInt(m_intValue); break;
    case NBTTagType::Long: result.writeLong(m_longValue); break;
    case NBTTagType::Float: result.writeFloat(m_floatValue); break;
    case NBTTagType::Double: result.writeDouble(m_doubleValue); break;
    case NBTTagType::ByteArray: result.writeInt(m_byteArrayValue.size()); result.writeByteArray(m_byteArrayValue); break;
    case NBTTagType::String: result.writeShort(m_stringValue.size()); result.writeBytes(m_stringValue.data(), m_stringValue.size()); break;
    case NBTTagType::List: {
        if (m_childrenTagsValue.empty()) result.writeByte(0);
        else {
            NBTTagType type = m_childrenTagsValue[0].m_type;
            for (NBTTag& tag : m_childrenTagsValue) if (type != tag.m_type) {
                type = NBTTagType::End;
                break;
            }
            result.writeByte((char) type);
        }
        if (result.getBytes()[result.getBytes().size() - 1] != 0) {
            result.writeInt(m_childrenTagsValue.size());
            for (NBTTag& tag : m_childrenTagsValue) result.writeByteArray(tag.encode(true, true));
            if (m_childrenTagsValue[0].m_type == NBTTagType::Compound) result.writeByte(0);
        } else result.writeInt(0);
        break;
    }
    case NBTTagType::Compound: {
        for (NBTTag& tag : m_childrenTagsValue) result.writeByteArray(tag.encode());
        result.writeByte(0);
        break;
    }
    case NBTTagType::IntArray: result.writeInt(m_intArrayValue.size()); result.writeArray<int>(m_intArrayValue, &ByteBuffer::writeInt); break;
    case NBTTagType::LongArray: result.writeInt(m_longArrayValue.size()); result.writeArray<long>(m_longArrayValue, &ByteBuffer::writeLong); break;
    default: break;
    }
    return result.getBytes();
}
bool NBTTag::operator==(NBTTag& b) { return encode(true) == b.encode(true); }
bool NBTTag::operator!=(NBTTag& b) { return encode(true) != b.encode(true); }
NBTTag NBTTag::End() { return NBTTag(); }
NBTTag NBTTag::Byte(char value, std::string const& tagName) {
    NBTTag result;
    result.m_type = NBTTagType::Byte;
    result.m_tagName = tagName;
    result.m_byteValue = value;
    return result;
}
NBTTag NBTTag::Short(short value, std::string const& tagName) {
    NBTTag result;
    result.m_type = NBTTagType::Short;
    result.m_tagName = tagName;
    result.m_shortValue = value;
    return result;
}
NBTTag NBTTag::Int(int value, std::string const& tagName) {
    NBTTag result;
    result.m_type = NBTTagType::Int;
    result.m_tagName = tagName;
    result.m_intValue = value;
    return result;
}
NBTTag NBTTag::Long(long value, std::string const& tagName) {
    NBTTag result;
    result.m_type = NBTTagType::Long;
    result.m_tagName = tagName;
    result.m_longValue = value;
    return result;
}
NBTTag NBTTag::Float(float value, std::string const& tagName) {
    NBTTag result;
    result.m_type = NBTTagType::Float;
    result.m_tagName = tagName;
    result.m_floatValue = value;
    return result;
}
NBTTag NBTTag::Double(double value, std::string const& tagName) {
    NBTTag result;
    result.m_type = NBTTagType::Double;
    result.m_tagName = tagName;
    result.m_doubleValue = value;
    return result;
}
NBTTag NBTTag::ByteArray(std::vector<char> const& value, std::string const& tagName) {
    NBTTag result;
    result.m_type = NBTTagType::ByteArray;
    result.m_tagName = tagName;
    result.m_byteArrayValue = value;
    return result;
}
NBTTag NBTTag::String(std::string const& value, std::string const& tagName) {
    NBTTag result;
    result.m_type = NBTTagType::String;
    result.m_tagName = tagName;
    result.m_stringValue = value;
    return result;
}
NBTTag NBTTag::IntArray(std::vector<int> const& value, std::string const& tagName) {
    NBTTag result;
    result.m_type = NBTTagType::IntArray;
    result.m_tagName = tagName;
    result.m_intArrayValue = value;
    return result;
}
NBTTag NBTTag::LongArray(std::vector<long> const& value, std::string const& tagName) {
    NBTTag result;
    result.m_type = NBTTagType::LongArray;
    result.m_tagName = tagName;
    result.m_longArrayValue = value;
    return result;
}
NBTTag NBTTag::List(std::vector<NBTTag> const& value, std::string const& tagName) {
    NBTTag result;
    result.m_type = NBTTagType::List;
    result.m_tagName = tagName;
    result.m_childrenTagsValue = value;
    return result;
}
NBTTag NBTTag::Compound(std::vector<NBTTag> const& value, std::string const& tagName) {
    NBTTag result;
    result.m_type = NBTTagType::Compound;
    result.m_tagName = tagName;
    result.m_childrenTagsValue = value;
    return result;
}

}