#pragma once

#include "NBTElementType.h"

namespace zinc {

struct NBTSettings {
    bool m_isInArray;
    bool m_isNetwork;
    NBTElementType m_type;
    
    NBTSettings() : m_isInArray(false), m_isNetwork(false), m_type(NBTElementType::End) {}
    NBTSettings(const bool& isInArray, const bool& isNetwork, const NBTElementType& type) : m_isInArray(isInArray), m_isNetwork(isNetwork), m_type(type) {}

    bool operator==(const NBTSettings& settings) const;
    bool operator!=(const NBTSettings& settings) const;
};

}