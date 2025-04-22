#include <type/gametypes/TeleportFlags.h>

namespace zinc {

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

}