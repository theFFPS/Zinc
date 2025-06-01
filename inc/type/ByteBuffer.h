#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <mutex>
#include <optional>
#include <type_traits>
#include <array>
#include <deque>

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
#include "Recipe.h"
#include "GameTypes.h"
#include "LightData.h"
#include "ChunkData.h"
#include "XorY.h"
#include <util/Memory.h>
#include "nbt/NBTElement.h"

namespace zinc {

struct ConsumeEffectData {
    std::vector<PotionEffect> m_effects;
    float m_probability;
    IDSet m_effectsRemove;
    float m_diameter;
    SoundEvent m_sound;
    std::vector<char> m_customData;
};
struct RecipeDisplayData {
    int m_width;
    int m_height;
    std::vector<SlotDisplay> m_ingredients;
    SlotDisplay m_fuel;
    SlotDisplay m_result;
    SlotDisplay m_craftingStation;
    int m_cookingTime;
    float m_experience;
    std::vector<char> m_customData;
};
struct SlotDisplayData {
    int m_itemType;
    Slot m_itemStack;
    Identifier m_tag;
    std::vector<SlotDisplay> m_children;
    std::vector<char> m_customData;
};
struct ByteBuffer {
    Logger m_logger = Logger("ByteBuffer");
    struct InternalByteBuffer {
    public:
        static constexpr size_t BLOCK_SIZE = 4096;
    private:
        Logger m_logger = Logger("InternalByteBuffer");
        std::deque<std::vector<char>> m_blocks;
        size_t m_writeBlock = 0, m_writeOffset = 0;
        size_t m_readBlock = 0, m_readOffset = 0;
        std::mutex m_mutex;

        bool m_enableBlockRecycle = false;
    public:
        InternalByteBuffer();
        ~InternalByteBuffer();

        void clear() noexcept;

        void write(const char* data, const size_t& length);
        std::vector<char> read(const size_t& length);

        bool& areBlocksRecycled();
        bool areBlocksRecycled() const;
        std::pair<size_t, size_t> getReader() const;
        std::pair<size_t, size_t> getWriter() const;

        void toggleBlockRecycle(const bool& state);
        void setReader(const std::pair<size_t, size_t>& reader);
        void setWriter(const std::pair<size_t, size_t>& writer);

        std::vector<char> getBytes() const;

        bool operator==(const InternalByteBuffer& buffer) const;
        bool operator!=(const InternalByteBuffer& buffer) const;
    } m_internalBuffer;
    bool m_isBigEndian;

    ByteBuffer() : m_isBigEndian(true) {}
    ByteBuffer(const bool& isBigEndian) : m_isBigEndian(isBigEndian) {}
    ByteBuffer(const std::vector<char>& bytes, const size_t& readPtr = 0) : m_isBigEndian(true) {
        size_t readBlock = (readPtr / InternalByteBuffer::BLOCK_SIZE);
        size_t readOffset = (readPtr % InternalByteBuffer::BLOCK_SIZE);
        m_internalBuffer.write(bytes.data(), bytes.size());
        m_internalBuffer.setReader(std::make_pair(readBlock, readOffset));
    }
    ByteBuffer(const std::vector<char>& bytes, const size_t& readPtr, const bool& isBigEndian) : m_isBigEndian(isBigEndian) {
        size_t readBlock = (readPtr / InternalByteBuffer::BLOCK_SIZE);
        size_t readOffset = (readPtr % InternalByteBuffer::BLOCK_SIZE);
        m_internalBuffer.write(bytes.data(), bytes.size());
        m_internalBuffer.setReader(std::make_pair(readBlock, readOffset));
    }
    ByteBuffer(ByteBuffer&& buffer) : m_isBigEndian(buffer.m_isBigEndian) {
        std::vector<char> bytes = buffer.m_internalBuffer.getBytes();
        m_internalBuffer.write(bytes.data(), bytes.size());
        m_internalBuffer.setReader(buffer.m_internalBuffer.getReader());
    }

    void clear() noexcept;

    std::vector<char> getBytes() const;
    size_t size() const;
    size_t getReaderPointer() const;

    void writeBytes(const std::vector<char>& bytes);
    std::vector<char> readBytes(const size_t& length);

    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>> void writeNumeric(const T& value) {
        if constexpr (std::is_same_v<T, bool>) { writeByte(value ? 1 : 0); return; }
        if constexpr (std::is_same_v<T, char>) { writeByte(value); return; }
        if constexpr (std::is_same_v<T, unsigned char>) { writeUnsignedByte(value); return; }
        std::array<char, sizeof(T)> bytes;
        std::copy((char*)&value, ((char*)&value) + sizeof(T), bytes.begin());
        if (m_isBigEndian) std::reverse(bytes.begin(), bytes.end());
        m_internalBuffer.write(bytes.data(), bytes.size());
    }
    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>> T readNumeric() {
        if constexpr (std::is_same_v<T, bool>) return readByte();
        if constexpr (std::is_same_v<T, char>) return readByte();
        if constexpr (std::is_same_v<T, unsigned char>) return readUnsignedByte();
        T result = 0;
        std::vector<char> bytes = readBytes(sizeof(T));
        if (m_isBigEndian) std::reverse(bytes.begin(), bytes.end());
        std::copy(bytes.begin(), bytes.end(), (char*)&result);
        return result;
    }

    void writeByte(const char& c);
    char readByte();
    void writeUnsignedByte(const unsigned char& c);
    unsigned char readUnsignedByte();

    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>> static constexpr size_t varNumericMaxSize() noexcept { 
        return sizeof(T) * 8 / 7 + 1;
    }
    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>> static size_t getVarNumericLength(const T& value) {
        size_t count = 0;
        using UnsignedT = std::make_unsigned_t<T>;
        UnsignedT uv = static_cast<UnsignedT>(value);
        do { count++; uv >>= 7; } while (uv);
        return count;
    }
    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>> void writeVarNumeric(const T& value) {
        std::array<uint8_t, varNumericMaxSize<T>()> temp;
        size_t count = 0;
        using UnsignedT = std::make_unsigned_t<T>;
        UnsignedT uv = static_cast<UnsignedT>(value);
        do { temp[count++] = (uv & 0x7F) | 0x80; uv >>= 7; } while (uv);
        temp[count-1] &= 0x7F;
        m_internalBuffer.write((const char*) temp.data(), count);
    }
    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>> T readVarNumeric() {
        using UnsignedT = std::make_unsigned_t<T>;
        UnsignedT result = 0;
        int shift = 0;
        std::vector<char> readByte;
        const auto maxBytes = varNumericMaxSize<T>();
        for (size_t i = 0; i < maxBytes; ++i) {
            readByte = m_internalBuffer.read(1);
            const uint8_t byte = zinc_safe_cast<char, uint8_t>(readByte[0]);
            result |= static_cast<UnsignedT>(byte & 0x7F) << shift;
            if (!(byte & 0x80)) {
                if constexpr (std::is_signed_v<T>) {
                    constexpr auto signBit = static_cast<UnsignedT>(1) << (sizeof(T) * 8 - 1);
                    if (result & signBit) {
                        result |= static_cast<UnsignedT>(~0) << (sizeof(T) * 8 - 1);
                    }
                }
                return static_cast<T>(result);
            }
            shift += 7;
            if (shift >= (int)(sizeof(T) * 8)) {
                m_logger.error("VarInt overflow");
                return 0;
            }
        }
        m_logger.error("Malformed VarInt");
        return 0;
    }
    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>> static size_t getZigZagVarNumericLength(const T& value) {
        using UnsignedT = std::make_unsigned_t<T>;
        auto transformed = static_cast<UnsignedT>((value << 1) ^ (value >> (sizeof(T) * 8 - 1)));
        return getVarNumericLength<UnsignedT>(transformed);
    }
    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>> void writeZigZagVarNumeric(const T& value) {
        using UnsignedT = std::make_unsigned_t<T>;
        auto transformed = static_cast<UnsignedT>((value << 1) ^ (value >> (sizeof(T) * 8 - 1)));
        writeVarNumeric<UnsignedT>(transformed);
    }
    template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>> T readZigZagVarNumeric() {
        using UnsignedT = std::make_unsigned_t<T>;
        UnsignedT uv = readVarNumeric<UnsignedT>();
        return static_cast<T>((uv >> 1) ^ (-(uv & 1)));
    }

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
    template<typename T> void writeOptional(const std::optional<T>& value, std::function<void(const T&, ByteBuffer&)>) {
        if (value.has_value()) func(value.value(), *this);
    }
    template<typename T> void writePrefixedOptional(const std::optional<T>& value, void(ByteBuffer::*func)(const T&)) {
        writeByte(value.has_value());
        if (value.has_value()) (this->*func)(value.value());
    }
    template<typename T> void writePrefixedOptional(const std::optional<T>& value, std::function<void(const T&, ByteBuffer&)> func) {
        writeByte(value.has_value());
        if (value.has_value()) func(value.value(), *this);
    }
    template<typename T> std::optional<T> readOptional(T(ByteBuffer::*func)(), bool hasValue) {
        std::optional<T> result;
        if (hasValue) result = (this->*func)();
        return result;
    }
    template<typename T> std::optional<T> readOptional(std::function<T(ByteBuffer&)> func, bool hasValue) {
        std::optional<T> result;
        if (hasValue) result = func(*this);
        return result;
    }
    template<typename T> std::optional<T> readPrefixedOptional(T(ByteBuffer::*func)()) {
        return readOptional<T>(func, readByte());
    }
    template<typename T> std::optional<T> readPrefixedOptional(std::function<T(ByteBuffer&)> func) {
        return readOptional<T>(func, readByte());
    }

    template<typename T> void writeArray(const std::vector<T>& value, void(ByteBuffer::*func)(const T&)) {
        for (const T& element : value) (this->*func)(element);
    }
    template<typename T> void writeArray(const std::vector<T>& value, std::function<void(const T&, ByteBuffer&)> func) {
        for (const T& element : value) func(element, *this);
    }
    template<typename T> void writePrefixedArray(const std::vector<T>& value, void(ByteBuffer::*func)(const T&)) {
        writeVarNumeric<int>(zinc_safe_cast<size_t, int>(value.size()));
        writeArray(value, func);
    }
    template<typename T> void writePrefixedArray(const std::vector<T>& value, std::function<void(const T&, ByteBuffer&)> func) {
        writeVarNumeric<int>(zinc_safe_cast<size_t, int>(value.size()));
        writeArray(value, func);
    }
    template<typename T> std::vector<T> readArray(T(ByteBuffer::*func)(), size_t length) {
        std::vector<T> result;
        for (size_t i = 0; i < length; i++) result.push_back((this->*func)());
        return result;
    }
    template<typename T> std::vector<T> readArray(std::function<T(ByteBuffer&)> func, size_t length) {
        std::vector<T> result;
        for (size_t i = 0; i < length; i++) result.push_back(func(*this));
        return result;
    }
    template<typename T> std::vector<T> readPrefixedArray(T(ByteBuffer::*func)()) {
        return readArray(func, zinc_safe_cast<int, size_t>(readVarNumeric<int>()));
    }
    template<typename T> std::vector<T> readPrefixedArray(std::function<T(ByteBuffer&)> func) {
        return readArray(func, zinc_safe_cast<int, size_t>(readVarNumeric<int>()));
    }

    void writeByteArray(const std::vector<char>& bytes);
    void writePrefixedByteArray(const std::vector<char>& bytes);
    std::vector<char> readByteArray(const size_t& length);
    std::vector<char> readPrefixedByteArray();

    template<typename T> void writeIDorX(const IDorX<T>& value, std::function<void(const T&, ByteBuffer&)> func) {
        writeVarNumeric<int>(value.getId());
        if (!value.getId()) func(value.getValue());
    }
    template<typename T> void writeIDorX(const IDorX<T>& value, void(ByteBuffer::*func)(const T&)) {
        writeVarNumeric<int>(value.getId());
        if (!value.getId()) (this->*func)(value.getValue());
    }
    template<typename T> IDorX<T> readIDorX(std::function<T(ByteBuffer&)> func) {
        IDorX<T> result;
        result.setId(readVarNumeric<int>());
        if (!result.getId()) result.setValue(func(*this));
        return result;
    }
    template<typename T> IDorX<T> readIDorX(T(ByteBuffer::*func)()) {
        IDorX<T> result;
        result.setId(readVarNumeric<int>());
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

    template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>> void writeEnumSet(const std::vector<T>& enumSet) {
        BitSet bitSet;
        for (const T& value : enumSet) {
            bitSet.set((unsigned long) value);
        }
        writeFixedBitSet(bitSet);
    }
    template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>> std::vector<T> readEnumSet(const size_t& length) {
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

    void writeSlotDisplay(const SlotDisplay& display);
    SlotDisplay readSlotDisplay();

    void writeRecipeDisplay(const RecipeDisplay& display);
    RecipeDisplay readRecipeDisplay();

    void writePartialDataComponentMatcher(const PartialDataComponentMatcher& component);
    PartialDataComponentMatcher readPartialDataComponentMatcher();

    void writeProperty(const Property& property);
    Property readProperty();

    void writeFireworkExplosion(const FireworkExplosion& fireworkExplosion);
    FireworkExplosion readFireworkExplosion();

    void writePotionEffectDetail(const PotionEffectDetail& detail);
    PotionEffectDetail readPotionEffectDetail();

    void writePotionEffect(const PotionEffect& effect);
    PotionEffect readPotionEffect();

    void writeTrimMaterial(const TrimMaterial& material);
    TrimMaterial readTrimMaterial();

    void writeTrimPattern(const TrimPattern& pattern);
    TrimPattern readTrimPattern();

    void writeConsumeEffect(const ConsumeEffect& effect);
    ConsumeEffect readConsumeEffect();

    void writeInstrument(const Instrument& instrument);
    Instrument readInstrument();

    void writeJukeBox(const JukeBox& jukeBox);
    JukeBox readJukeBox();

    void writeBlockPredicate(const BlockPredicate& predicate);

    void writeLightData(const LightData& data);
    LightData readLightData();

    void writeChunkData(const ChunkData& data);
    ChunkData readChunkData();

    template<typename TX, typename TY> void writeXorY(const XorY<TX, TY>& xOrY, std::function<void(const TX&, ByteBuffer&)> funcX, 
                                                      std::function<void(const TY&, ByteBuffer&)> funcY) {
        if (xOrY.m_x.has_value() == xOrY.m_y.has_value()) return;
        writeByte(xOrY.m_x.has_value());
        if (xOrY.m_x.has_value()) funcX(xOrY.m_x.value(), *this);
        else funcY(xOrY.m_y.value(), *this);
    }
    template<typename TX, typename TY> void writeXorY(const XorY<TX, TY>& xOrY, void(ByteBuffer::*funcX)(const TX&), void(ByteBuffer::*funcY)(const TY&)) {
        if (xOrY.m_x.has_value() == xOrY.m_y.has_value()) return;
        if (xOrY.m_x.has_value()) (this->*funcX)(xOrY.m_x.value());
        else (this->*funcY)(xOrY.m_y.value());
    }
    template<typename TX, typename TY> XorY<TX, TY> readXorY(std::function<TX(ByteBuffer&)> funcX, std::function<TY(ByteBuffer&)> funcY) {
        if (readByte()) return XorY<TX, TY>(funcX(*this));
        return XorY<TX, TY>(funcY(*this));
    }
    template<typename TX, typename TY> XorY<TX, TY> readXorY(TX(ByteBuffer::*funcX)(), TY(ByteBuffer::*funcY)()) {
        if (readByte()) return XorY<TX, TY>((this->*funcX)());
        return XorY<TX, TY>((this->*funcY)());
    }

    bool operator==(const ByteBuffer& buffer) const;
    bool operator!=(const ByteBuffer& buffer) const;
};

}
