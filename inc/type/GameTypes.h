#pragma once

#include "nbt/NBTElement.h"
#include <vector>
#include <optional>
#include "TextComponent.h"
#include "SoundEvent.h"
#include "IDSet.h"
#include "IDorX.h"

namespace zinc {

using ExactDataComponentMatcher = std::vector<char>;
struct PartialDataComponentMatcher {
    int m_type;
    NBTElement m_predicate;

    PartialDataComponentMatcher() : m_type(-1) {}
    PartialDataComponentMatcher(const int& type, const NBTElement& predicate) : m_type(type), m_predicate(predicate) {}

    std::vector<char> toBytes() const;

    bool operator==(const PartialDataComponentMatcher& component) const;
    bool operator!=(const PartialDataComponentMatcher& component) const;
};
struct Property {
    std::string m_name;
    bool m_isExact;
    std::string m_exactValue, m_minValue, m_maxValue;

    Property() : m_isExact(false) {}
    Property(const std::string& name, const std::string& exact) : m_name(name), m_isExact(true), m_exactValue(exact) {}
    Property(const std::string& name, const std::string& minValue, const std::string& maxValue) 
        : m_name(name), m_isExact(true), m_minValue(minValue), m_maxValue(maxValue) {}
    
    std::vector<char> toBytes() const;

    bool operator==(const Property& property) const;
    bool operator!=(const Property& property) const;
};
struct FireworkExplosion {
    Identifier m_shape;
    std::vector<int> m_colors;
    std::vector<int> m_fadeColors;
    bool m_hasTrail;
    bool m_hasTwinkle;

    FireworkExplosion() : m_hasTrail(false), m_hasTwinkle(false) {}
    FireworkExplosion(const Identifier& shape, const std::vector<int>& colors, const std::vector<int>& fadeColors,
                      const bool& hasTrail, const bool& hasTwinkle) 
        : m_shape(shape), m_colors(colors), m_fadeColors(fadeColors), m_hasTrail(hasTrail), m_hasTwinkle(hasTwinkle) {}
    
    std::vector<char> toBytes() const;

    bool operator==(const FireworkExplosion& fireworkExplosion) const;
    bool operator!=(const FireworkExplosion& fireworkExplosion) const;
};
struct PotionEffectDetail {
    int m_amplifier;
    int m_duration;
    bool m_ambient;
    bool m_showParticles;
    bool m_showIcon;
    std::vector<PotionEffectDetail> m_hiddenEffect;

    PotionEffectDetail() : m_amplifier(1), m_duration(30), m_ambient(false), m_showParticles(false), m_showIcon(false) {}
    PotionEffectDetail(const int& amplifier, const int& duration, const bool& ambient, const bool& showParticles, const bool& showIcon) 
        : m_amplifier(amplifier), m_duration(duration), m_ambient(ambient), m_showParticles(showParticles), m_showIcon(showIcon) {}
    PotionEffectDetail(const int& amplifier, const int& duration, const bool& ambient, const bool& showParticles, const bool& showIcon,
                        const std::vector<PotionEffectDetail>& hiddenEffect) 
        : m_amplifier(amplifier), m_duration(duration), m_ambient(ambient), m_showParticles(showParticles), m_showIcon(showIcon), 
            m_hiddenEffect(hiddenEffect) {}

    std::vector<char> toBytes() const;

    bool operator==(const PotionEffectDetail& detail) const;
    bool operator!=(const PotionEffectDetail& detail) const;
};
struct PotionEffect {
    int m_type;
    PotionEffectDetail m_detail;

    PotionEffect() : m_type(-1) {}
    PotionEffect(const int& type, const PotionEffectDetail& detail) : m_type(type), m_detail(detail) {}

    std::vector<char> toBytes() const;

    bool operator==(const PotionEffect& effect) const;
    bool operator!=(const PotionEffect& effect) const;
};
struct TrimMaterial {
    std::string m_suffix;
    std::vector<std::vector<std::string>> m_overrides;
    TextComponent m_description;

    TrimMaterial() {}
    TrimMaterial(const std::string& suffix, const std::vector<std::vector<std::string>>& overrides, const TextComponent& description)
        : m_suffix(suffix), m_overrides(overrides), m_description(description) {}

    std::vector<char> toBytes() const;

    bool operator==(const TrimMaterial& material) const;
    bool operator!=(const TrimMaterial& material) const;
};
struct TrimPattern {
    std::string m_asset;
    int m_templateItem;
    TextComponent m_description;
    bool m_decal;

    TrimPattern() : m_templateItem(-1), m_decal(false) {}
    TrimPattern(const std::string& asset, const int& templateItem, const TextComponent& description, const bool& decal)
        : m_asset(asset), m_templateItem(templateItem), m_description(description), m_decal(decal) {}

    std::vector<char> toBytes() const;

    bool operator==(const TrimPattern& pattern) const;
    bool operator!=(const TrimPattern& pattern) const;
};
struct ConsumeEffect {
    Identifier m_type;
    std::vector<PotionEffect> m_effects;
    float m_probability;
    IDSet m_effectsRemove;
    float m_diameter;
    SoundEvent m_sound;
    std::vector<char> m_customData;

    ConsumeEffect() : m_type("minecraft:clear_effects") {}
    ConsumeEffect(const std::vector<PotionEffect>& effects, const float& probability) 
        : m_type("minecraft:apply_effects"), m_effects(effects), m_probability(probability) {}
    ConsumeEffect(const IDSet& effects) : m_type("minecraft:remove_effects"), m_effectsRemove(effects) {}
    ConsumeEffect(const float& diameter) : m_type("minecraft:teleport_randomly"), m_diameter(diameter) {}
    ConsumeEffect(const SoundEvent& sound) : m_type("minecraft:play_sound"), m_sound(sound) {}
    ConsumeEffect(const std::string& type, const std::vector<char>& data) : m_type(type), m_customData(data) {}

    std::vector<char> toBytes() const;

    bool operator==(const ConsumeEffect& effect) const;
    bool operator!=(const ConsumeEffect& effect) const;
};
struct Instrument {
    IDorX<SoundEvent> m_soundEvent;
    float m_soundRange;
    float m_range;
    TextComponent m_description;

    Instrument() : m_soundRange(-1), m_range(-1) {}
    Instrument(const IDorX<SoundEvent>& soundEvent, const float& soundRange, const float& range, const TextComponent& description)
        : m_soundEvent(soundEvent), m_soundRange(soundRange), m_range(range), m_description(description) {}

    std::vector<char> toBytes() const;

    bool operator==(const Instrument& instrument) const;
    bool operator!=(const Instrument& instrument) const;
};
struct JukeBox {
    IDorX<SoundEvent> m_soundEvent;
    TextComponent m_description;
    float m_duration;
    int m_output;

    JukeBox() : m_duration(-1), m_output(-1) {}
    JukeBox(const IDorX<SoundEvent>& soundEvent, const TextComponent& description, const float& duration, const int& output)
        : m_soundEvent(soundEvent), m_description(description), m_duration(duration), m_output(output) {}

    std::vector<char> toBytes() const;

    bool operator==(const JukeBox& jukeBox) const;
    bool operator!=(const JukeBox& jukeBox) const;
};
struct BlockPredicate {
    std::optional<IDSet> m_blocks;
    std::optional<std::vector<Property>> m_properties;
    std::optional<NBTElement> m_NBT;
    std::vector<ExactDataComponentMatcher> m_dataComponents;
    std::vector<PartialDataComponentMatcher> m_partialDataComponents;

    BlockPredicate() {}
    BlockPredicate(const std::optional<IDSet>& blocks, const std::optional<std::vector<Property>>& properties, const std::optional<NBTElement>& NBT,
                   const std::vector<ExactDataComponentMatcher>& dataComponents, const std::vector<PartialDataComponentMatcher>& partialDataComponents)
        : m_blocks(blocks), m_properties(properties), m_NBT(NBT), m_dataComponents(dataComponents), m_partialDataComponents(partialDataComponents) {}

    std::vector<char> toBytes() const;

    bool operator==(const BlockPredicate& predicate) const;
    bool operator!=(const BlockPredicate& predicate) const;
};

}