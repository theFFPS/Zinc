#include <type/GameTypes.h>
#include <type/ByteBuffer.h>

namespace zinc {

std::vector<char> PartialDataComponentMatcher::toBytes() const {
    ByteBuffer buffer;
    buffer.writeVarInt(m_type);
    buffer.writeNBTElement(m_predicate);
    return buffer.getBytes();
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
std::vector<char> FireworkExplosion::toBytes() const {
    ByteBuffer buffer;
    buffer.writeVarInt((int) m_shape);
    buffer.writePrefixedArray<int>(m_colors, &ByteBuffer::writeNumeric<int>);
    buffer.writePrefixedArray<int>(m_fadeColors, &ByteBuffer::writeNumeric<int>);
    buffer.writeByte(m_hasTrail);
    buffer.writeByte(m_hasTwinkle);
    return buffer.getBytes();
}
std::vector<char> PotionEffectDetail::toBytes() const {
    ByteBuffer buffer;
    buffer.writeVarInt(m_amplifier);
    buffer.writeVarInt(m_duration);
    buffer.writeByte(m_ambient);
    buffer.writeByte(m_showParticles);
    buffer.writeByte(m_showIcon);
    buffer.writeByte(!m_hiddenEffect.empty());
    if (m_hiddenEffect.size()) buffer.writeByteArray(m_hiddenEffect[0].toBytes());
    return buffer.getBytes();
}
std::vector<char> PotionEffect::toBytes() const {
    ByteBuffer buffer;
    buffer.writeVarInt(m_type);
    buffer.writeByteArray(m_detail.toBytes());
    return buffer.getBytes();
}
std::vector<char> TrimMaterial::toBytes() const {
    ByteBuffer buffer;
    buffer.writeString(m_suffix);
    buffer.writeVarInt(m_overrides.size());
    for (const std::vector<std::string>& overrideValue : m_overrides) {
        if (overrideValue.size() != 2) continue;
        buffer.writeIdentifier(overrideValue[0]);
        buffer.writeString(overrideValue[1]);
    }
    buffer.writeTextComponent(m_description);
    return buffer.getBytes();
}
std::vector<char> TrimPattern::toBytes() const {
    ByteBuffer buffer;
    buffer.writeString(m_asset);
    buffer.writeVarInt(m_templateItem);
    buffer.writeTextComponent(m_description);
    buffer.writeByte(m_decal);
    return buffer.getBytes();
}
std::vector<char> ConsumeEffect::toBytes() const {
    ByteBuffer buffer;
    buffer.writeVarInt((int) m_type);
    switch (m_type) {
    case ConsumeEffectType::PlaySound: buffer.writeSoundEvent(m_sound); break;
    case ConsumeEffectType::TeleportRandomly: buffer.writeNumeric<float>(m_diameter); break;
    case ConsumeEffectType::RemoveEffects: buffer.writeIDSet(m_effectsRemove); break;
    case ConsumeEffectType::ApplyEffects: {
        for (const PotionEffect& effect : m_effects) buffer.writeByteArray(effect.toBytes());
        buffer.writeNumeric<float>(m_probability);
        break;
    }
    default: break;
    }
    return buffer.getBytes();
}
std::vector<char> Instrument::toBytes() const {
    ByteBuffer buffer;
    buffer.writeIDorX<SoundEvent>(m_soundEvent, &ByteBuffer::writeSoundEvent);
    buffer.writeNumeric<float>(m_soundRange);
    buffer.writeNumeric<float>(m_range);
    buffer.writeTextComponent(m_description);
    return buffer.getBytes();
}
std::vector<char> JukeBox::toBytes() const {
    ByteBuffer buffer;
    buffer.writeIDorX<SoundEvent>(m_soundEvent, &ByteBuffer::writeSoundEvent);
    buffer.writeTextComponent(m_description);
    buffer.writeNumeric<float>(m_duration);
    buffer.writeVarInt(m_output);
    return buffer.getBytes();
}
std::vector<char> BlockPredicate::toBytes() const {
    ByteBuffer buffer;
    buffer.writePrefixedOptional<IDSet>(m_blocks, &ByteBuffer::writeIDSet);
    buffer.writeByte(m_properties.has_value());
    if (m_properties.has_value()) {
        buffer.writeVarInt(m_properties.value().size());
        for (const Property& property : m_properties.value()) buffer.writeByteArray(property.toBytes());
    }
    buffer.writePrefixedOptional<NBTElement>(m_NBT, &ByteBuffer::writeNBTElement);
    buffer.writeVarInt(m_dataComponents.size());
    for (const ExactDataComponentMatcher& component : m_dataComponents) buffer.writeByteArray(component);
    buffer.writeVarInt(m_partialDataComponents.size());
    for (const PartialDataComponentMatcher& component : m_partialDataComponents) buffer.writeByteArray(component.toBytes());
    return buffer.getBytes();
}

}