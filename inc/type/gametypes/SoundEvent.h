#pragma once

#include <string>

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

}