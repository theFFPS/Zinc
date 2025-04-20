#include <type/GameTypes.h>

namespace zinc {

SoundEvent::SoundEvent() {}
SoundEvent::SoundEvent(std::string const& soundName) : m_soundName(soundName), m_hasFixedRange(false) {}
SoundEvent::SoundEvent(std::string const& soundName, float fixedRange) : m_soundName(soundName), m_hasFixedRange(true), m_fixedRange(fixedRange) {}
bool SoundEvent::operator==(SoundEvent const& b) {
    return b.m_fixedRange == m_fixedRange && m_hasFixedRange == b.m_hasFixedRange && m_soundName == b.m_soundName;
}
bool SoundEvent::operator!=(SoundEvent const& b) {
    return !operator==(b);
}

bool m_relX, m_relY, m_relZ;
bool m_relYaw, m_relPitch;
bool m_relVelocityX, m_relVelocityY, m_relVelocityZ;
bool m_rotVelocity;
int m_rawBitmap;
TeleportFlags::TeleportFlags() 
    : m_relX(false), m_relY(false), m_relZ(false), 
      m_relYaw(false), m_relPitch(false),
      m_relVelocityX(false), m_relVelocityY(false), m_relVelocityZ(false),
      m_rotVelocity(false),
      m_rawBitmap(0) {}
TeleportFlags::TeleportFlags(int bitmap) 
    : m_relX(bitmap & (int) TeleportFlagsBits::RelativeX), m_relY(bitmap & (int) TeleportFlagsBits::RelativeY), 
        m_relZ(bitmap & (int) TeleportFlagsBits::RelativeZ), 
      m_relYaw(bitmap & (int) TeleportFlagsBits::RelativeYaw), m_relPitch(bitmap & (int) TeleportFlagsBits::RelativePitch),
      m_relVelocityX(bitmap & (int) TeleportFlagsBits::RelativeVelocityX), m_relVelocityY(bitmap & (int) TeleportFlagsBits::RelativeVelocityY), 
        m_relVelocityZ(bitmap & (int) TeleportFlagsBits::RelativeVelocityZ),
      m_rotVelocity(bitmap & (int) TeleportFlagsBits::RotateVelocity),
      m_rawBitmap(bitmap) {}
bool TeleportFlags::operator==(int b) {
    update();
    return b == m_rawBitmap;
}
bool TeleportFlags::operator==(TeleportFlags const& b) {
    return operator==(b.m_rawBitmap);
}
bool TeleportFlags::operator!=(int b) {
    return !operator==(b);
}
bool TeleportFlags::operator!=(TeleportFlags const& b) {
    return !operator==(b);
}
void TeleportFlags::update() {
    m_rawBitmap = 0;
    if (m_relX) m_rawBitmap |= (int) TeleportFlagsBits::RelativeX;
    if (m_relY) m_rawBitmap |= (int) TeleportFlagsBits::RelativeY;
    if (m_relZ) m_rawBitmap |= (int) TeleportFlagsBits::RelativeZ;
    if (m_relYaw) m_rawBitmap |= (int) TeleportFlagsBits::RelativeYaw;
    if (m_relPitch) m_rawBitmap |= (int) TeleportFlagsBits::RelativePitch;
    if (m_relVelocityX) m_rawBitmap |= (int) TeleportFlagsBits::RelativeVelocityX;
    if (m_relVelocityY) m_rawBitmap |= (int) TeleportFlagsBits::RelativeVelocityY;
    if (m_relVelocityZ) m_rawBitmap |= (int) TeleportFlagsBits::RelativeVelocityZ;
    if (m_rotVelocity) m_rawBitmap |= (int) TeleportFlagsBits::RotateVelocity;
}
int TeleportFlags::toInt() {
    update();
    return m_rawBitmap;
}

bool ChunkDataBlockEntity::operator==(ChunkDataBlockEntity& b) {
    return m_x == b.m_x && m_z == b.m_z && m_y == b.m_y && m_type == b.m_type && m_data == b.m_data;
}
bool ChunkDataBlockEntity::operator!=(ChunkDataBlockEntity& b) {
    return !operator==(b);
}
bool ChunkData::operator==(ChunkData& b) {
    bool checkEntities = (b.m_blockEntities.size() == m_blockEntities.size());
    if (checkEntities) for (size_t i = 0; i < m_blockEntities.size(); i++) {
        checkEntities = (m_blockEntities[i] == b.m_blockEntities[i]);
        if (!checkEntities) break;
    }
    return m_heightMaps == b.m_heightMaps && m_data == b.m_data && checkEntities;
}
bool ChunkData::operator!=(ChunkData& b) {
    return !operator==(b);
}

bool LightData::operator==(LightData const& b) {
    return m_skyLightArrays == b.m_skyLightArrays && m_blockLightArrays == b.m_blockLightArrays && m_blockLightMask == b.m_blockLightMask &&
           m_blockLightMaskEmpty == b.m_blockLightMaskEmpty && m_skyLightMask == b.m_skyLightMask && m_skyLightMaskEmpty == b.m_skyLightMaskEmpty;
}
bool LightData::operator!=(LightData const& b) {
    return !operator==(b);
}

}