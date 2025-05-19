#pragma once

#include "BitSet.h"
#include <vector>

namespace zinc {

struct LightData {
    BitSet m_skyLightMask;
    BitSet m_blockLightMask;
    BitSet m_emptySkyLightMask;
    BitSet m_emptyBlockLightMask;
    std::vector<std::vector<char>> m_skyLightArrays;
    std::vector<std::vector<char>> m_blockLightArrays;

    LightData() {}

    std::vector<char> toBytes() const;

    bool operator==(const LightData& data) const;
    bool operator!=(const LightData& data) const;
};

}