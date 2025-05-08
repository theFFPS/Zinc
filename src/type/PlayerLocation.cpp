#include <type/PlayerLocation.h>

namespace zinc {

Vector3f& PlayerLocation::getPosition() {
    return m_position;
}
Vector3f PlayerLocation::getPosition() const {
    return m_position;
}
float& PlayerLocation::getPitch() {
    return m_pitch;
}
float PlayerLocation::getPitch() const {
    return m_pitch;
}
float& PlayerLocation::getHeadYaw() {
    return m_headYaw;
}
float PlayerLocation::getHeadYaw() const {
    return m_headYaw;
}
float& PlayerLocation::getYaw() {
    return m_yaw;
}
float PlayerLocation::getYaw() const {
    return m_yaw;
}

void PlayerLocation::setPosition(const Vector3f& position) {
    m_position = position;
}
void PlayerLocation::setPitch(const float& pitch) {
    m_pitch = pitch;
}
void PlayerLocation::setHeadYaw(const float& headYaw) {
    m_headYaw = headYaw;
}
void PlayerLocation::setYaw(const float& yaw) {
    m_yaw = yaw;
}

bool PlayerLocation::operator==(const PlayerLocation& playerLocation) const {
    return m_position == playerLocation.getPosition() && m_pitch == playerLocation.getPitch() && 
           m_headYaw == playerLocation.getHeadYaw() && m_yaw == playerLocation.getYaw();
}
bool PlayerLocation::operator!=(const PlayerLocation& playerLocation) const {
    return !operator==(playerLocation);
}

}