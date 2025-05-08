#pragma once

#include <optional>
#include "Identifier.h"

namespace zinc {

struct SoundEvent {
private:
    Identifier m_identifier;
    std::optional<float> m_fixedRange;
public:
    SoundEvent() : m_identifier(Identifier()), m_fixedRange(std::optional<float>()) {}
    SoundEvent(const Identifier& identifier) : m_identifier(identifier), m_fixedRange(std::optional<float>()) {}
    SoundEvent(const Identifier& identifier, const float& fixedRange) : m_identifier(identifier), m_fixedRange(fixedRange) {}

    Identifier& getIdentifier();
    Identifier getIdentifier() const;
    bool hasFixedRange() const;
    float getFixedRange() const;

    void setIdentifier(const Identifier& identifier);
    void setFixedRange(const float& fixedRange);
    void resetFixedRange();

    bool operator==(const SoundEvent& soundEvent);
    bool operator!=(const SoundEvent& soundEvent);
};

}