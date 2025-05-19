#include <type/ByteBuffer.h>
#include <stdexcept>
#include <registry/DefaultRegistries.h>

namespace zinc {

ByteBuffer::InternalByteBuffer::InternalByteBuffer() {
    m_blocks.emplace_back(BLOCK_SIZE);
}
ByteBuffer::InternalByteBuffer::~InternalByteBuffer() {
    clear();
}
void ByteBuffer::InternalByteBuffer::clear() noexcept {
    std::lock_guard lock(m_mutex);
    m_blocks.clear();
    m_blocks.emplace_back(BLOCK_SIZE);
    m_writeBlock = 0;
    m_readBlock = 0;
    m_writeOffset = 0;
    m_readOffset = 0;
}
void ByteBuffer::InternalByteBuffer::write(const char* data, const size_t& length) {
    std::lock_guard lock(m_mutex);
    size_t remaining = length;
    char *l_data = (char*) data;
    while (remaining) {
        const size_t spaceLeft = (BLOCK_SIZE - m_writeOffset);
        const size_t toCopy = std::min(remaining, spaceLeft);
        std::vector<char>& block = m_blocks[m_writeBlock];
        std::memcpy(block.data() + m_writeOffset, l_data, toCopy);
        m_writeOffset += toCopy;
        l_data += toCopy;
        remaining -= toCopy;
        if (m_writeOffset == BLOCK_SIZE) {
            m_writeBlock++;
            m_writeOffset = 0;
            m_blocks.emplace_back(BLOCK_SIZE);
        }
    }
}
std::vector<char> ByteBuffer::InternalByteBuffer::read(const size_t& length) {
    std::lock_guard lock(m_mutex);
    std::vector<char> result;
    result.reserve(length);
    size_t remaining = length;
    while (remaining) {
        if (m_readBlock >= m_blocks.size()) {
            m_logger.error(std::out_of_range("Not enough data to read").what());
            return result;
        }
        const std::vector<char>& block = m_blocks[m_readBlock];
        const size_t available = (BLOCK_SIZE - m_readOffset);
        const size_t toCopy = std::min(remaining, available);
        result.insert(result.end(), block.begin() + m_readOffset, block.begin() + m_readOffset + toCopy);
        m_readOffset += toCopy;
        remaining -= toCopy;
        if (m_readOffset == BLOCK_SIZE) {
            m_readBlock++;
            m_readOffset = 0;
            if (m_enableBlockRecycle) {
                m_readBlock--;
                if (m_writeBlock) m_writeBlock--;
                m_blocks.pop_front();
            }
        }
    }
    return result;
}
bool& ByteBuffer::InternalByteBuffer::areBlocksRecycled() {
    return m_enableBlockRecycle;
}
bool ByteBuffer::InternalByteBuffer::areBlocksRecycled() const {
    return m_enableBlockRecycle;
}
void ByteBuffer::InternalByteBuffer::toggleBlockRecycle(const bool& state) {
    m_enableBlockRecycle = state;
}
std::vector<char> ByteBuffer::InternalByteBuffer::getBytes() const {
    std::vector<char> result;
    size_t totalSize = 0;
    for (size_t i = 0; i < m_blocks.size(); ++i) {
        if (i < m_writeBlock) {
            totalSize += BLOCK_SIZE;
        } else if (i == m_writeBlock) {
            totalSize += m_writeOffset;
        }
    }
    result.reserve(totalSize);
    for (size_t i = 0; i < m_blocks.size(); ++i) {
        if (i < m_writeBlock) {
            result.insert(result.end(), m_blocks[i].begin(), m_blocks[i].end());
        } else if (i == m_writeBlock) {
            result.insert(result.end(), m_blocks[i].begin(), m_blocks[i].begin() + m_writeOffset);
            break;
        } else {
            break;
        }
    }
    result.shrink_to_fit();
    return result;
}
std::pair<size_t, size_t> ByteBuffer::InternalByteBuffer::getReader() const {
    return std::make_pair(m_readBlock, m_readOffset);
}
std::pair<size_t, size_t> ByteBuffer::InternalByteBuffer::getWriter() const {
    return std::make_pair(m_writeBlock, m_writeOffset);
}
void ByteBuffer::InternalByteBuffer::setReader(const std::pair<size_t, size_t>& reader) {
    m_readBlock = reader.first;
    m_readOffset = reader.first;
}
void ByteBuffer::InternalByteBuffer::setWriter(const std::pair<size_t, size_t>& writer) {
    m_writeBlock = writer.first;
    m_writeOffset = writer.first;
}
bool ByteBuffer::InternalByteBuffer::operator==(const InternalByteBuffer& buffer) const {
    return getReader() == buffer.getReader() && getWriter() == buffer.getWriter() && getBytes() == buffer.getBytes() 
        && areBlocksRecycled() == buffer.areBlocksRecycled();
}
bool ByteBuffer::InternalByteBuffer::operator!=(const InternalByteBuffer& buffer) const {
    return !operator==(buffer);
}

void ByteBuffer::clear() noexcept {
    m_internalBuffer.clear();
}

std::vector<char> ByteBuffer::getBytes() const {
    return m_internalBuffer.getBytes();
}
size_t ByteBuffer::size() const {
    std::pair<size_t, size_t> writer = m_internalBuffer.getWriter();
    return (writer.first * InternalByteBuffer::BLOCK_SIZE) + writer.second;
}
size_t ByteBuffer::getReaderPointer() const {
    std::pair<size_t, size_t> reader = m_internalBuffer.getReader();
    return (reader.first * InternalByteBuffer::BLOCK_SIZE) + reader.second;
}

void ByteBuffer::writeBytes(const std::vector<char>& bytes) {
    m_internalBuffer.write(bytes.data(), bytes.size());
}
std::vector<char> ByteBuffer::readBytes(const size_t& length) {
    return m_internalBuffer.read(length);
}

void ByteBuffer::writeByte(const char& c) {
    m_internalBuffer.write(&c, 1);
}
char ByteBuffer::readByte() {
    std::vector<char> readBytes = m_internalBuffer.read(1);
    return readBytes[0];
}
void ByteBuffer::writeUnsignedByte(const unsigned char& c) {
    writeByte((const char&)c);
}
unsigned char ByteBuffer::readUnsignedByte() {
    return (unsigned char) readByte();
}

void ByteBuffer::writeString(const std::string& value) {
    writeVarNumeric<int>(value.size());
    writeBytes(std::vector<char>(value.begin(), value.end()));
}
std::string ByteBuffer::readString() {
    std::vector<char> bytes = readBytes(readVarNumeric<int>());
    return std::string(bytes.begin(), bytes.end());
}
void ByteBuffer::writeIdentifier(const Identifier& value) {
    writeString(value.toString());
}
Identifier ByteBuffer::readIdentifier() {
    return Identifier(readString());
}

void ByteBuffer::writePosition(const Vector3i& value) {
    writeNumeric<long>((((long) value.getX() & 0x3FFFFFFL) << 38) | ((value.getZ() & 0x3FFFFFFL) << 12) | (value.getY() & 0xFFFL));
}
Vector3i ByteBuffer::readPosition() {
    long longVal = readNumeric<long>();
    int x = static_cast<int>(longVal >> 38);
    int z = static_cast<int>((longVal >> 12) & 0x3FFFFFFL);
    int y = static_cast<int>(longVal & 0xFFFL);
    x = (x << 6) >> 6;
    z = (z << 6) >> 6;
    y = (y << 20) >> 20;
    return Vector3i(x, y, z);
}

void ByteBuffer::writeAngle(const float& angle) {
    writeUnsignedByte((unsigned char)(angle * 256));
}
float ByteBuffer::readAngle() {
    return (float)readUnsignedByte() / 256;
}

void ByteBuffer::writeUUID(const uuids::uuid& uuid) {
    for (const std::byte& c : uuid.as_bytes()) writeUnsignedByte((unsigned char) c);
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
    writeVarNumeric<int>(bytes.size());
    writeBytes(bytes);
}
std::vector<char> ByteBuffer::readByteArray(const size_t& length) {
    return readBytes(length);
}
std::vector<char> ByteBuffer::readPrefixedByteArray() {
    return readBytes(readVarNumeric<int>());
}

void ByteBuffer::writeIDSet(const IDSet& idSet) {
    writeVarNumeric<int>(idSet.getType());
    if (!idSet.getType()) writeIdentifier(idSet.getIdentifier());
    else writeArray<int>(idSet.getIDs(), &ByteBuffer::writeVarNumeric<int>);
}
IDSet ByteBuffer::readIDSet() {
    IDSet result;
    result.setType(readVarNumeric<int>());
    if (result.getType()) result.setIDs(readArray<int>(&ByteBuffer::readVarNumeric<int>, result.getType() - 1));
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
    writeVarNumeric<int>(blockCoordinates.getX());
    writeVarNumeric<int>(blockCoordinates.getY());
    writeVarNumeric<int>(blockCoordinates.getZ());
}
Vector3i ByteBuffer::readBlockCoordinates() {
    return Vector3i(readVarNumeric<int>(), readVarNumeric<int>(), readVarNumeric<int>());
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

void ByteBuffer::writeNBTElement(const NBTElement& nbtElement) {
    NBTElement element = nbtElement;
    NBTSettings settings;
    settings.m_isNetwork = true;
    element.m_settings = settings;
    element.encode(*this);
}
NBTElement ByteBuffer::readNBTElement() {
    NBTSettings settings;
    settings.m_isNetwork = true;
    return NBTElement(*this, settings);
}

void ByteBuffer::writeTextComponent(const TextComponent& textComponent) {
    NBTElement NBT = textComponent.encode(NBTSettings(false, true, NBTElementType::End));
    writeNBTElement(NBT);
}
TextComponent ByteBuffer::readTextComponent() {
    NBTElement NBT = readNBTElement();
    return TextComponent(NBT);
}

void ByteBuffer::writeChatType(const ChatType& chatType) {
    writeString(chatType.m_chat.m_translationKey);
    std::vector<int> paramInt;
    for (const auto& param : chatType.m_chat.m_parameters) paramInt.push_back((int) param);
    writePrefixedArray<int>(paramInt, &ByteBuffer::writeVarNumeric<int>);
    writeNBTElement(chatType.m_chat.m_style);
    writeString(chatType.m_narration.m_translationKey);
    paramInt = {};
    for (const auto& param : chatType.m_chat.m_parameters) paramInt.push_back((int) param);
    writePrefixedArray<int>(paramInt, &ByteBuffer::writeVarNumeric<int>);
    writeNBTElement(chatType.m_narration.m_style);
}
ChatType ByteBuffer::readChatType() {
    ChatType chatType;
    chatType.m_chat.m_translationKey = readString();
    std::vector<int> paramInt;
    paramInt = readPrefixedArray<int>(&ByteBuffer::readVarNumeric<int>);
    for (const int& param : paramInt) chatType.m_chat.m_parameters.push_back((ChatTypeDecoration::Parameter) param);
    chatType.m_chat.m_style = readNBTElement();
    chatType.m_narration.m_translationKey = readString();
    paramInt = readPrefixedArray<int>(&ByteBuffer::readVarNumeric<int>);
    for (const int& param : paramInt) chatType.m_narration.m_parameters.push_back((ChatTypeDecoration::Parameter) param);
    chatType.m_narration.m_style = readNBTElement();
    return chatType;
}

void ByteBuffer::writeSlot(const Slot& slot) {
    writeByteArray(slot.toBytes());
}
void ByteBuffer::writeHashedSlot(const Slot& slot) {
    writeByteArray(slot.toBytesHashed());
}

void ByteBuffer::writeSlotDisplay(const SlotDisplay& display) {
    writeByteArray(display.toBytes());
}
SlotDisplay ByteBuffer::readSlotDisplay() {
    SlotDisplay display;
    display.m_type = getSlotDisplayTypeId(readVarNumeric<int>());
    SlotDisplayData data = g_slotDisplayReaders[display.m_type.toString()](*this);
    display.m_itemType = data.m_itemType;
    display.m_itemStack = data.m_itemStack;
    display.m_tag = data.m_tag;
    display.m_children = data.m_children;
    display.m_customData = data.m_customData;
    return display;
}

void ByteBuffer::writeRecipeDisplay(const RecipeDisplay& display) {
    writeByteArray(display.toBytes());
}
RecipeDisplay ByteBuffer::readRecipeDisplay() {
    RecipeDisplay display;
    display.m_type = getRecipeDisplayTypeId(readVarNumeric<int>());
    RecipeDisplayData data = g_recipeDisplayReaders[display.m_type.toString()](*this);
    display.m_width = data.m_width;
    display.m_height = data.m_height;
    display.m_ingredients = data.m_ingredients;
    display.m_fuel = data.m_fuel;
    display.m_result = data.m_result;
    display.m_craftingStation = data.m_craftingStation;
    display.m_cookingTime = data.m_cookingTime;
    display.m_experience = data.m_experience;
    display.m_customData = data.m_customData;
    return display;
}

void ByteBuffer::writePartialDataComponentMatcher(const PartialDataComponentMatcher& component) {
    writeByteArray(component.toBytes());
}
PartialDataComponentMatcher ByteBuffer::readPartialDataComponentMatcher() {
    PartialDataComponentMatcher component;
    component.m_type = readVarNumeric<int>();
    component.m_predicate = readNBTElement();
    return component;
}

void ByteBuffer::writeProperty(const Property& property) {
    writeByteArray(property.toBytes());
}
Property ByteBuffer::readProperty() {
    Property property;
    property.m_name = readString();
    property.m_isExact = readByte();
    if (property.m_isExact) property.m_exactValue = readString();
    else {
        property.m_minValue = readString();
        property.m_maxValue = readString();
    }
    return property;
}

void ByteBuffer::writeFireworkExplosion(const FireworkExplosion& fireworkExplosion) {
    writeByteArray(fireworkExplosion.toBytes());
}
FireworkExplosion ByteBuffer::readFireworkExplosion() {
    FireworkExplosion fireworkExplosion;
    fireworkExplosion.m_shape = getFireworkExplosionShapeId(readVarNumeric<int>());
    fireworkExplosion.m_colors = readPrefixedArray<int>(&ByteBuffer::readNumeric<int>);
    fireworkExplosion.m_fadeColors = readPrefixedArray<int>(&ByteBuffer::readNumeric<int>);
    fireworkExplosion.m_hasTrail = readByte();
    fireworkExplosion.m_hasTwinkle = readByte();
    return fireworkExplosion;
}

void ByteBuffer::writePotionEffectDetail(const PotionEffectDetail& detail) {
    writeByteArray(detail.toBytes());
}
PotionEffectDetail ByteBuffer::readPotionEffectDetail() {
    PotionEffectDetail detail;
    detail.m_amplifier = readVarNumeric<int>();
    detail.m_duration = readVarNumeric<int>();
    detail.m_ambient = readByte();
    detail.m_showParticles = readByte();
    detail.m_showIcon = readByte();
    if (readByte()) detail.m_hiddenEffect.push_back(readPotionEffectDetail());
    return detail;
}

void ByteBuffer::writePotionEffect(const PotionEffect& effect) {
    writeByteArray(effect.toBytes());
}
PotionEffect ByteBuffer::readPotionEffect() {
    PotionEffect effect;
    effect.m_type = readVarNumeric<int>();
    effect.m_detail = readPotionEffectDetail();
    return effect;
}

void ByteBuffer::writeTrimMaterial(const TrimMaterial& material) {
    writeByteArray(material.toBytes());
}
TrimMaterial ByteBuffer::readTrimMaterial() {
    TrimMaterial material;
    material.m_suffix = readString();
    int overridesAmount = readVarNumeric<int>();
    for (int i = 0; i < overridesAmount; i++) {
        std::vector<std::string> overrideValue;
        overrideValue.push_back(readIdentifier().toString());
        overrideValue.push_back(readString());
        material.m_overrides.push_back(overrideValue);
    }
    material.m_description = readTextComponent();
    return material;
}

void ByteBuffer::writeTrimPattern(const TrimPattern& pattern) {
    writeByteArray(pattern.toBytes());
}
TrimPattern ByteBuffer::readTrimPattern() {
    TrimPattern pattern;
    pattern.m_asset = readString();
    pattern.m_templateItem = readVarNumeric<int>();
    pattern.m_description = readTextComponent();
    pattern.m_decal = readByte();
    return pattern;
}

void ByteBuffer::writeConsumeEffect(const ConsumeEffect& effect) {
    writeByteArray(effect.toBytes());
}
ConsumeEffect ByteBuffer::readConsumeEffect() {
    ConsumeEffect effect;
    effect.m_type = getConsumeEffectTypeId(readVarNumeric<int>());
    ConsumeEffectData data = g_consumeEffectReaders[effect.m_type.toString()](*this);
    effect.m_effects = data.m_effects;
    effect.m_effectsRemove = data.m_effectsRemove;
    effect.m_diameter = data.m_diameter;
    effect.m_probability = data.m_probability;
    effect.m_sound = data.m_sound;
    effect.m_customData = data.m_customData;
    return effect;
}

void ByteBuffer::writeInstrument(const Instrument& instrument) {
    writeByteArray(instrument.toBytes());
}
Instrument ByteBuffer::readInstrument() {
    Instrument instrument;
    instrument.m_soundEvent = readIDorX<SoundEvent>(&ByteBuffer::readSoundEvent);
    instrument.m_soundRange = readNumeric<float>();
    instrument.m_range = readNumeric<float>();
    instrument.m_description = readTextComponent();
    return instrument;
}

void ByteBuffer::writeJukeBox(const JukeBox& jukeBox) {
    writeByteArray(jukeBox.toBytes());
}
JukeBox ByteBuffer::readJukeBox() {
    JukeBox jukeBox;
    jukeBox.m_soundEvent = readIDorX<SoundEvent>(&ByteBuffer::readSoundEvent);
    jukeBox.m_description = readTextComponent();
    jukeBox.m_duration = readNumeric<float>();
    jukeBox.m_output = readVarNumeric<int>();
    return jukeBox;
}

void ByteBuffer::writeBlockPredicate(const BlockPredicate& predicate) {
    writeByteArray(predicate.toBytes());
}
BlockPredicate ByteBuffer::readBlockPredicate() {
    BlockPredicate predicate;
    predicate.m_blocks = readPrefixedOptional<IDSet>(&ByteBuffer::readIDSet);
    bool hasProperties = readByte();
    if (hasProperties) {
        predicate.m_properties = readPrefixedArray<Property>(&ByteBuffer::readProperty);
    }
    predicate.m_NBT = readPrefixedOptional<NBTElement>(&ByteBuffer::readNBTElement);
    int dataComponentsAmount = readVarNumeric<int>();
    for (int i = 0; i < dataComponentsAmount; i++) {
        // invoke data component parser here
    }
    predicate.m_partialDataComponents = readPrefixedArray<PartialDataComponentMatcher>(&ByteBuffer::readPartialDataComponentMatcher);
    return predicate;
}

void ByteBuffer::writeLightData(const LightData& data) {
    writeByteArray(data.toBytes());
}
LightData ByteBuffer::readLightData() {
    LightData data;
    data.m_skyLightMask = readBitSet();
    data.m_blockLightMask = readBitSet();
    data.m_emptySkyLightMask = readBitSet();
    data.m_emptyBlockLightMask = readBitSet();
    data.m_skyLightArrays = readPrefixedArray<std::vector<char>>(&ByteBuffer::readPrefixedByteArray);
    data.m_blockLightArrays = readPrefixedArray<std::vector<char>>(&ByteBuffer::readPrefixedByteArray);
    return data;
}

bool ByteBuffer::operator==(const ByteBuffer& buffer) const {
    return m_isBigEndian == buffer.m_isBigEndian && m_internalBuffer == buffer.m_internalBuffer;
}
bool ByteBuffer::operator!=(const ByteBuffer& buffer) const {
    return !operator==(buffer);
}

}
