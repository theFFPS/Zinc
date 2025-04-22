#pragma once

#include <vector>
#include <string>
#include <cstring>
#include <stdexcept>
#include <optional>

#include <external/JSON.h>
#include <external/UUID.h>

#include "gametypes/LightData.h"
#include "gametypes/SoundEvent.h"
#include "gametypes/TeleportFlags.h"
#include "registry/IDorX.h"
#include "registry/IDSet.h"
#include "BitSet.h"

namespace zinc {

struct ByteBuffer {
private:
    std::vector<char> m_bytes;
    unsigned long m_readPtr;

    static const int SEGMENT_BITS = 0x7F;
    static const int CONTINUE_BIT = 0x80;

public:
    ByteBuffer();
    ByteBuffer(const std::vector<char>& bytes, unsigned long readPtr = 0);

    std::vector<char> getBytes();

    template<typename T> void writeNumeric(T value) {
        char tmp[sizeof(T)];
        memcpy(tmp, &value, sizeof(T));
        writeBytes(tmp, sizeof(T));
    }
    template<typename T> T readNumeric() {
        char tmp[sizeof(T)];
        T value;
        readBytes(tmp, sizeof(T));
        memcpy(&value, tmp, sizeof(T));
        return value;
    }

    void writeBoolean(bool value);
    bool readBoolean();

    void writeByte(char value);
    char readByte();
    void writeUnsignedByte(unsigned char value);
    unsigned char readUnsignedByte();

    void writeShort(short value);
    short readShort();
    void writeUnsignedShort(unsigned short value);
    unsigned short readUnsignedShort();

    void writeInt(int value);
    int readInt();
    void writeUnsignedInt(unsigned int value);
    unsigned int readUnsignedInt();

    void writeLong(long value);
    long readLong();
    void writeUnsignedLong(unsigned long value);
    unsigned long readUnsignedLong();

    void writeFloat(float value);
    float readFloat();

    void writeDouble(double value);
    double readDouble();

    void writeVarInt(int value);
    unsigned long getVarIntSize(int value);
    int readVarInt();

    void writeVarLong(long value);
    unsigned long getVarLongSize(long value);
    long readVarLong();

    void writeString(std::string value);
    std::string readString();

    void writeJSON(nlohmann::json value);
    nlohmann::json readJSON();

    void writePosition(int x, int y, int z);
    void writePosition(std::vector<int> position);
    std::vector<int> readPosition();

    void writeBytes(const char* data, size_t size);
    void readBytes(char* data, size_t size);

    void writeUUID(uuids::uuid uuid);
    uuids::uuid readUUID();

    template<typename T> void writeArray(std::vector<T> values, void(ByteBuffer::*writeFunc)(T)) {
        for (T& value : values) (this->*writeFunc)(value);
    }
    template<typename T> void writeArray(std::vector<T> values, void(*writeFunc)(T)) {
        for (T& value : values) writeFunc(value);
    }
    template<typename T> std::vector<T> readArray(size_t size, T(ByteBuffer::*readFunc)()) {
        std::vector<T> result;
        for (size_t i = 0; i < size; i++) result.push_back((this->*readFunc)());
        return result;
    }
    template<typename T> std::vector<T> readArray(size_t size, T(*readFunc)()) {
        std::vector<T> result;
        for (size_t i = 0; i < size; i++) result.push_back(readFunc());
        return result;
    }
    template<typename T> void writePrefixedArray(std::vector<T> values, void(ByteBuffer::*writeFunc)(T)) {
        writeVarInt(values.size());
        writeArray(values, writeFunc);
    }
    template<typename T> void writePrefixedArray(std::vector<T> values, void(*writeFunc)(T)) {
        writeVarInt(values.size());
        writeArray(values, writeFunc);
    }
    template<typename T> std::vector<T> readPrefixedArray(T(ByteBuffer::*readFunc)()) {
        return readArray(readVarInt(), readFunc);
    }
    template<typename T> std::vector<T> readPrefixedArray(T(*readFunc)()) {
        return readArray(readVarInt(), readFunc);
    }

    template<typename T> void writeOptional(std::optional<T> value, void(ByteBuffer::*writeFunc)(T)) {
        if (value.has_value()) (this->*writeFunc)(value.value());
    }
    template<typename T> void writeOptional(std::optional<T> value, void(*writeFunc)(T)) {
        if (value.has_value()) writeFunc(value.value());
    }
    template<typename T> std::optional<T> readOptional(bool shouldRead, T(ByteBuffer::*readFunc)()) {
        if (shouldRead) return (this->*readFunc)();
        return std::optional<T>();
    }
    template<typename T> std::optional<T> readOptional(bool shouldRead, T(*readFunc)()) {
        if (shouldRead) return readFunc();
        return std::optional<T>();
    }
    template<typename T> void writePrefixedOptional(std::optional<T> value, void(ByteBuffer::*writeFunc)(T)) {
        writeBoolean(value.has_value());
        if (value.has_value()) (this->*writeFunc)(value.value());
    }
    template<typename T> void writePrefixedOptional(std::optional<T> value, void(*writeFunc)(T)) {
        writeBoolean(value.has_value());
        if (value.has_value()) writeFunc(value.value());
    }
    template<typename T> std::optional<T> readPrefixedOptional(T(ByteBuffer::*readFunc)()) {
        if (readBoolean()) return (this->*readFunc)();
        return std::optional<T>();
    }
    template<typename T> std::optional<T> readPrefixedOptional(T(*readFunc)()) {
        if (readBoolean()) return readFunc();
        return std::optional<T>();
    }

    template<typename T> void writeEnum(T value) { writeNumeric<T>(value); }
    template<typename T> T readEnum() { return readNumeric<T>(); }

    void writeByteArray(std::vector<char> value);
    std::vector<char> readByteArray(size_t size);

    void writePrefixedByteArray(std::vector<char> value);
    std::vector<char> readPrefixedByteArray();

    void writeBitSet(BitSet bitset);
    BitSet readBitSet();

    void writeFixedBitSet(BitSet bitset);
    BitSet readFixedBitSet(size_t sizeInBytes);

    template<typename T> void writeIDorX(IDorX<T> value, void(ByteBuffer::*writeFunc)(T)) {
        writeVarInt(value.m_id);
        if (value.m_id == 0) (this->*writeFunc)(value.m_value.value());
    }
    template<typename T> void writeIDorX(IDorX<T> value, void(*writeFunc)(T)) {
        writeVarInt(value.m_id);
        if (value.m_id == 0) writeFunc(value.m_value.value());
    }
    template<typename T> IDorX<T> readIDorX(T(ByteBuffer::*readFunc)()) {
        int id = readVarInt();
        if (id != 0) return IDorX<T>(id);
        return IDorX<T>(std::optional<T>((this->*readFunc)()));
    }
    template<typename T> IDorX<T> readIDorX(T(*readFunc)()) {
        int id = readVarInt();
        if (id != 0) return IDorX<T>(id);
        return IDorX<T>(std::optional<T>(readFunc()));
    }

    void writeIDSet(IDSet value);
    IDSet readIDSet();

    void writeSoundEvent(SoundEvent value);
    SoundEvent readSoundEvent();

    void writeTeleportFlags(TeleportFlags flags);
    TeleportFlags readTeleportFlags();

    void writeAngle(float value);
    float readAngle();

    void writeLightData(LightData value);
    LightData readLightData();
};

}
