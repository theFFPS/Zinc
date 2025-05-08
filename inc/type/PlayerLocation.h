#pragma once

#include "Vector.h"

namespace zinc {

struct PlayerLocation {
private:
    Vector3f m_position;
    float m_pitch, m_headYaw, m_yaw;
public:
    PlayerLocation() : m_position(Vector3f()), m_pitch(0), m_headYaw(0), m_yaw(0) {}
    PlayerLocation(const Vector3f& position, const float& pitch, const float& headYaw, const float& yaw)
        : m_position(position), m_pitch(pitch), m_headYaw(headYaw), m_yaw(yaw) {}
    
    Vector3f& getPosition();
    Vector3f getPosition() const;
    float& getPitch();
    float getPitch() const;
    float& getHeadYaw();
    float getHeadYaw() const;
    float& getYaw();
    float getYaw() const;

    void setPosition(const Vector3f& position);
    void setPitch(const float& pitch);
    void setHeadYaw(const float& headYaw);
    void setYaw(const float& yaw);

    bool operator==(const PlayerLocation& playerLocation) const;
    bool operator!=(const PlayerLocation& playerLocation) const;
};

}