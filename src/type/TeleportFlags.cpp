#include <type/TeleportFlags.h>

namespace zinc {

void TeleportFlags::setRelX(const bool& relX) {
    m_relX = relX;
}
void TeleportFlags::setRelY(const bool& relY) {
    m_relY = relY;
}
void TeleportFlags::setRelZ(const bool& relZ) {
    m_relZ = relZ;
}
void TeleportFlags::setRelYaw(const bool& relYaw) {
    m_relYaw = relYaw;
}
void TeleportFlags::setRelPitch(const bool& relPitch) {
    m_relPitch = relPitch;
}
void TeleportFlags::setRelVelocityX(const bool& relVelocityX) {
    m_relVelocityX = relVelocityX;
}
void TeleportFlags::setRelVelocityY(const bool& relVelocityY) {
    m_relVelocityY = relVelocityY;
}
void TeleportFlags::setRelVelocityZ(const bool& relVelocityZ) {
    m_relVelocityZ = relVelocityZ;
}
void TeleportFlags::setRotVelocity(const bool& rotVelocity) {
    m_rotVelocity = rotVelocity;
}

void TeleportFlags::flipRelX() {
    m_relX = !m_relX;
}
void TeleportFlags::flipRelY() {
    m_relY = !m_relY;
}
void TeleportFlags::flipRelZ() {
    m_relZ = !m_relZ;
}
void TeleportFlags::flipRelYaw() {
    m_relYaw = !m_relYaw;
}
void TeleportFlags::flipRelPitch() {
    m_relPitch = !m_relPitch;
}
void TeleportFlags::flipRelVelocityX() {
    m_relVelocityX = !m_relVelocityX;
}
void TeleportFlags::flipRelVelocityY() {
    m_relVelocityY = !m_relVelocityY;
}
void TeleportFlags::flipRelVelocityZ() {
    m_relVelocityZ = !m_relVelocityZ;
}
void TeleportFlags::flipRotVelocity() {
    m_rotVelocity = !m_rotVelocity;
}

bool& TeleportFlags::getRelX() {
    return m_relX;
}
bool TeleportFlags::getRelX() const {
    return m_relX;
}
bool& TeleportFlags::getRelY() {
    return m_relY;
}
bool TeleportFlags::getRelY() const {
    return m_relY;
}
bool& TeleportFlags::getRelZ() {
    return m_relZ;
}
bool TeleportFlags::getRelZ() const {
    return m_relZ;
}
bool& TeleportFlags::getRelYaw() {
    return m_relYaw;
}
bool TeleportFlags::getRelYaw() const {
    return m_relYaw;
}
bool& TeleportFlags::getRelPatch() {
    return m_relPitch;
}
bool TeleportFlags::getRelPatch() const {
    return m_relPitch;
}
bool& TeleportFlags::getRelVelocityX() {
    return m_relVelocityX;
}
bool TeleportFlags::getRelVelocityX() const {
    return m_relVelocityX;
}
bool& TeleportFlags::getRelVelocityY() {
    return m_relVelocityY;
}
bool TeleportFlags::getRelVelocityY() const {
    return m_relVelocityY;
}
bool& TeleportFlags::getRelVelocityZ() {
    return m_relVelocityZ;
}
bool TeleportFlags::getRelVelocityZ() const {
    return m_relVelocityZ;
}
bool& TeleportFlags::getRotVelocity() {
    return m_rotVelocity;
}
bool TeleportFlags::getRotVelocity() const {
    return m_rotVelocity;
}

int TeleportFlags::encode() const {
    int result = 0;
    if (m_relX) result += 1;
    if (m_relY) result += 2;
    if (m_relZ) result += 4;
    if (m_relYaw) result += 8;
    if (m_relPitch) result += 16;
    if (m_relVelocityX) result += 32;
    if (m_relVelocityY) result += 64;
    if (m_relVelocityZ) result += 128;
    if (m_rotVelocity) result += 256;
    return result;
}

bool TeleportFlags::operator==(const TeleportFlags& flags) const {
    return encode() == flags.encode();
}
bool TeleportFlags::operator!=(const TeleportFlags& flags) const {
    return encode() != flags.encode();
}

}