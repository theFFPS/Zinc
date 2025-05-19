#include <registry/DefaultRegistries.h>
#include <type/ByteBuffer.h>

namespace zinc {

std::map<std::string, int> g_fireworkExplosionShapes = {
    { "minecraft:small", 0 },
    { "minecraft:large", 1 },
    { "minecraft:star", 2 },
    { "minecraft:creeper", 3 },
    { "minecraft:burst", 4 }
};
std::string getFireworkExplosionShapeId(const int& shape) {
    for (const auto& shapePair : g_fireworkExplosionShapes) if (shape == shapePair.second) return shapePair.first;
    return "minecraft:small";
}

std::map<std::string, int> g_dyeColors = {
    { "minecraft:white", 0 },
    { "minecraft:orange", 1 },
    { "minecraft:magenta", 2 },
    { "minecraft:light_blue", 3 },
    { "minecraft:yellow", 4 },
    { "minecraft:lime", 5 },
    { "minecraft:pink", 6 },
    { "minecraft:gray", 7 },
    { "minecraft:light_gray", 8 },
    { "minecraft:cyan", 9 },
    { "minecraft:purple", 10 },
    { "minecraft:blue", 11 },
    { "minecraft:brown", 12 },
    { "minecraft:green", 13 },
    { "minecraft:red", 14 },
    { "minecraft:black", 15 }
};
std::string getDyeColorId(const int& color) {
    for (const auto& dyeColorPair : g_dyeColors) if (color == dyeColorPair.second) return dyeColorPair.first;
    return "minecraft:white";
}

std::map<std::string, int> g_consumeEffectTypes = {
    { "minecraft:apply_effects", 0 },
    { "minecraft:remove_effects", 1 },
    { "minecraft:clear_effects", 2 },
    { "minecraft:teleport_randomly", 3 },
    { "minecraft:play_sound", 4 }
};
std::string getConsumeEffectTypeId(const int& type) {
    for (const auto& effectPair : g_consumeEffectTypes) if (type == effectPair.second) return effectPair.first;
    return "minecraft:clear_effects";
}
std::vector<char> writeApplyEffects(const ConsumeEffectData& args) {
    ByteBuffer buffer;
    for (const PotionEffect& effect : args.m_effects) buffer.writeByteArray(effect.toBytes());
    buffer.writeNumeric<float>(args.m_probability);
    return buffer.getBytes();
}
std::vector<char> writeRemoveEffects(const ConsumeEffectData& args) {
    ByteBuffer buffer;
    buffer.writeIDSet(args.m_effectsRemove);
    return buffer.getBytes();
}
std::vector<char> writeClearEffects(const ConsumeEffectData&) { return {}; }
std::vector<char> writeTeleportRandomly(const ConsumeEffectData& args) {
    ByteBuffer buffer;
    buffer.writeNumeric<float>(args.m_diameter);
    return buffer.getBytes();
}
std::vector<char> writePlaySound(const ConsumeEffectData& args) {
    ByteBuffer buffer;
    buffer.writeSoundEvent(args.m_sound);
    return buffer.getBytes();
}
std::map<std::string, std::vector<char>(*)(const ConsumeEffectData&)> g_consumeEffectWriters = {
    { "minecraft:apply_effects", writeApplyEffects },
    { "minecraft:remove_effects", writeRemoveEffects },
    { "minecraft:clear_effects", writeClearEffects },
    { "minecraft:teleport_randomly", writeTeleportRandomly },
    { "minecraft:play_sound", writePlaySound }
};
ConsumeEffectData readApplyEffects(ByteBuffer&) { return {}; }
ConsumeEffectData readRemoveEffects(ByteBuffer& data) {
    ConsumeEffectData result;
    result.m_effectsRemove = data.readIDSet();
    return result;
}
ConsumeEffectData readClearEffects(ByteBuffer&) { return {}; }
ConsumeEffectData readTeleportRandomly(ByteBuffer& data) {
    ConsumeEffectData result;
    result.m_diameter = data.readNumeric<float>();
    return result;
}
ConsumeEffectData readPlaySound(ByteBuffer& data) {
    ConsumeEffectData result;
    result.m_sound = data.readSoundEvent();
    return result;
}
std::map<std::string, ConsumeEffectData(*)(ByteBuffer&)> g_consumeEffectReaders = {
    { "minecraft:apply_effects", readApplyEffects },
    { "minecraft:remove_effects", readRemoveEffects },
    { "minecraft:clear_effects", readClearEffects },
    { "minecraft:teleport_randomly", readTeleportRandomly },
    { "minecraft:play_sound", readPlaySound }
};

std::map<std::string, int> g_recipeDisplayTypes = {
    { "minecraft:crafting_shapeless", 0 },
    { "minecraft:crafting_shaped", 1 },
    { "minecraft:furnace", 2 },
    { "minecraft:stonecutter", 3 },
    { "minecraft:smithing", 4 }
};
std::string getRecipeDisplayTypeId(const int& type) {
    for (const auto& pair : g_recipeDisplayTypes) if (type == pair.second) return pair.first;
    return "minecraft:crafting_shapeless";
}
std::vector<char> writeShapeless(const RecipeDisplayData& data) {
    ByteBuffer buffer;
    buffer.writePrefixedArray<SlotDisplay>(data.m_ingredients, &ByteBuffer::writeSlotDisplay);
    buffer.writeSlotDisplay(data.m_result);
    buffer.writeSlotDisplay(data.m_craftingStation);
    return buffer.getBytes();
}
std::vector<char> writeShaped(const RecipeDisplayData& data) {
    ByteBuffer buffer;
    buffer.writeVarNumeric<int>(data.m_width);
    buffer.writeVarNumeric<int>(data.m_height);
    buffer.writePrefixedArray<SlotDisplay>(data.m_ingredients, &ByteBuffer::writeSlotDisplay);
    buffer.writeSlotDisplay(data.m_result);
    buffer.writeSlotDisplay(data.m_craftingStation);
    return buffer.getBytes();
}
std::vector<char> writeFurnace(const RecipeDisplayData& data) {
    ByteBuffer buffer;
    if (data.m_ingredients.size() != 1) {
        buffer.writeSlotDisplay(SlotDisplay());
    } else buffer.writeSlotDisplay(data.m_ingredients[0]);
    buffer.writeSlotDisplay(data.m_fuel);
    buffer.writeSlotDisplay(data.m_result);
    buffer.writeSlotDisplay(data.m_craftingStation);
    buffer.writeVarNumeric<int>(data.m_cookingTime);
    buffer.writeNumeric<float>(data.m_experience);
    return buffer.getBytes();
}
std::vector<char> writeStoneCutter(const RecipeDisplayData& data) {
    ByteBuffer buffer;
    if (data.m_ingredients.size() != 1) {
        buffer.writeSlotDisplay(SlotDisplay());
    } else buffer.writeSlotDisplay(data.m_ingredients[0]);
    buffer.writeSlotDisplay(data.m_result);
    buffer.writeSlotDisplay(data.m_craftingStation);
    return buffer.getBytes();
}
std::vector<char> writeSmithing(const RecipeDisplayData& data) {
    ByteBuffer buffer;
    if (data.m_ingredients.size() != 3) {
        buffer.writeSlotDisplay(SlotDisplay());
        buffer.writeSlotDisplay(SlotDisplay());
        buffer.writeSlotDisplay(SlotDisplay());
    } else {
        buffer.writeSlotDisplay(data.m_ingredients[0]);
        buffer.writeSlotDisplay(data.m_ingredients[1]);
        buffer.writeSlotDisplay(data.m_ingredients[2]);
    }
    buffer.writeSlotDisplay(data.m_result);
    buffer.writeSlotDisplay(data.m_craftingStation);
    return buffer.getBytes();
}
std::map<std::string, std::vector<char>(*)(const RecipeDisplayData&)> g_recipeDisplayWriters = {
    { "minecraft:crafting_shapeless", writeShapeless },
    { "minecraft:crafting_shaped", writeShaped },
    { "minecraft:furnace", writeFurnace },
    { "minecraft:stonecutter", writeStoneCutter },
    { "minecraft:smithing", writeSmithing }
};
RecipeDisplayData readShapeless(ByteBuffer& buffer) {
    RecipeDisplayData data;
    data.m_ingredients = buffer.readPrefixedArray<SlotDisplay>(&ByteBuffer::readSlotDisplay);
    data.m_result = buffer.readSlotDisplay();
    data.m_craftingStation = buffer.readSlotDisplay();
    return data;
}
RecipeDisplayData readShaped(ByteBuffer& buffer) {
    RecipeDisplayData data;
    data.m_width = buffer.readVarNumeric<int>();
    data.m_height = buffer.readVarNumeric<int>();
    data.m_ingredients = buffer.readPrefixedArray<SlotDisplay>(&ByteBuffer::readSlotDisplay);
    data.m_result = buffer.readSlotDisplay();
    data.m_craftingStation = buffer.readSlotDisplay();
    return data;
}
RecipeDisplayData readFurnace(ByteBuffer& buffer) {
    RecipeDisplayData data;
    data.m_ingredients = { buffer.readSlotDisplay() };
    data.m_fuel = buffer.readSlotDisplay();
    data.m_result = buffer.readSlotDisplay();
    data.m_craftingStation = buffer.readSlotDisplay();
    data.m_cookingTime = buffer.readVarNumeric<int>();
    data.m_experience = buffer.readNumeric<float>();
    return data;
}
RecipeDisplayData readStoneCutter(ByteBuffer& buffer) {
    RecipeDisplayData data;
    data.m_ingredients = { buffer.readSlotDisplay() };
    data.m_result = buffer.readSlotDisplay();
    data.m_craftingStation = buffer.readSlotDisplay();
    return data;
}
RecipeDisplayData readSmithing(ByteBuffer& buffer) {
    RecipeDisplayData data;
    data.m_ingredients = { buffer.readSlotDisplay() };
    data.m_ingredients.push_back(buffer.readSlotDisplay());
    data.m_ingredients.push_back(buffer.readSlotDisplay());
    data.m_result = buffer.readSlotDisplay();
    data.m_craftingStation = buffer.readSlotDisplay();
    return data;
}
std::map<std::string, RecipeDisplayData(*)(ByteBuffer&)> g_recipeDisplayReaders = {
    { "minecraft:crafting_shapeless", readShapeless },
    { "minecraft:crafting_shaped", readShaped },
    { "minecraft:furnace", readFurnace },
    { "minecraft:stonecutter", readStoneCutter },
    { "minecraft:smithing", readSmithing }
};

std::map<std::string, int> g_slotDisplayTypes = {
    { "minecraft:empty", 0 },
    { "minecraft:any_fuel", 1 },
    { "minecraft:item", 2 },
    { "minecraft:item_stack", 3 },
    { "minecraft:tag", 4 },
    { "minecraft:smithing_trim", 5 },
    { "minecraft:with_remainder", 6 },
    { "minecraft:composite", 7 }
};
std::string getSlotDisplayTypeId(const int& type) {
    for (const auto& pair : g_slotDisplayTypes) if (type == pair.second) return pair.first;
    return "minecraft:empty";
}
/*
switch (m_type) {
    case SlotDisplayType::Item: buffer.writeVarNumeric<int>(m_itemType); break;
    case SlotDisplayType::ItemStack: buffer.writeSlot(m_itemStack); break;
    case SlotDisplayType::Tag: buffer.writeIdentifier(m_tag); break;
    case SlotDisplayType::SmithingTrim: {
        if (m_children.size() != 3) {
            buffer.writeByteArray(SlotDisplay().toBytes());
            buffer.writeByteArray(SlotDisplay().toBytes());
            buffer.writeByteArray(SlotDisplay().toBytes());
        } else {
            buffer.writeByteArray(m_children[0].toBytes());
            buffer.writeByteArray(m_children[1].toBytes());
            buffer.writeByteArray(m_children[2].toBytes());
        }
        break;
    }
    case SlotDisplayType::WithRemainder: {
        if (m_children.size() != 2) {
            buffer.writeByteArray(SlotDisplay().toBytes());
            buffer.writeByteArray(SlotDisplay().toBytes());
        } else {
            buffer.writeByteArray(m_children[0].toBytes());
            buffer.writeByteArray(m_children[1].toBytes());
        }
        break;
    }
    case SlotDisplayType::Composite: {
        buffer.writeVarNumeric<int>(m_children.size());
        for (const SlotDisplay& slotDisplay : m_children) buffer.writeByteArray(slotDisplay.toBytes());
        break;
    }
    default: break;
    }
*/
std::map<std::string, std::vector<char>(*)(const SlotDisplayData&)> g_slotDisplayWriters = {
    // { "minecraft:empty", 0 },
    // { "minecraft:any_fuel", 1 },
    // { "minecraft:item", 2 },
    // { "minecraft:item_stack", 3 },
    // { "minecraft:tag", 4 },
    // { "minecraft:smithing_trim", 5 },
    // { "minecraft:with_remainder", 6 },
    // { "minecraft:composite", 7 }
};
std::map<std::string, SlotDisplayData(*)(ByteBuffer&)> g_slotDisplayReaders = {
    // { "minecraft:empty", 0 },
    // { "minecraft:any_fuel", 1 },
    // { "minecraft:item", 2 },
    // { "minecraft:item_stack", 3 },
    // { "minecraft:tag", 4 },
    // { "minecraft:smithing_trim", 5 },
    // { "minecraft:with_remainder", 6 },
    // { "minecraft:composite", 7 }
};

}