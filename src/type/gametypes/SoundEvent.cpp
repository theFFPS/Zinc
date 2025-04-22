#include <type/gametypes/SoundEvent.h>

namespace zinc {

SoundEvent::SoundEvent() {}
SoundEvent::SoundEvent(std::string const& soundName) : m_soundName(soundName), m_hasFixedRange(false) {}
SoundEvent::SoundEvent(std::string const& soundName, float fixedRange) : m_soundName(soundName), m_hasFixedRange(true), m_fixedRange(fixedRange) {}
bool SoundEvent::operator==(SoundEvent const& b) {
    return b.m_fixedRange == m_fixedRange && m_hasFixedRange == b.m_hasFixedRange && m_soundName == b.m_soundName;
}
bool SoundEvent::operator!=(SoundEvent const& b) {
    return !operator==(b);
}

}