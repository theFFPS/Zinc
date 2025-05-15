#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <mutex>
#include <optional>

#include <util/Logger.h>
#include <external/UUID.h>
#include "Identifier.h"
#include "Vector.h"
#include "IDorX.h"
#include "IDSet.h"
#include "TextComponent.h"
#include "PlayerLocation.h"
#include "SoundEvent.h"
#include "BitSet.h"
#include "TeleportFlags.h"
#include "TextComponent.h"
#include "ChatType.h"
#include "Slot.h"
#include "nbt/NBTElement.h"

namespace zinc {

struct ByteBuffer {
private:
    std::vector<char> m_bytes;
    size_t m_readPtr;

    Logger m_logger = Logger("ByteBuffer");
    std::mutex m_mutex;

    constexpr static const int SEGMENT_BITS = 0x7F;
    constexpr static const int CONTINUE_BIT = 0x80;

    bool m_isBigEndian;
public:
    ByteBuffer() : m_bytes({}), m_readPtr(0), m_isBigEndian(true) {}
    ByteBuffer(const bool& isBigEndian) : m_bytes({}), m_readPtr(0), m_isBigEndian(isBigEndian) {}
    ByteBuffer(const std::vector<char>& bytes, const size_t& readPtr = 0) : m_bytes(bytes), m_readPtr(readPtr), m_isBigEndian(true) {}
    ByteBuffer(const std::vector<char>& bytes, const size_t& readPtr, const bool& isBigEndian) : m_bytes(bytes), m_readPtr(readPtr), m_isBigEndian(isBigEndian) {}
    ByteBuffer(ByteBuffer&& buffer) : m_bytes(buffer.getBytes()), m_readPtr(buffer.getReadPointer()), m_isBigEndian(buffer.getIsBigEndian()) {}

    std::vector<char>& getBytes();
    std::vector<char> getBytes() const;
    size_t& getReadPointer();
    size_t getReadPointer() const;
    bool& getIsBigEndian();
    bool getIsBigEndian() const;
    void setReadPointer(size_t readPtr);
    void setBytes(const std::vector<char>& bytes);
    void setIsBigEndian(const bool& isBigEndian);
    void clear();
    char *data();
    const char *data() const;
    size_t size() const;

    void writeBytes(const std::vector<char>& bytes);
    std::vector<char> readBytes(const size_t& length);

    template<typename T> void writeNumeric(const T& value) {
        std::vector<char> bytes (sizeof(T));
        std::copy((char*)&value, ((char*)&value) + sizeof(T), bytes.begin());
        if (m_isBigEndian) std::reverse(bytes.begin(), bytes.end());
        writeBytes(bytes);
    }
    template<typename T> T readNumeric() {
        T result;
        std::vector<char> bytes = readBytes(sizeof(T));
        if (m_isBigEndian) std::reverse(bytes.begin(), bytes.end());
        std::copy(bytes.begin(), bytes.end(), (char*)&result);
        return result;
    }

    void writeByte(const char& c);
    char readByte();
    void writeUnsignedByte(const unsigned char& c);
    unsigned char readUnsignedByte();

    size_t getVarIntLength(const int& value);
    size_t getVarLongLength(const long& value);
    void writeVarInt(const int& value);
    void writeVarLong(const long& value);
    int readVarInt();
    long readVarLong();

    size_t getZigZagVarIntLength(const int& value);
    size_t getZigZagVarLongLength(const long& value);
    void writeZigZagVarInt(const int& value);
    void writeZigZagVarLong(const long& value);
    int readZigZagVarInt();
    long readZigZagVarLong();

    void writeString(const std::string& value);
    std::string readString();

    void writeIdentifier(const Identifier& value);
    Identifier readIdentifier();

    void writePosition(const Vector3i& value);
    Vector3i readPosition();

    void writeAngle(const float& angle);
    float readAngle();

    void writeUUID(const uuids::uuid& uuid);
    uuids::uuid readUUID();

    template<typename T> void writeOptional(const std::optional<T>& value, void(ByteBuffer::*func)(const T&)) {
        if (value.has_value()) (this->*func)(value.value());
    }
    template<typename T> void writeOptional(const std::optional<T>& value, void(*func)(const T&, ByteBuffer&)) {
        if (value.has_value()) func(value.value(), *this);
    }
    template<typename T> void writePrefixedOptional(const std::optional<T>& value, void(ByteBuffer::*func)(const T&)) {
        writeByte(value.has_value());
        if (value.has_value()) (this->*func)(value.value());
    }
    template<typename T> void writePrefixedOptional(const std::optional<T>& value, void(*func)(const T&, ByteBuffer&)) {
        writewriteByte(value.has_value());
        if (value.has_value()) func(value.value(), *this);
    }
    template<typename T> std::optional<T> readOptional(T(ByteBuffer::*func)(), bool hasValue) {
        std::optional<T> result;
        if (hasValue) result = (this->*func)();
        return result;
    }
    template<typename T> std::optional<T> readOptional(T(func)(ByteBuffer&), bool hasValue) {
        std::optional<T> result;
        if (hasValue) result = func(*this);
        return result;
    }
    template<typename T> std::optional<T> readPrefixedOptional(T(ByteBuffer::*func)()) {
        return readOptional<T>(func, readByte());
    }
    template<typename T> std::optional<T> readPrefixedOptional(T(func)(ByteBuffer&)) {
        return readOptional<T>(func, readByte());
    }

    template<typename T> void writeArray(const std::vector<T>& value, void(ByteBuffer::*func)(const T&)) {
        for (const T& element : value) (this->*func)(element);
    }
    template<typename T> void writeArray(const std::vector<T>& value, void(*func)(const T&, ByteBuffer&)) {
        for (const T& element : value) func(element, *this);
    }
    template<typename T> void writePrefixedArray(const std::vector<T>& value, void(ByteBuffer::*func)(const T&)) {
        writeVarInt(value.size());
        writeArray(value, func);
    }
    template<typename T> void writePrefixedArray(const std::vector<T>& value, void(*func)(const T&, ByteBuffer&)) {
        writeVarInt(value.size());
        writeArray(value, func);
    }
    template<typename T> std::vector<T> readArray(T(ByteBuffer::*func)(), size_t length) {
        std::vector<T> result;
        for (size_t i = 0; i < length; i++) result.push_back((this->*func)());
        return result;
    }
    template<typename T> std::vector<T> readArray(T(func)(ByteBuffer&), size_t length) {
        std::vector<T> result;
        for (size_t i = 0; i < length; i++) result.push_back(func(*this));
        return result;
    }
    template<typename T> std::vector<T> readPrefixedArray(T(ByteBuffer::*func)()) {
        return readArray(func, readVarInt());
    }
    template<typename T> std::vector<T> readPrefixedArray(T(func)(ByteBuffer&)) {
        return readArray(func, readVarInt());
    }

    void writeByteArray(const std::vector<char>& bytes);
    void writePrefixedByteArray(const std::vector<char>& bytes);
    std::vector<char> readByteArray(const size_t& length);
    std::vector<char> readPrefixedByteArray();

    template<typename T> void writeIDorX(const IDorX<T>& value, void(*func)(const T&, ByteBuffer&)) {
        writeVarInt(value.getId());
        if (!value.getId()) func(value.getValue());
    }
    template<typename T> void writeIDorX(const IDorX<T>& value, void(ByteBuffer::*func)(const T&)) {
        writeVarInt(value.getId());
        if (!value.getId()) (this->*func)(value.getValue());
    }
    template<typename T> IDorX<T> readIDorX(T(func)(ByteBuffer&)) {
        IDorX<T> result;
        result.setId(readVarInt());
        if (!result.getId()) result.setValue(func(*this));
        return result;
    }
    template<typename T> IDorX<T> readIDorX(T(ByteBuffer::*func)()) {
        IDorX<T> result;
        result.setId(readVarInt());
        if (!result.getId()) result.setValue((this->*func)());
        return result;
    }

    void writeIDSet(const IDSet& idSet);
    IDSet readIDSet();

    void writeVector3(const Vector3f& vector);
    Vector3f readVector3();
    void writeVector2(const Vector2f& vector);
    Vector2f readVector2();
    void writeBlockCoordinates(const Vector3i& blockCoordinates);
    Vector3i readBlockCoordinates();

    void writePlayerLocation(const PlayerLocation& playerLocation);
    PlayerLocation readPlayerLocation();

    void writeSoundEvent(const SoundEvent& soundEvent);
    SoundEvent readSoundEvent();

    void writeBitSet(const BitSet& bitSet);
    BitSet readBitSet();
    void writeFixedBitSet(const BitSet& bitSet);
    BitSet readFixedBitSet(const size_t& length);

    template<typename T> void writeEnumSet(const std::vector<T>& enumSet) {
        BitSet bitSet;
        for (const T& value : enumSet) {
            bitSet.set((unsigned long) value);
        }
        writeFixedBitSet(bitSet);
    }
    template<typename T> std::vector<T> readEnumSet(const size_t& length) {
        BitSet bitSet = readFixedBitSet(length);
        std::vector<T> result;
        for (size_t i = 0; i < (length * 8); i++) {
            if (bitSet.get(i)) result.push_back((T) i);
        }
        return result;
    }

    void writeTeleportFlags(const TeleportFlags& teleportFlags);
    TeleportFlags readTeleportFlags();

    void writeNBTElement(const NBTElement& nbtElement);
    NBTElement readNBTElement();

    void writeTextComponent(const TextComponent& textComponent);
    TextComponent readTextComponent();

    void writeChatType(const ChatType& chatType);
    ChatType readChatType();

    void writeSlot(const Slot& slot);
    void writeHashedSlot(const Slot& slot);

    bool operator==(const ByteBuffer& buffer) const;
    bool operator!=(const ByteBuffer& buffer) const;
};

}