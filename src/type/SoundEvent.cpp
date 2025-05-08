#include <type/SoundEvent.h>

namespace zinc {

Identifier& SoundEvent::getIdentifier() {
    return m_identifier;
}
Identifier SoundEvent::getIdentifier() const {
    return m_identifier;
}
bool SoundEvent::hasFixedRange() const {
    return m_fixedRange.has_value();
}
float SoundEvent::getFixedRange() const {
    if (hasFixedRange()) return m_fixedRange.value();
    return 0;
}

void SoundEvent::setIdentifier(const Identifier& identifier) {
    m_identifier = identifier;
}
void SoundEvent::setFixedRange(const float& fixedRange) {
    m_fixedRange = fixedRange;
}
void SoundEvent::resetFixedRange() {
    m_fixedRange.reset();
}

bool SoundEvent::operator==(const SoundEvent& soundEvent) {
    bool doesFixedRangeMatch = (m_fixedRange.has_value() == soundEvent.hasFixedRange());
    if (doesFixedRangeMatch) {
        doesFixedRangeMatch = (m_fixedRange.value() == soundEvent.getFixedRange());
    }
    return doesFixedRangeMatch && m_identifier == soundEvent.getIdentifier();
}
bool SoundEvent::operator!=(const SoundEvent& soundEvent) {
    return !operator==(soundEvent);
}

}