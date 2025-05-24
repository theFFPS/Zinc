#pragma once

#include <vector>
#include "nbt/NBTElement.h"

namespace zinc {

struct ChunkDataHeightMap {
    int m_type;
    std::vector<long> m_data;
};
struct ChunkDataBlockEntity {
    unsigned char m_x, m_z; // chunk rel coords
    short m_y; // world rel coord
    int m_blockEntityTypeInt;
    NBTElement m_data;
};
struct ChunkData {
    std::vector<ChunkDataHeightMap> m_heightMaps;
    std::vector<char> m_data;
    std::vector<ChunkDataBlockEntity> m_blockEntities;
};

}