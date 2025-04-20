#pragma once

#include <string>
#include <vector>
#include "BitSet.h"
#include "NBT.h"

namespace zinc {

struct SoundEvent {
    std::string m_soundName;
    bool m_hasFixedRange;
    float m_fixedRange;

    SoundEvent();
    SoundEvent(std::string const& soundName);
    SoundEvent(std::string const& soundName, float fixedRange);

    bool operator==(SoundEvent const& b);
    bool operator!=(SoundEvent const& b);
};

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

struct ChunkDataBlockEntity {
    char m_x, m_z;
    short m_y;
    int m_type;
    NBTTag m_data;

    bool operator==(ChunkDataBlockEntity& b);
    bool operator!=(ChunkDataBlockEntity& b);
};
struct ChunkData {
    NBTTag m_heightMaps;
    std::vector<char> m_data;
    std::vector<ChunkDataBlockEntity> m_blockEntities;

    bool operator==(ChunkData& b);
    bool operator!=(ChunkData& b);
};

struct LightData {
    BitSet m_skyLightMask;
    BitSet m_blockLightMask;
    BitSet m_skyLightMaskEmpty;
    BitSet m_blockLightMaskEmpty;
    std::vector<std::vector<char>> m_skyLightArrays;   // char[2048]*
    std::vector<std::vector<char>> m_blockLightArrays; // char[2048]*

    bool operator==(LightData const& b);
    bool operator!=(LightData const& b);
};

}