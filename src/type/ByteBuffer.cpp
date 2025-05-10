#include <type/ByteBuffer.h>
#include <stdexcept>

namespace zinc {

std::vector<char>& ByteBuffer::getBytes() {
    return m_bytes;
}
std::vector<char> ByteBuffer::getBytes() const {
    return m_bytes;
}
size_t& ByteBuffer::getReadPointer() {
    return m_readPtr;
}
size_t ByteBuffer::getReadPointer() const {
    return m_readPtr;
}
bool& ByteBuffer::getIsBigEndian() {
    return m_isBigEndian;
}
bool ByteBuffer::getIsBigEndian() const {
    return m_isBigEndian;
}
void ByteBuffer::setReadPointer(size_t readPtr) {
    m_readPtr = readPtr;
    if (readPtr >= m_bytes.size()) m_readPtr = 0;
}
void ByteBuffer::setBytes(const std::vector<char>& bytes) {
    m_bytes = bytes;
    m_readPtr = 0;
}
void ByteBuffer::setIsBigEndian(const bool& isBigEndian) {
    m_isBigEndian = isBigEndian;
}
void ByteBuffer::clear() {
    m_bytes.clear();
}
char *ByteBuffer::data() {
    return m_bytes.data();
}
const char *ByteBuffer::data() const {
    return (const char*) m_bytes.data();
}
size_t ByteBuffer::size() const {
    return m_bytes.size();
}

void ByteBuffer::writeBytes(const std::vector<char>& bytes) {
    m_mutex.lock();
    m_bytes.insert(m_bytes.end(), bytes.begin(), bytes.end());
    m_mutex.unlock();
}
std::vector<char> ByteBuffer::readBytes(const size_t& length) {
    if ((m_readPtr + length) > m_bytes.size()) {
        m_logger.error(std::out_of_range("Not enough bytes to read").what());
        return std::vector<char>(length, 0);
    }
    std::vector<char> result (length);
    std::copy(m_bytes.begin() + m_readPtr, m_bytes.begin() + m_readPtr + length, result.begin());
    m_mutex.lock();
    m_readPtr += length;
    m_mutex.unlock();
    return result;
}

void ByteBuffer::writeByte(const char& c) {
    m_mutex.lock();
    m_bytes.push_back(c);
    m_mutex.unlock();
}
char ByteBuffer::readByte() {
    char result = m_bytes[m_readPtr];
    m_mutex.lock();
    m_readPtr++;
    m_mutex.unlock();
    return result;
}
void ByteBuffer::writeUnsignedByte(const unsigned char& c) {
    m_mutex.lock();
    m_bytes.push_back((const char&)c);
    m_mutex.unlock();
}
unsigned char ByteBuffer::readUnsignedByte() {
    unsigned char result = (unsigned char) m_bytes[m_readPtr];
    m_mutex.lock();
    m_readPtr++;
    m_mutex.unlock();
    return result;
}

size_t ByteBuffer::getVarIntLength(const int& value) {
    int l_value = value;
    size_t size = 0;
    while (true) {
        if (!(l_value & ~SEGMENT_BITS)) {
            size++;
            return size;
        }
        size++;
        l_value >>= 7;
    }
    return size;
}
size_t ByteBuffer::getVarLongLength(const long& value) {
    int l_value = value;
    size_t size = 0;
    while (true) {
        if (!(l_value & ~SEGMENT_BITS)) {
            size++;
            return size;
        }
        size++;
        l_value >>= 7;
    }
    return size;
}
void ByteBuffer::writeVarInt(const int& value) {
    int l_value = value;
    while (true) {
        if (!(l_value & ~SEGMENT_BITS)) {
            writeByte(l_value);
            return;
        }
        writeByte((l_value & SEGMENT_BITS) | CONTINUE_BIT);
        l_value >>= 7;
    }
}
void ByteBuffer::writeVarLong(const long& value) {
    int l_value = value;
    while (true) {
        if (!(l_value & ~SEGMENT_BITS)) {
            writeByte(l_value);
            return;
        }
        writeByte((l_value & SEGMENT_BITS) | CONTINUE_BIT);
        l_value >>= 7;
    }
}
int ByteBuffer::readVarInt() {
    int value = 0;
    int position = 0;
    char c;
    while (true) {
        c = readByte();
        value |= (c & SEGMENT_BITS) << position;
        if (!(c & CONTINUE_BIT)) break;
        position += 7;
        if (position >= 32) {
            m_logger.error("VarInt is too big");
            return 0;
        }
    }
    size_t size = getVarIntLength(value);
    m_mutex.lock();
    m_readPtr += size;
    m_mutex.unlock();
    if (size > 5) {
        m_logger.error("VarInt is too big");
        return 0;
    }
    return value;
}
long ByteBuffer::readVarLong() {
    int value = 0;
    int position = 0;
    char c;
    while (true) {
        c = readByte();
        value |= (c & SEGMENT_BITS) << position;
        if (!(c & CONTINUE_BIT)) break;
        position += 7;
        if (position >= 64) {
            m_logger.error("VarLong is too big");
            return 0;
        }
    }
    size_t size = getVarLongLength(value);
    m_mutex.lock();
    m_readPtr += size;
    m_mutex.unlock();
    if (size > 10) {
        m_logger.error("VarLong is too big");
        return 0;
    }
    return value;
}

size_t ByteBuffer::getZigZagVarIntLength(const int& value) {
    return getVarIntLength((value << 1) ^ (value >> 31));
}
size_t ByteBuffer::getZigZagVarLongLength(const long& value) {
    return getVarLongLength((value << 1) ^ (value >> 63));
}
void ByteBuffer::writeZigZagVarInt(const int& value) {
    writeVarInt((value << 1) ^ (value >> 31));
}
void ByteBuffer::writeZigZagVarLong(const long& value) {
    writeVarLong((value << 1) ^ (value >> 63));
}
int ByteBuffer::readZigZagVarInt() {
    int value = readVarInt();
    return (value >> 1) ^ -(value & 1);
}
long ByteBuffer::readZigZagVarLong() {
    long value = readVarLong();
    return (value >> 1) ^ -(value & 1);
}

void ByteBuffer::writeString(const std::string& value) {
    writeVarInt(value.size());
    writeBytes(std::vector<char>(value.begin(), value.end()));
}
std::string ByteBuffer::readString() {
    std::vector<char> bytes = readBytes(readVarInt());
    return std::string(bytes.begin(), bytes.end());
}
void ByteBuffer::writeIdentifier(const Identifier& value) {
    writeString(value.toString());
}
Identifier ByteBuffer::readIdentifier() {
    return Identifier(readString());
}

void ByteBuffer::writePosition(const Vector3i& value) {
    writeNumeric<long>((((long) value.getX() & 0x3FFFFFF) << 38) | ((value.getZ() & 0x3FFFFFF) << 12) | (value.getY() & 0xFFF));
}
Vector3i ByteBuffer::readPosition() {
    long longVal = readNumeric<long>();
    int x = int(longVal >> 38);
    int z = int((longVal >> 12) & 0x3FFFFFF);
    int y = int(longVal & 0xFFF);
    return Vector3i(x, y, z);
}

void ByteBuffer::writeAngle(const float& angle) {
    writeUnsignedByte((unsigned char)(angle * 256));
}
float ByteBuffer::readAngle() {
    return (float)readUnsignedByte() / 256;
}

void ByteBuffer::writeUUID(const uuids::uuid& uuid) {
    m_mutex.lock();
    for (const std::byte& c : uuid.as_bytes()) writeNumeric<std::byte>(c);
    m_mutex.unlock();
}
uuids::uuid ByteBuffer::readUUID() {
    std::vector<char> bytes = readBytes(16);
    std::array<unsigned char, 16> bytesArr;
    std::copy(bytes.begin(), bytes.end(), bytesArr.begin());
    return uuids::uuid(bytesArr);
}

void ByteBuffer::writeByteArray(const std::vector<char>& bytes) {
    writeBytes(bytes);
}
void ByteBuffer::writePrefixedByteArray(const std::vector<char>& bytes) {
    writeVarInt(bytes.size());
    writeBytes(bytes);
}
std::vector<char> ByteBuffer::readByteArray(const size_t& length) {
    return readBytes(length);
}
std::vector<char> ByteBuffer::readPrefixedByteArray() {
    return readBytes(readVarInt());
}

void ByteBuffer::writeIDSet(const IDSet& idSet) {
    writeVarInt(idSet.getType());
    if (!idSet.getType()) writeIdentifier(idSet.getIdentifier());
    else writeArray<int>(idSet.getIDs(), &ByteBuffer::writeVarInt);
}
IDSet ByteBuffer::readIDSet() {
    IDSet result;
    result.setType(readVarInt());
    if (result.getType()) result.setIDs(readArray<int>(&ByteBuffer::readVarInt, result.getType() - 1));
    else result.setIdentifier(readIdentifier());
    return result;
}

void ByteBuffer::writeVector3(const Vector3f& vector) {
    writeNumeric<float>(vector.getX());
    writeNumeric<float>(vector.getY());
    writeNumeric<float>(vector.getZ());
}
Vector3f ByteBuffer::readVector3() {
    return Vector3f(readNumeric<float>(), readNumeric<float>(), readNumeric<float>());
}
void ByteBuffer::writeVector2(const Vector2f& vector) {
    writeNumeric<float>(vector.getX());
    writeNumeric<float>(vector.getY());
}
Vector2f ByteBuffer::readVector2() {
    return Vector2f(readNumeric<float>(), readNumeric<float>());
}
void ByteBuffer::writeBlockCoordinates(const Vector3i& blockCoordinates) {
    writeVarInt(blockCoordinates.getX());
    writeVarInt(blockCoordinates.getY());
    writeVarInt(blockCoordinates.getZ());
}
Vector3i ByteBuffer::readBlockCoordinates() {
    return Vector3i(readVarInt(), readVarInt(), readVarInt());
}

void ByteBuffer::writePlayerLocation(const PlayerLocation& playerLocation) {
    writeVector3(playerLocation.getPosition());
    writeByte((unsigned char)(playerLocation.getPitch() * 0.71));
    writeByte((unsigned char)(playerLocation.getHeadYaw() * 0.71));
    writeByte((unsigned char)(playerLocation.getYaw() * 0.71));
}
PlayerLocation ByteBuffer::readPlayerLocation() {
    return PlayerLocation(readVector3(), (float)(readByte()) / 0.71, (float)(readByte()) / 0.71, (float)(readByte()) / 0.71);
}

void ByteBuffer::writeSoundEvent(const SoundEvent& soundEvent) {
    writeIdentifier(soundEvent.getIdentifier());
    writeByte(soundEvent.hasFixedRange());
    writeNumeric<float>(soundEvent.getFixedRange());
}
SoundEvent ByteBuffer::readSoundEvent() {
    SoundEvent result (readIdentifier());
    if (readByte()) result.setFixedRange(readNumeric<float>());
    return result;
}

void ByteBuffer::writeBitSet(const BitSet& bitSet) {
    writePrefixedArray<unsigned long>(bitSet.toLongArray(), &ByteBuffer::writeNumeric<unsigned long>);
}
BitSet ByteBuffer::readBitSet() {
    return BitSet::fromLongArray(readPrefixedArray<unsigned long>(&ByteBuffer::readNumeric<unsigned long>));
}
void ByteBuffer::writeFixedBitSet(const BitSet& bitSet) {
    writeArray<unsigned char>(bitSet.toByteArray(), &ByteBuffer::writeUnsignedByte);
}
BitSet ByteBuffer::readFixedBitSet(const size_t& length) {
    return BitSet::fromByteArray(readArray<unsigned char>(&ByteBuffer::readUnsignedByte, length));
}

void ByteBuffer::writeTeleportFlags(const TeleportFlags& teleportFlags) {
    writeNumeric<int>(teleportFlags.encode());
}
TeleportFlags ByteBuffer::readTeleportFlags() {
    return TeleportFlags(readNumeric<int>());
}

void ByteBuffer::writeNBTElement(const NBTElement& nbtElement, const int& protocol) {
    NBTSettings settings;
    settings.setIsInArray(false);
    settings.setIsNetwork(true);
    settings.setProtocol(protocol);
    settings.setType(NBTElementType::End);
    writeNBTElement(nbtElement, settings);
}
void ByteBuffer::writeNBTElement(const NBTElement& nbtElement, const NBTSettings& settings) {
    NBTElement element = nbtElement;
    element.setSettings(settings);
    element.encode(*this);
}
NBTElement ByteBuffer::readNBTElement(const int& protocol) {
    NBTSettings settings;
    settings.setIsInArray(false);
    settings.setIsNetwork(true);
    settings.setProtocol(protocol);
    settings.setType(NBTElementType::End);
    return readNBTElement(settings);
}
NBTElement ByteBuffer::readNBTElement(const NBTSettings& settings) {
    NBTElement element;
    element.setSettings(settings);
    element.decode(*this);
    return element;
}

}