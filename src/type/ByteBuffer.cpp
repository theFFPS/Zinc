#include <type/ByteBuffer.h>

namespace zinc {

ByteBuffer::ByteBuffer() : m_bytes({}), m_readPtr(0) {}
ByteBuffer::ByteBuffer(const std::vector<char>& bytes, unsigned long readPtr) : m_bytes(bytes), m_readPtr(readPtr) {}

std::vector<char> ByteBuffer::getBytes() {
    return m_bytes;
}

void ByteBuffer::writeBoolean(bool value) { writeByte(value ? 1 : 0); }
bool ByteBuffer::readBoolean() { return readByte(); }

void ByteBuffer::writeByte(char value) { writeNumeric(value); }
char ByteBuffer::readByte() { return readNumeric<char>(); }
void ByteBuffer::writeUnsignedByte(unsigned char value) { writeNumeric(value); }
unsigned char ByteBuffer::readUnsignedByte() { return readNumeric<unsigned char>(); }

void ByteBuffer::writeShort(short value) { writeNumeric(value); }
short ByteBuffer::readShort() { return readNumeric<short>(); }
void ByteBuffer::writeUnsignedShort(unsigned short value) { writeNumeric(value); }
unsigned short ByteBuffer::readUnsignedShort() { return readNumeric<unsigned short>(); }

void ByteBuffer::writeInt(int value) { writeNumeric(value); }
int ByteBuffer::readInt() { return readNumeric<int>(); }
void ByteBuffer::writeUnsignedInt(unsigned int value) { writeNumeric(value); }
unsigned int ByteBuffer::readUnsignedInt() { return readNumeric<unsigned int>(); }

void ByteBuffer::writeLong(long value) { writeNumeric(value); }
long ByteBuffer::readLong() { return readNumeric<long>(); }
void ByteBuffer::writeUnsignedLong(unsigned long value) { writeNumeric(value); }
unsigned long ByteBuffer::readUnsignedLong() { return readNumeric<unsigned long>(); }

void ByteBuffer::writeFloat(float value) { writeNumeric(value); }
float ByteBuffer::readFloat() { return readNumeric<float>(); }

void ByteBuffer::writeDouble(double value) { writeNumeric(value); }
double ByteBuffer::readDouble() { return readNumeric<double>(); }

void ByteBuffer::writeVarInt(int value) {
    while (true) {
        if ((value & ~SEGMENT_BITS) == 0) {
            writeByte(value);
            return;
        }
        writeByte((value & SEGMENT_BITS) | CONTINUE_BIT);
        value >>= 7;
    }
}
unsigned long ByteBuffer::getVarIntSize(int value) {
    unsigned long size = 0;
    while (true) {
        if ((value & ~SEGMENT_BITS) == 0) {
            ++size;
            return size;
        }
        ++size;
        value >>= 7;
    }
}
int ByteBuffer::readVarInt() {
    int value = 0;
    int position = 0;
    char currentByte;
    while (true) {
        currentByte = readByte();
        value |= (currentByte & SEGMENT_BITS) << position;
        if ((currentByte & CONTINUE_BIT) == 0) break;
        position += 7;
        if (position >= 32) throw std::runtime_error("VarInt is too big");
    }
    return value;
}

void ByteBuffer::writeVarLong(long value) {
    while (true) {
        if ((value & ~((long) SEGMENT_BITS)) == 0) {
            writeByte(value);
            return;
        }
        writeByte((value & SEGMENT_BITS) | CONTINUE_BIT);
        value >>= 7;
    }
}
unsigned long ByteBuffer::getVarLongSize(long value) {
    unsigned long size = 0;
    while (true) {
        if ((value & ~((long) SEGMENT_BITS)) == 0) {
            ++size;
            return size;
        }
        ++size;
        value >>= 7;
    }
}
long ByteBuffer::readVarLong() {
    long value = 0;
    int position = 0;
    char currentByte;
    while (true) {
        currentByte = readByte();
        value |= (long) (currentByte & SEGMENT_BITS) << position;
        if ((currentByte & CONTINUE_BIT) == 0) break;
        position += 7;
        if (position >= 64) throw std::runtime_error("VarLong is too big");
    }
    return value;
}

void ByteBuffer::writeString(std::string value) {
    writeVarInt(value.size());
    writeBytes(value.data(), value.size());
}
std::string ByteBuffer::readString() {
    unsigned long size = readVarInt();
    std::string value(size, '\0');
    readBytes(&value[0], size);
    return value;
}

void ByteBuffer::writeJSON(nlohmann::json value) {
    writeString(value.dump());
}
nlohmann::json ByteBuffer::readJSON() {
    return nlohmann::json::parse(readString());
}

void ByteBuffer::writePosition(int x, int y, int z) { writePosition({ x, y, z }); }
void ByteBuffer::writePosition(std::vector<int> position) {
    if (position.size() != 3) return;
    writeLong((((long)position[0] & 0x3FFFFFF) << 38) | (((long)position[2] & 0x3FFFFFF) << 12) | (position[1] & 0xFFF));
}
std::vector<int> ByteBuffer::readPosition() {
    std::vector<int> result = {0,0,0};
    long value = readLong();
    result[0] = value >> 38;
    result[1] = value << 52 >> 52;
    result[2] = value << 26 >> 38;
    if (result[0] >= (1 << 25)) result[0] -= (1 << 26);
    if (result[1] >= (1 << 11)) result[1] -= (1 << 12);
    if (result[2] >= (1 << 25)) result[2] -= (1 << 26);
    return result;
}

void ByteBuffer::writeBytes(const char* data, size_t size) {
    m_bytes.insert(m_bytes.end(), data, data + size);
}
void ByteBuffer::readBytes(char* data, size_t size) {
    if (m_readPtr + size > m_bytes.size()) {
        throw std::runtime_error("Attempted to read beyond buffer bounds");
    }
    std::copy(m_bytes.begin() + m_readPtr, m_bytes.begin() + m_readPtr + size, data);
    m_readPtr += size;
}

void ByteBuffer::writeUUID(uuids::uuid uuid) {
    writeBytes((const char*) uuid.as_bytes().data(), 16);
}
uuids::uuid ByteBuffer::readUUID() {
    std::array<uuids::uuid::value_type, 16> bytes;
    readBytes((char*) bytes.data(), 16);
    return uuids::uuid(bytes);
}

void ByteBuffer::writeByteArray(std::vector<char> value) {
    writeBytes(value.data(), value.size());
}
std::vector<char> ByteBuffer::readByteArray(size_t size) {
    std::vector<char> result(size, 0);
    readBytes(&result[0], size);
    return result;
}

void ByteBuffer::writePrefixedByteArray(std::vector<char> value) {
    writeVarInt(value.size());
    writeByteArray(value);
}
std::vector<char> ByteBuffer::readPrefixedByteArray() {
    return readByteArray(readVarInt());
}

void ByteBuffer::writeBitSet(BitSet bitset) {
    writePrefixedArray(bitset.toLongArray(), &ByteBuffer::writeLong);
}
BitSet ByteBuffer::readBitSet() {
    return BitSet(readPrefixedArray(&ByteBuffer::readLong));
}

void ByteBuffer::writeFixedBitSet(BitSet bitset) {
    writeArray(bitset.toLongArray(), &ByteBuffer::writeLong);
}
BitSet ByteBuffer::readFixedBitSet(size_t sizeInBytes) {
    return BitSet(readArray(sizeInBytes / 8, &ByteBuffer::readLong));
}

void ByteBuffer::writeIDSet(IDSet value) {
    writeVarInt(value.m_type);
    writeString(value.m_tagName);
    if (value.m_type != 0) writeArray(value.m_ids, &ByteBuffer::writeVarInt);
}
IDSet ByteBuffer::readIDSet() {
    IDSet result;
    result.m_type = readVarInt();
    result.m_tagName = readString();
    if (result.m_type != 0) result.m_ids = readArray(result.m_type - 1, &ByteBuffer::readVarInt);
    return result;
}

void ByteBuffer::writeSoundEvent(SoundEvent value) {
    writeString(value.m_soundName);
    writeBoolean(value.m_hasFixedRange);
    if (value.m_hasFixedRange) writeFloat(value.m_fixedRange);
}
SoundEvent ByteBuffer::readSoundEvent() {
    SoundEvent result;
    result.m_soundName = readString();
    std::optional<float> fixedRangeOptional = readPrefixedOptional(&ByteBuffer::readFloat);
    result.m_hasFixedRange = fixedRangeOptional.has_value();
    if (result.m_hasFixedRange) result.m_fixedRange = fixedRangeOptional.value();
    return result;
}

void ByteBuffer::writeTeleportFlags(TeleportFlags flags) {
    writeInt(flags.toInt());
}
TeleportFlags ByteBuffer::readTeleportFlags() {
    return TeleportFlags(readInt());
}

ByteBuffer::NetworkNBTTag::NetworkNBTTag() {}
ByteBuffer::NetworkNBTTag::NetworkNBTTag(ByteBuffer *buffer, NetworkNBTTagType expectedType, bool expectTagName, bool isTypeKnown, bool isInList) {
    if (!isTypeKnown) m_type = (NetworkNBTTagType) buffer->readByte(); 
    else m_type = expectedType;
    if (expectTagName && expectedType != NetworkNBTTagType::End) {
        short size = buffer->readShort();
        m_tagName = std::string(size, '\0');
        buffer->readBytes(m_tagName.data(), size);
    }
    switch (m_type) {
    case NetworkNBTTagType::Byte: m_byteValue = buffer->readByte(); break;
    case NetworkNBTTagType::Short: m_shortValue = buffer->readShort(); break;
    case NetworkNBTTagType::Int: m_intValue = buffer->readInt(); break;
    case NetworkNBTTagType::Long: m_longValue = buffer->readLong(); break;
    case NetworkNBTTagType::Float: m_floatValue = buffer->readFloat(); break;
    case NetworkNBTTagType::Double: m_doubleValue = buffer->readDouble(); break;
    case NetworkNBTTagType::ByteArray: m_byteArrayValue = buffer->readByteArray(buffer->readInt()); break;
    case NetworkNBTTagType::String: {
        short size = buffer->readShort();
        m_stringValue = std::string(size, '\0');
        buffer->readBytes(m_stringValue.data(), size);
        break;
    }
    case NetworkNBTTagType::List: {
        NetworkNBTTagType elementType = (NetworkNBTTagType) buffer->readByte();
        int size = buffer->readInt();
        for (int i = 0; i < size; i++) m_childrenTagsValue.push_back(NetworkNBTTag(buffer, elementType, false, true, true));
        break;
    }
    case NetworkNBTTagType::Compound: {
        while (true) {
            NetworkNBTTagType elementType = (NetworkNBTTagType) buffer->readByte();
            if (!isInList) {
                if (elementType == NetworkNBTTagType::End) break;
            } else if (elementType == NetworkNBTTagType::End || elementType == NetworkNBTTagType::Compound) break;
            m_childrenTagsValue.push_back(NetworkNBTTag(buffer, elementType, true, true));
        }
        break;
    }
    case NetworkNBTTagType::IntArray: m_intArrayValue = buffer->readArray<int>(buffer->readInt(), &ByteBuffer::readInt); break;
    case NetworkNBTTagType::LongArray: m_longArrayValue = buffer->readArray<long>(buffer->readInt(), &ByteBuffer::readLong); break;
    default: break;
    }
}
std::vector<char> ByteBuffer::NetworkNBTTag::encode(bool ignoreNametag, bool isInList) {
    ByteBuffer result;
    if (!isInList) result.writeByte((char) m_type);
    if (!ignoreNametag && m_type != NetworkNBTTagType::End) {
        result.writeShort(m_tagName.size());
        result.writeByteArray(std::vector<char>(m_tagName.begin(), m_tagName.end()));
    }
    switch (m_type) {
    case NetworkNBTTagType::Byte: result.writeByte(m_byteValue); break;
    case NetworkNBTTagType::Short: result.writeShort(m_shortValue); break;
    case NetworkNBTTagType::Int: result.writeInt(m_intValue); break;
    case NetworkNBTTagType::Long: result.writeLong(m_longValue); break;
    case NetworkNBTTagType::Float: result.writeFloat(m_floatValue); break;
    case NetworkNBTTagType::Double: result.writeDouble(m_doubleValue); break;
    case NetworkNBTTagType::ByteArray: result.writeInt(m_byteArrayValue.size()); result.writeByteArray(m_byteArrayValue); break;
    case NetworkNBTTagType::String: result.writeShort(m_stringValue.size()); result.writeBytes(m_stringValue.data(), m_stringValue.size()); break;
    case NetworkNBTTagType::List: {
        if (m_childrenTagsValue.empty()) result.writeByte(0);
        else {
            NetworkNBTTagType type = m_childrenTagsValue[0].m_type;
            for (NetworkNBTTag& tag : m_childrenTagsValue) if (type != tag.m_type) {
                type = NetworkNBTTagType::End;
                break;
            }
            result.writeByte((char) type);
        }
        if (result.getBytes()[result.getBytes().size() - 1] != 0) {
            result.writeInt(m_childrenTagsValue.size());
            for (NetworkNBTTag& tag : m_childrenTagsValue) result.writeByteArray(tag.encode(true, true));
            if (m_childrenTagsValue[0].m_type == NetworkNBTTagType::Compound) result.writeByte(0);
        } else result.writeInt(0);
        break;
    }
    case NetworkNBTTagType::Compound: {
        for (NetworkNBTTag& tag : m_childrenTagsValue) result.writeByteArray(tag.encode());
        result.writeByte(0);
        break;
    }
    case NetworkNBTTagType::IntArray: result.writeInt(m_intArrayValue.size()); result.writeArray<int>(m_intArrayValue, &ByteBuffer::writeInt); break;
    case NetworkNBTTagType::LongArray: result.writeInt(m_longArrayValue.size()); result.writeArray<long>(m_longArrayValue, &ByteBuffer::writeLong); break;
    default: break;
    }
    return result.getBytes();
}
bool ByteBuffer::NetworkNBTTag::operator==(NetworkNBTTag& b) { return encode() == b.encode(); }
bool ByteBuffer::NetworkNBTTag::operator!=(NetworkNBTTag& b) { return encode() != b.encode(); }
ByteBuffer::NetworkNBTTag ByteBuffer::NetworkNBTTag::End() { return NetworkNBTTag(); }
ByteBuffer::NetworkNBTTag ByteBuffer::NetworkNBTTag::Byte(char value, std::string const& tagName) {
    NetworkNBTTag result;
    result.m_type = NetworkNBTTagType::Byte;
    result.m_tagName = tagName;
    result.m_byteValue = value;
    return result;
}
ByteBuffer::NetworkNBTTag ByteBuffer::NetworkNBTTag::Short(short value, std::string const& tagName) {
    NetworkNBTTag result;
    result.m_type = NetworkNBTTagType::Short;
    result.m_tagName = tagName;
    result.m_shortValue = value;
    return result;
}
ByteBuffer::NetworkNBTTag ByteBuffer::NetworkNBTTag::Int(int value, std::string const& tagName) {
    NetworkNBTTag result;
    result.m_type = NetworkNBTTagType::Int;
    result.m_tagName = tagName;
    result.m_intValue = value;
    return result;
}
ByteBuffer::NetworkNBTTag ByteBuffer::NetworkNBTTag::Long(long value, std::string const& tagName) {
    NetworkNBTTag result;
    result.m_type = NetworkNBTTagType::Long;
    result.m_tagName = tagName;
    result.m_longValue = value;
    return result;
}
ByteBuffer::NetworkNBTTag ByteBuffer::NetworkNBTTag::Float(float value, std::string const& tagName) {
    NetworkNBTTag result;
    result.m_type = NetworkNBTTagType::Float;
    result.m_tagName = tagName;
    result.m_floatValue = value;
    return result;
}
ByteBuffer::NetworkNBTTag ByteBuffer::NetworkNBTTag::Double(double value, std::string const& tagName) {
    NetworkNBTTag result;
    result.m_type = NetworkNBTTagType::Double;
    result.m_tagName = tagName;
    result.m_doubleValue = value;
    return result;
}
ByteBuffer::NetworkNBTTag ByteBuffer::NetworkNBTTag::ByteArray(std::vector<char> const& value, std::string const& tagName) {
    NetworkNBTTag result;
    result.m_type = NetworkNBTTagType::ByteArray;
    result.m_tagName = tagName;
    result.m_byteArrayValue = value;
    return result;
}
ByteBuffer::NetworkNBTTag ByteBuffer::NetworkNBTTag::String(std::string const& value, std::string const& tagName) {
    NetworkNBTTag result;
    result.m_type = NetworkNBTTagType::String;
    result.m_tagName = tagName;
    result.m_stringValue = value;
    return result;
}
ByteBuffer::NetworkNBTTag ByteBuffer::NetworkNBTTag::IntArray(std::vector<int> const& value, std::string const& tagName) {
    NetworkNBTTag result;
    result.m_type = NetworkNBTTagType::IntArray;
    result.m_tagName = tagName;
    result.m_intArrayValue = value;
    return result;
}
ByteBuffer::NetworkNBTTag ByteBuffer::NetworkNBTTag::LongArray(std::vector<long> const& value, std::string const& tagName) {
    NetworkNBTTag result;
    result.m_type = NetworkNBTTagType::LongArray;
    result.m_tagName = tagName;
    result.m_longArrayValue = value;
    return result;
}
ByteBuffer::NetworkNBTTag ByteBuffer::NetworkNBTTag::List(std::vector<NetworkNBTTag> const& value, std::string const& tagName) {
    NetworkNBTTag result;
    result.m_type = NetworkNBTTagType::List;
    result.m_tagName = tagName;
    result.m_childrenTagsValue = value;
    return result;
}
ByteBuffer::NetworkNBTTag ByteBuffer::NetworkNBTTag::Compound(std::vector<NetworkNBTTag> const& value, std::string const& tagName) {
    NetworkNBTTag result;
    result.m_type = NetworkNBTTagType::Compound;
    result.m_tagName = tagName;
    result.m_childrenTagsValue = value;
    return result;
}
void ByteBuffer::writeNBT(NetworkNBTTag nbt, int protocol) {
    writeByteArray(nbt.encode(protocol >= 764));
}
ByteBuffer::NetworkNBTTag ByteBuffer::readNBT(int protocol) {
    return NetworkNBTTag(this, NetworkNBTTagType::Compound, protocol < 764);
}

}
