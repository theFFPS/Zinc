#pragma once

namespace zinc {

enum class TeleportFlagsBits : int {
    RelativeX = 1, RelativeY = 2, RelativeZ = 4,
    RelativeYaw = 8, RelativePitch = 16,
    RelativeVelocityX = 32, RelativeVelocityY = 64, RelativeVelocityZ = 128,
    RotateVelocity = 256
};
struct TeleportFlags {
    bool m_relX, m_relY, m_relZ;
    bool m_relYaw, m_relPitch;
    bool m_relVelocityX, m_relVelocityY, m_relVelocityZ;
    bool m_rotVelocity;
    int m_rawBitmap;
    
    TeleportFlags();
    TeleportFlags(int bitmap);

    bool operator==(int b);
    bool operator==(TeleportFlags const& b);
    bool operator!=(int b);
    bool operator!=(TeleportFlags const& b);

    void update();
    int toInt();
};

}