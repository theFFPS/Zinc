#include <type/ByteBuffer.h>

namespace zinc {

ByteBuffer::ByteBuffer() : m_bytes({}), m_readPtr(0) {}
ByteBuffer::ByteBuffer(const std::vector<char>& bytes, unsigned long readPtr) : m_bytes(bytes), m_readPtr(readPtr) {}

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
    std::vector<char> result(0, size);
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

}
