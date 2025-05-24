#include <registry/DefaultRegistries.h>
#include <network/minecraft/channels/DefaultChannels.h>
#include <type/ByteBuffer.h>

namespace zinc {

Registry<char> g_fireworkExplosionShapesRegistry = Registry<char>({
    { "minecraft:small", 0 }, { "minecraft:large", 1 }, { "minecraft:star", 2 }, { "minecraft:creeper", 3 }, { "minecraft:burst", 4 }
}, {}, {}, Identifier("minecraft:small"));
Registry<char> g_dyeColorsRegistry = Registry<char>({
    { "minecraft:white", 0 },      { "minecraft:orange", 1 }, { "minecraft:magenta", 2 },  { "minecraft:light_blue", 3 },
    { "minecraft:yellow", 4 },     { "minecraft:lime", 5 },   { "minecraft:pink", 6 },     { "minecraft:gray", 7 },
    { "minecraft:light_gray", 8 }, { "minecraft:cyan", 9 },   { "minecraft:purple", 10 },  { "minecraft:blue", 11 },
    { "minecraft:brown", 12 },     { "minecraft:green", 13 },     { "minecraft:red", 14 }, { "minecraft:black", 15 }
}, {}, {}, Identifier("minecraft:white"));
Registry<ConsumeEffectData> g_consumeEffectRegistry = Registry<ConsumeEffectData>({ 
    { "minecraft:apply_effects", 0 }, { "minecraft:remove_effects", 1 }, { "minecraft:clear_effects", 2 }, 
    { "minecraft:teleport_randomly", 3 }, { "minecraft:play_sound", 4 } 
}, {
    { "minecraft:apply_effects", [](const ConsumeEffectData& data) {
        ByteBuffer buffer;
        for (const PotionEffect& effect : data.m_effects) buffer.writeByteArray(effect.toBytes());
        buffer.writeNumeric<float>(data.m_probability);
        return buffer.getBytes();
    } }, 
    { "minecraft:remove_effects", [](const ConsumeEffectData& data) 
        { ByteBuffer buffer; buffer.writeIDSet(data.m_effectsRemove); return buffer.getBytes(); } }, 
    { "minecraft:clear_effects", [](const ConsumeEffectData& data) { return ByteBuffer().getBytes(); } }, 
    { "minecraft:teleport_randomly", [](const ConsumeEffectData& data) 
        { ByteBuffer buffer; buffer.writeNumeric<float>(data.m_diameter); return buffer.getBytes(); } }, 
    { "minecraft:play_sound", [](const ConsumeEffectData& data) { ByteBuffer buffer; buffer.writeSoundEvent(data.m_sound); return buffer.getBytes(); } } 
}, {
    { "minecraft:apply_effects", [](ByteBuffer&) { return ConsumeEffectData(); } }, 
    { "minecraft:remove_effects", [](ByteBuffer& data) { ConsumeEffectData result; result.m_effectsRemove = data.readIDSet(); return result; } }, 
    { "minecraft:clear_effects", [](ByteBuffer&) { return ConsumeEffectData(); } }, 
    { "minecraft:teleport_randomly", [](ByteBuffer& data) { ConsumeEffectData result; result.m_diameter = data.readNumeric<float>(); return result; } }, 
    { "minecraft:play_sound", [](ByteBuffer& data) { ConsumeEffectData result; result.m_sound = data.readSoundEvent(); return result; } } 
}, Identifier("minecraft:clear_effects"));
Registry<RecipeDisplayData> g_recipeDisplayRegistry = Registry<RecipeDisplayData>({ 
    { "minecraft:crafting_shapeless", 0 }, { "minecraft:crafting_shaped", 1 }, { "minecraft:furnace", 2 },
    { "minecraft:stonecutter", 3 },        { "minecraft:smithing", 4 }
}, {
    { "minecraft:crafting_shapeless", [](const RecipeDisplayData& data) {
        ByteBuffer buffer;
        buffer.writePrefixedArray<SlotDisplay>(data.m_ingredients, &ByteBuffer::writeSlotDisplay);
        buffer.writeSlotDisplay(data.m_result);
        buffer.writeSlotDisplay(data.m_craftingStation);
        return buffer.getBytes();
    } }, 
    { "minecraft:crafting_shaped", [](const RecipeDisplayData& data) {
        ByteBuffer buffer;
        buffer.writeVarNumeric<int>(data.m_width);
        buffer.writeVarNumeric<int>(data.m_height);
        buffer.writePrefixedArray<SlotDisplay>(data.m_ingredients, &ByteBuffer::writeSlotDisplay);
        buffer.writeSlotDisplay(data.m_result);
        buffer.writeSlotDisplay(data.m_craftingStation);
        return buffer.getBytes();
    } }, 
    { "minecraft:furnace", [](const RecipeDisplayData& data) {
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
    } }, 
    { "minecraft:stonecutter", [](const RecipeDisplayData& data) {
        ByteBuffer buffer;
        if (data.m_ingredients.size() != 1) {
            buffer.writeSlotDisplay(SlotDisplay());
        } else buffer.writeSlotDisplay(data.m_ingredients[0]);
        buffer.writeSlotDisplay(data.m_result);
        buffer.writeSlotDisplay(data.m_craftingStation);
        return buffer.getBytes();
    } }, 
    { "minecraft:smithing", [](const RecipeDisplayData& data) {
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
    } }, 
}, {
    { "minecraft:crafting_shapeless", [](ByteBuffer& buffer) {
        RecipeDisplayData data;
        data.m_ingredients = buffer.readPrefixedArray<SlotDisplay>(&ByteBuffer::readSlotDisplay);
        data.m_result = buffer.readSlotDisplay();
        data.m_craftingStation = buffer.readSlotDisplay();
        return data;
    } }, 
    { "minecraft:crafting_shaped", [](ByteBuffer& buffer) {
        RecipeDisplayData data;
        data.m_width = buffer.readVarNumeric<int>();
        data.m_height = buffer.readVarNumeric<int>();
        data.m_ingredients = buffer.readPrefixedArray<SlotDisplay>(&ByteBuffer::readSlotDisplay);
        data.m_result = buffer.readSlotDisplay();
        data.m_craftingStation = buffer.readSlotDisplay();
        return data;
    } }, 
    { "minecraft:furnace", [](ByteBuffer& buffer) {
        RecipeDisplayData data;
        data.m_ingredients = { buffer.readSlotDisplay() };
        data.m_fuel = buffer.readSlotDisplay();
        data.m_result = buffer.readSlotDisplay();
        data.m_craftingStation = buffer.readSlotDisplay();
        data.m_cookingTime = buffer.readVarNumeric<int>();
        data.m_experience = buffer.readNumeric<float>();
        return data;
    } }, 
    { "minecraft:stonecutter", [](ByteBuffer& buffer) {
        RecipeDisplayData data;
        data.m_ingredients = { buffer.readSlotDisplay() };
        data.m_result = buffer.readSlotDisplay();
        data.m_craftingStation = buffer.readSlotDisplay();
        return data;
    } }, 
    { "minecraft:smithing", [](ByteBuffer& buffer) {
        RecipeDisplayData data;
        data.m_ingredients = { buffer.readSlotDisplay() };
        data.m_ingredients.push_back(buffer.readSlotDisplay());
        data.m_ingredients.push_back(buffer.readSlotDisplay());
        data.m_result = buffer.readSlotDisplay();
        data.m_craftingStation = buffer.readSlotDisplay();
        return data;
    } }, 
}, Identifier("minecraft:crafting_shapeless"));
Registry<SlotDisplayData> g_slotDisplayRegistry = Registry<SlotDisplayData>({ 
    { "minecraft:empty", 0 }, { "minecraft:any_fuel", 1 },      { "minecraft:item", 2 },           { "minecraft:item_stack", 3 },
    { "minecraft:tag", 4 },   { "minecraft:smithing_trim", 5 }, { "minecraft:with_remainder", 6 }, { "minecraft:composite", 7 }
}, {
    { "minecraft:empty", [](const SlotDisplayData& data) { return ByteBuffer().getBytes(); } },
    { "minecraft:any_fuel", [](const SlotDisplayData& data) { return ByteBuffer().getBytes(); } },
    { "minecraft:item", [](const SlotDisplayData& data) {
        ByteBuffer buffer;
        buffer.writeVarNumeric<int>(data.m_itemType);
        return buffer.getBytes();
    } }, 
    { "minecraft:item_stack", [](const SlotDisplayData& data) {
        ByteBuffer buffer;
        buffer.writeSlot(data.m_itemStack);
        return buffer.getBytes();
    } }, 
    { "minecraft:tag", [](const SlotDisplayData& data) {
        ByteBuffer buffer;
        buffer.writeIdentifier(data.m_tag);
        return buffer.getBytes();
    } }, 
    { "minecraft:smithing_trim", [](const SlotDisplayData& data) {
        ByteBuffer buffer;
        if (data.m_children.size() != 3) {
            buffer.writeByteArray(SlotDisplay().toBytes());
            buffer.writeByteArray(SlotDisplay().toBytes());
            buffer.writeByteArray(SlotDisplay().toBytes());
        } else {
            buffer.writeByteArray(data.m_children[0].toBytes());
            buffer.writeByteArray(data.m_children[1].toBytes());
            buffer.writeByteArray(data.m_children[2].toBytes());
        }
        return buffer.getBytes();
    } }, 
    { "minecraft:with_remainder", [](const SlotDisplayData& data) {
        ByteBuffer buffer;
        if (data.m_children.size() != 2) {
            buffer.writeByteArray(SlotDisplay().toBytes());
            buffer.writeByteArray(SlotDisplay().toBytes());
        } else {
            buffer.writeByteArray(data.m_children[0].toBytes());
            buffer.writeByteArray(data.m_children[1].toBytes());
        }
        return buffer.getBytes();
    } }, 
    { "minecraft:composite", [](const SlotDisplayData& data) {
        ByteBuffer buffer;
        buffer.writePrefixedArray<SlotDisplay>(data.m_children, &ByteBuffer::writeSlotDisplay);
        return buffer.getBytes();
    } },
}, {
    { "minecraft:empty", [](ByteBuffer& buffer) { return SlotDisplayData(); } },
    { "minecraft:any_fuel", [](ByteBuffer& buffer) { return SlotDisplayData(); } },
    { "minecraft:item", [](ByteBuffer& buffer) {
        SlotDisplayData data;
        data.m_itemType = buffer.readVarNumeric<int>();
        return data;
    } }, 
    { "minecraft:item_stack", [](ByteBuffer& buffer) { return SlotDisplayData(); } }, 
    { "minecraft:tag", [](ByteBuffer& buffer) {
        SlotDisplayData data;
        data.m_tag = buffer.readIdentifier();
        return data;
    } }, 
    { "minecraft:smithing_trim", [](ByteBuffer& buffer) {
        SlotDisplayData data;
        data.m_children = buffer.readArray<SlotDisplay>(&ByteBuffer::readSlotDisplay, 3);
        return data;
    } }, 
    { "minecraft:with_remainder", [](ByteBuffer& buffer) {
        SlotDisplayData data;
        data.m_children = buffer.readArray<SlotDisplay>(&ByteBuffer::readSlotDisplay, 2);
        return data;
    } }, 
    { "minecraft:composite", [](ByteBuffer& buffer) {
        SlotDisplayData data;
        data.m_children = buffer.readPrefixedArray<SlotDisplay>(&ByteBuffer::readSlotDisplay);
        return data;
    } }, 
}, Identifier("minecraft:empty"));
Registry<char> g_mushroomVariantsRegistry = Registry<char>({ { "minecraft:red", 0 }, { "minecraft:brown", 1 } }, {}, {}, Identifier("minecraft:red"));

std::unordered_map<std::string, std::function<void(ByteBuffer&, ZincConnection*)>> g_zincServerPluginChannels = {
    { Identifier("minecraft", "brand").toString(), BrandChannel }
};
std::unordered_map<std::string, std::function<void(std::optional<std::vector<char>>&, ZincConnection*)>> g_zincCookieResponseParsers;
std::unordered_map<ZincConnection::State, std::vector<std::string>> g_zincCookieRequests = {
    { ZincConnection::State::Login, {} },
    { ZincConnection::State::Config, {} },
    { ZincConnection::State::Play, {} }
};
std::unordered_map<ZincConnection::State, std::unordered_map<std::string, std::function<ByteBuffer(ZincConnection*)>>> g_zincServerInitPluginChannels = {
    { ZincConnection::State::Login, {} },
    { ZincConnection::State::Config, {} },
    { ZincConnection::State::Play, {} }
};

}