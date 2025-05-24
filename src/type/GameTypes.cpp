#include <type/GameTypes.h>
#include <type/ByteBuffer.h>
#include <registry/DefaultRegistries.h>

namespace zinc {

std::vector<char> PartialDataComponentMatcher::toBytes() const {
    ByteBuffer buffer;
    buffer.writeVarNumeric<int>(m_type);
    buffer.writeNBTElement(m_predicate);
    return buffer.getBytes();
}
bool PartialDataComponentMatcher::operator==(const PartialDataComponentMatcher& component) const {
    return toBytes() == component.toBytes();
}
bool PartialDataComponentMatcher::operator!=(const PartialDataComponentMatcher& component) const {
    return !operator==(component);
}

std::vector<char> Property::toBytes() const {
    ByteBuffer buffer;
    buffer.writeString(m_name);
    buffer.writeByte(m_isExact);
    if (m_isExact) buffer.writeString(m_exactValue);
    else {
        buffer.writeString(m_minValue);
        buffer.writeString(m_maxValue);
    }
    return buffer.getBytes();
}
bool Property::operator==(const Property& property) const {
    return toBytes() == property.toBytes();
}
bool Property::operator!=(const Property& property) const {
    return !operator==(property);
}

std::vector<char> FireworkExplosion::toBytes() const {
    ByteBuffer buffer;
    if (g_fireworkExplosionShapesRegistry.m_registryData.contains(m_shape.toString())) 
        buffer.writeVarNumeric<int>(g_fireworkExplosionShapesRegistry.m_registryData[m_shape.toString()]);
    else buffer.writeVarNumeric<int>(0);
    buffer.writePrefixedArray<int>(m_colors, &ByteBuffer::writeNumeric<int>);
    buffer.writePrefixedArray<int>(m_fadeColors, &ByteBuffer::writeNumeric<int>);
    buffer.writeByte(m_hasTrail);
    buffer.writeByte(m_hasTwinkle);
    return buffer.getBytes();
}
bool FireworkExplosion::operator==(const FireworkExplosion& fireworkExplosion) const {
    return toBytes() == fireworkExplosion.toBytes();
}
bool FireworkExplosion::operator!=(const FireworkExplosion& fireworkExplosion) const {
    return !operator==(fireworkExplosion);
}

std::vector<char> PotionEffectDetail::toBytes() const {
    ByteBuffer buffer;
    buffer.writeVarNumeric<int>(m_amplifier);
    buffer.writeVarNumeric<int>(m_duration);
    buffer.writeByte(m_ambient);
    buffer.writeByte(m_showParticles);
    buffer.writeByte(m_showIcon);
    buffer.writeByte(!m_hiddenEffect.empty());
    if (m_hiddenEffect.size()) buffer.writeByteArray(m_hiddenEffect[0].toBytes());
    return buffer.getBytes();
}
bool PotionEffectDetail::operator==(const PotionEffectDetail& detail) const {
    return toBytes() == detail.toBytes();
}
bool PotionEffectDetail::operator!=(const PotionEffectDetail& detail) const {
    return !operator==(detail);
}

std::vector<char> PotionEffect::toBytes() const {
    ByteBuffer buffer;
    buffer.writeVarNumeric<int>(m_type);
    buffer.writeByteArray(m_detail.toBytes());
    return buffer.getBytes();
}
bool PotionEffect::operator==(const PotionEffect& effect) const {
    return toBytes() == effect.toBytes();
}
bool PotionEffect::operator!=(const PotionEffect& effect) const {
    return !operator==(effect);
}

std::vector<char> TrimMaterial::toBytes() const {
    ByteBuffer buffer;
    buffer.writeString(m_suffix);
    buffer.writeVarNumeric<int>(m_overrides.size());
    for (const std::vector<std::string>& overrideValue : m_overrides) {
        if (overrideValue.size() != 2) continue;
        buffer.writeIdentifier(overrideValue[0]);
        buffer.writeString(overrideValue[1]);
    }
    buffer.writeTextComponent(m_description);
    return buffer.getBytes();
}
bool TrimMaterial::operator==(const TrimMaterial& material) const {
    return toBytes() == material.toBytes();
}
bool TrimMaterial::operator!=(const TrimMaterial& material) const {
    return !operator==(material);
}

std::vector<char> TrimPattern::toBytes() const {
    ByteBuffer buffer;
    buffer.writeString(m_asset);
    buffer.writeVarNumeric<int>(m_templateItem);
    buffer.writeTextComponent(m_description);
    buffer.writeByte(m_decal);
    return buffer.getBytes();
}
bool TrimPattern::operator==(const TrimPattern& pattern) const {
    return toBytes() == pattern.toBytes();
}
bool TrimPattern::operator!=(const TrimPattern& pattern) const {
    return !operator==(pattern);
}

std::vector<char> ConsumeEffect::toBytes() const {
    ByteBuffer buffer;
    ConsumeEffectData args;
    args.m_customData = m_customData;
    args.m_effects = m_effects;
    args.m_effectsRemove = m_effectsRemove;
    args.m_sound = m_sound;
    args.m_probability = m_probability;
    args.m_diameter = m_diameter;
    buffer.writeVarNumeric<int>(g_consumeEffectRegistry.m_registryData[m_type.toString()]);
    buffer.writeByteArray(g_consumeEffectRegistry.m_writers[m_type.toString()](args));
    return buffer.getBytes();
}
bool ConsumeEffect::operator==(const ConsumeEffect& effect) const {
    return toBytes() == effect.toBytes();
}
bool ConsumeEffect::operator!=(const ConsumeEffect& effect) const {
    return !operator==(effect);
}

std::vector<char> Instrument::toBytes() const {
    ByteBuffer buffer;
    buffer.writeIDorX<SoundEvent>(m_soundEvent, &ByteBuffer::writeSoundEvent);
    buffer.writeNumeric<float>(m_soundRange);
    buffer.writeNumeric<float>(m_range);
    buffer.writeTextComponent(m_description);
    return buffer.getBytes();
}
bool Instrument::operator==(const Instrument& instrument) const {
    return toBytes() == instrument.toBytes();
}
bool Instrument::operator!=(const Instrument& instrument) const {
    return !operator==(instrument);
}

std::vector<char> JukeBox::toBytes() const {
    ByteBuffer buffer;
    buffer.writeIDorX<SoundEvent>(m_soundEvent, &ByteBuffer::writeSoundEvent);
    buffer.writeTextComponent(m_description);
    buffer.writeNumeric<float>(m_duration);
    buffer.writeVarNumeric<int>(m_output);
    return buffer.getBytes();
}
bool JukeBox::operator==(const JukeBox& jukebox) const {
    return toBytes() == jukebox.toBytes();
}
bool JukeBox::operator!=(const JukeBox& jukebox) const {
    return !operator==(jukebox);
}

std::vector<char> BlockPredicate::toBytes() const {
    ByteBuffer buffer;
    buffer.writePrefixedOptional<IDSet>(m_blocks, &ByteBuffer::writeIDSet);
    buffer.writeByte(m_properties.has_value());
    if (m_properties.has_value()) {
        buffer.writeVarNumeric<int>(m_properties.value().size());
        for (const Property& property : m_properties.value()) buffer.writeByteArray(property.toBytes());
    }
    buffer.writePrefixedOptional<NBTElement>(m_NBT, &ByteBuffer::writeNBTElement);
    buffer.writeVarNumeric<int>(m_dataComponents.size());
    for (const ExactDataComponentMatcher& component : m_dataComponents) buffer.writeByteArray(component);
    buffer.writeVarNumeric<int>(m_partialDataComponents.size());
    for (const PartialDataComponentMatcher& component : m_partialDataComponents) buffer.writeByteArray(component.toBytes());
    return buffer.getBytes();
}
bool BlockPredicate::operator==(const BlockPredicate& predicate) const {
    return toBytes() == predicate.toBytes();
}
bool BlockPredicate::operator!=(const BlockPredicate& predicate) const {
    return !operator==(predicate);
}


}