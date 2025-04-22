#pragma once

#include "../BitSet.h"
#include <vector>

namespace zinc {

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