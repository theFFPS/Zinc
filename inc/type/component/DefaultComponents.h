#pragma once

#include "../ByteBuffer.h"

namespace zinc {

enum class Rarity : int { Common, Uncommon, Rare, Epic };
enum class AttributeModifiersOperation : int { Add, MultiplyBase, MultiplyTotal };
enum class SlotType : int { Any, MainHand, OffHand, Hand, Feet, Legs, Chest, Head, Armor, Body };
enum class EquippableSlotType : int { MainHand, Feet, Legs, Chest, Head, OffHand, Body };
enum class AnimationType : int { None, Eat, Drink, Block, Bow, Spear, Crossbow, Spyglass, TootHorn, Brush };
enum class MapPostProcessingType : int { Lock, Scale };
enum class MooshroomVariants : int { Red, Brown };
enum class RabbitVariants : int { Brown, White, Black, WhiteSplotched, Gold, Salt, Evil };
enum class ComponentTypes : int {
    CustomData, MaxStackSize, MaxDamage, Damage, Unbreakable, CustomName, ItemName, ItemModel, Lore, Rarity, Enchantments,
    CanPlaceOn, CanBreak, AttributeModifiers, CustomModelData, TooltipDisplay, RepairCost, CreativeSlotLock,
    EnchantmentGlintOverride, IntanglibleProjectile, Food, Consumable, UseRemainder, UseCooldown, DamageResistant,
    Tool, Weapon, Enchantable, Equippable, Repairable, Glider, TooltipStyle, DeathProtection, BlocksAttacks,
    StoredEnchantments, DyedColor, MapColor, MapId, MapDecorations, MapPostProcessing, ChargedProjectiles, BundleContents,
    PotionContents, PotionDurationScale, SuspiciousStewEffects, WritableBookContent, WrittenBookContent, Trim, DebugStickState,
    EntityData, BucketEntityData, BlockEntityData, Instrument, ProvidesTrimMaterial, OminousBottleAmplifier, JukeboxPlayable,
    ProvidesBannerPatterns, Recipes, LodestoneTracker, FireworkExplosion, Fireworks, Profile, NoteBlockSound, BannerPatterns,
    BaseColor, PotDecorations, Container, BlockState, Bees, Lock, ContainerLoot, BreakSound, VillagerVariant, WolfVariant,
    WolfSoundVariant, WolfCollar, FoxVariant, SalmonSize, ParrotVariant, TropicalFishPattern, TropicalFishBaseColor, TropicalFishPatternColor,
    MooshroomVariant, RabbitVariant, PigVariant, CowVariant, ChickenVariant, FrogVariant, HorseVariant, PaintingVariant, LlamaVariant,
    AxolotlVariant, CatVariant, CatCollar, SheepColor, ShulkerColor
};

struct CustomDataComponent {
private:
    NBTElement m_data;
public:
    CustomDataComponent() : m_data(NBTElement::Compound({})) {}
    CustomDataComponent(const NBTElement& data) : m_data(data) {}

    inline ByteBuffer encode() {
        ByteBuffer buffer;
        buffer.writeVarInt((int) ComponentTypes::CustomData);
        buffer.writeNBTElement(m_data);
        return buffer;
    }
    inline NBTElement toNBT() {
        NBTElement result = m_data;
        result.setTag("minecraft:custom_data");
        return result;
    }
};
struct MaxStackSizeComponent {
private:
    int m_maxStackSize;
public:
    MaxStackSizeComponent() : m_maxStackSize(64) {}
    MaxStackSizeComponent(const int& maxStackSize) : m_maxStackSize(maxStackSize) {}

    inline ByteBuffer encode() {
        ByteBuffer buffer;
        buffer.writeVarInt((int) ComponentTypes::MaxStackSize);
        buffer.writeVarInt(m_maxStackSize);
        return buffer;
    }
};
struct MaxDamageComponent {
private:
    int m_maxDamage;
public:
    MaxDamageComponent() : m_maxDamage(0) {}
    MaxDamageComponent(const int& maxDamage) : m_maxDamage(maxDamage) {}

    inline ByteBuffer encode() {
        ByteBuffer buffer;
        buffer.writeVarInt((int) ComponentTypes::MaxDamage);
        buffer.writeVarInt(m_maxDamage);
        return buffer;
    }
};
struct DamageComponent {
private:
    int m_damage;
public:
    DamageComponent() : m_damage(0) {}
    DamageComponent(const int& damage) : m_damage(m_damage) {}

    inline ByteBuffer encode() {
        ByteBuffer buffer;
        buffer.writeVarInt((int) ComponentTypes::Damage);
        buffer.writeVarInt(m_damage);
        return buffer;
    }
};
struct UnbreakableComponent {
    inline ByteBuffer encode() {
        ByteBuffer buffer;
        buffer.writeVarInt((int) ComponentTypes::Unbreakable);
        return buffer;
    }
};
struct CustomNameComponent {
private:
    TextComponent m_name;
public:
    CustomNameComponent() : m_name(TextComponent()) {}
    CustomNameComponent(const TextComponent& name) : m_name(name) {}

    inline ByteBuffer encode() {
        ByteBuffer buffer;
        buffer.writeVarInt((int) ComponentTypes::CustomName);
        buffer.writeTextComponent(m_name);
        return buffer;
    }
};
struct ItemNameComponent {
private:
    TextComponent m_name;
public:
    ItemNameComponent() : m_name(TextComponent()) {}
    ItemNameComponent(const TextComponent& name) : m_name(name) {}

    inline ByteBuffer encode() {
        ByteBuffer buffer;
        buffer.writeVarInt((int) ComponentTypes::ItemName);
        buffer.writeTextComponent(m_name);
        return buffer;
    }
};
struct ItemModelComponent {
private:
    Identifier m_model;
public:
    ItemModelComponent() : m_model(Identifier()) {}
    ItemModelComponent(const Identifier& model) : m_model(model) {}

    inline ByteBuffer encode() {
        ByteBuffer buffer;
        buffer.writeVarInt((int) ComponentTypes::ItemModel);
        buffer.writeIdentifier(m_model);
        return buffer;
    }
};

}