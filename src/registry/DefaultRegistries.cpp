#include "type/Identifier.h"
#include "type/TextComponent.h"
#include "type/nbt/NBTElement.h"
#include "util/Memory.h"
#include <registry/DefaultRegistries.h>
#include <network/minecraft/channels/BrandChannel.h>
#include <string>
#include <type/ByteBuffer.h>

namespace zinc {

ByteBuffer getNetworkRegistry(const std::unordered_map<std::string, NBTElement>& registryData, const Identifier& registryIdentifier) {
    ByteBuffer result;
    result.writeIdentifier(registryIdentifier);
    result.writeVarNumeric<int>(zinc_safe_cast<size_t, int>(registryData.size()));
    for (const auto& registryEntry : registryData) {
        result.writeString(registryEntry.first);
        result.writeByte(true);
        result.writeNBTElement(registryEntry.second);
    }
    return result;
}
ByteBuffer getNetworkRegistry(const std::vector<std::string>& registryData, const Identifier& registryIdentifier) {
    ByteBuffer result;
    result.writeIdentifier(registryIdentifier);
    result.writeVarNumeric<int>(zinc_safe_cast<size_t, int>(registryData.size()));
    for (const auto& registryEntry : registryData) {
        result.writeString(registryEntry);
        result.writeByte(false);
    }
    return result;
}

// START BUILTIN DATA GENERATORS
std::pair<std::string, NBTElement> builtinPainitingData(const std::string& id, int width, int height, bool hasAuthor = true) {
    return { "minecraft:" + id, NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:" + id),
        NBTElement::Compound("title", 
            TextComponentBuilder().translatable(TextComponentBuilder::TranslatableBuilder()
                .translate("painting.minecraft." + id + ".title").build()).color("yellow")
                .build().encode().m_childElements
        ),
        (hasAuthor ? NBTElement::Compound("author", TextComponentBuilder()
                .translatable(TextComponentBuilder::TranslatableBuilder()
                .translate("painting.minecraft." + id + ".author").build()).color("gray")
                .build().encode().m_childElements), 
            NBTElement::Int("width", width) : NBTElement::Int("width", width) ),
        NBTElement::Int("height", height),
    }) };
}
std::pair<std::string, NBTElement> builtinMobVariantData(const std::string& id, const std::string& variant) {
    return { variant, NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:entity/" + id + "/" + variant)
    }) };
}
std::pair<std::string, NBTElement> builtinMobVariantData(const std::string& id, int variant) {
    return builtinMobVariantData(id, (variant < 0 ? "cold_" + id : (
        variant > 0 ? "warm_" + id : "temperate_" + id
    )));
}
std::unordered_map<std::string, NBTElement> builtinTemperatureMobVariantData(const std::string& id) {
    return {
        builtinMobVariantData(id, -1),
        builtinMobVariantData(id, 0),
        builtinMobVariantData(id, 1)
    };
}
std::pair<std::string, NBTElement> builtinTrimMaterialData(const std::string& id, const std::string& color) {
    return { "minecraft:" + id, NBTElement::Compound({
        NBTElement::String("asset_name", "minecraft:" + id),
        NBTElement::Compound("description", TextComponentBuilder()
            .translatable(TextComponentBuilder::TranslatableBuilder()
                .translate("trim_material.minecraft." + id).build()).color("#" + color)
                .build().encode().m_childElements
        )
    }) };
}
std::pair<std::string, NBTElement> builtinTrimMaterialData(const std::string& id, const std::string& color, const std::string& override) {
    return { "minecraft:" + id, NBTElement::Compound({
        NBTElement::String("asset_name", "minecraft:" + id),
        NBTElement::Compound("override_armor_assets", {
            NBTElement::String("minecraft:" + override, id + "_darker")
        }),
        NBTElement::Compound("description", TextComponentBuilder()
            .translatable(TextComponentBuilder::TranslatableBuilder()
                .translate("trim_material.minecraft." + id).build()).color("#" + color)
                .build().encode().m_childElements
            )
    }) };
}
std::pair<std::string, NBTElement> builtinTrimPatternData(const std::string& id) {
    return { "minecraft:" + id, NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:" + id),
        NBTElement::Compound("description", TextComponentBuilder()
            .translatable(TextComponentBuilder::TranslatableBuilder()
                .translate("trim_pattern.minecraft." + id).build()).build().encode().m_childElements
        ),
        NBTElement::Byte("decal", false)
    }) };
}
std::pair<std::string, NBTElement> builtinBannerPatternData(const std::string& id) {
    return { "minecraft:" + id, NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:" + id),
        NBTElement::String("translation_key", "block.minecraft.banner." + id)
    }) };
}
std::pair<std::string, NBTElement> builtinEndBiomeData(const std::string& id) {
    return { "minecraft:" + id, NBTElement::Compound({
        NBTElement::Byte("has_precipitation", false),
        NBTElement::Float("temperature", 0.5f),
        NBTElement::Float("downfall", 0.5f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Int("sky_color", 0),
            NBTElement::Int("fog_color", 10518688),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) };
}
std::pair<std::string, NBTElement> builtinDesertBiomeData(const std::string& id, bool enableMusic) {
    return { "minecraft:" + id, NBTElement::Compound({
        NBTElement::Byte("has_precipitation", false),
        NBTElement::Float("temperature", 2.0f),
        NBTElement::Float("downfall", 0.0f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            (enableMusic ? NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.overworld.desert"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }), NBTElement::Int("sky_color", 7254527) : NBTElement::Int("sky_color", 7254527)),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) };
}
std::pair<std::string, NBTElement> builtinWindsweptBiomeData(const std::string& id) {
    return { "minecraft:" + id, NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", 0.2f),
        NBTElement::Float("downfall", 0.3f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Int("sky_color", 8233727),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) };
}
std::pair<std::string, NBTElement> builtinBadlandsBiomeData(const std::string& id) {
    return { "minecraft:" + id, NBTElement::Compound({
        NBTElement::Byte("has_precipitation", false),
        NBTElement::Float("temperature", 2.0f),
        NBTElement::Float("downfall", 0.0f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.overworld.badlands"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Int("sky_color", 7254527),
            NBTElement::Int("foliage_color", 10387789),
            NBTElement::Int("grass_color", 9470285),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) };
}
std::pair<std::string, NBTElement> builtinOceanBiomeData(const std::string& id, int waterColor, int waterFogColor, bool isFrozen) {
    return { "minecraft:" + id, NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", 0.5f),
        (isFrozen ? NBTElement::String("temperature_modifier", "frozen"), NBTElement::Float("downfall", 0.5f) 
            : NBTElement::Float("downfall", 0.5f)),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Int("sky_color", 8103167),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", waterColor),
            NBTElement::Int("water_fog_color", waterFogColor)
        })
    }) };
}
std::pair<std::string, NBTElement> builtinNetherBiomeData(const std::string& id, float particleProbability, const std::string& particle) {
    return { "minecraft:" + id, NBTElement::Compound({
        NBTElement::Byte("has_precipitation", false),
        NBTElement::Float("temperature", 2.0f),
        NBTElement::Float("downfall", 0.0f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient." + id + ".mood"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.nether." + id),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Compound("additions_sound", {
                NBTElement::String("sound", "minecraft:ambient." + id + ".additions"),
                NBTElement::Double("tick_chance", 0.0111)
            }),
            (particleProbability >= 0 ? NBTElement::Compound("particle", {
                NBTElement::Compound("options", {
                    NBTElement::String("type", "minecraft:" + particle)
                }),
                NBTElement::Float("probability", particleProbability)
            }), NBTElement::String("ambient_sound", "minecraft:ambient." + id + ".loop") 
                : NBTElement::String("ambient_sound", "minecraft:ambient." + id + ".loop")),
            NBTElement::Int("sky_color", 7254527),
            NBTElement::Int("fog_color", 6840176),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) };
}
std::pair<std::string, NBTElement> builtinPlainsBiomeData(const std::string& id) {
    return { "minecraft:" + id, NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", 0.8f),
        NBTElement::Float("downfall", 0.4f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Int("sky_color", 7907327),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) };
}
std::pair<std::string, NBTElement> builtinCaveBiomeData(const std::string& id) {
    return { "minecraft:" + id, NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", 0.8f),
        NBTElement::Float("downfall", 0.4f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.overworld." + id),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Int("sky_color", 7907327),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) };
}
std::pair<std::string, NBTElement> builtinJungleBiomeData(const std::string& id, float downfall) {
    return { "minecraft:" + id, NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", 0.95f),
        NBTElement::Float("downfall", downfall),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.overworld." + id),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Int("sky_color", 7842047),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) };
}
std::pair<std::string, NBTElement> builtinPeaksBiomeData(const std::string& id, float downfall, float temperature, int skyColor) {
    return { "minecraft:" + id, NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", temperature),
        NBTElement::Float("downfall", downfall),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.overworld." + id),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Int("sky_color", skyColor),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) };
}
std::pair<std::string, NBTElement> builtinTaigaBiomeData(const std::string& id, const std::string& music, int skyColor) {
    return { "minecraft:" + id, NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", 0.3f),
        NBTElement::Float("downfall", 0.8f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.overworld." + music),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Int("sky_color", skyColor),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) };
}
std::pair<std::string, NBTElement> builtinRiverBiomeData(const std::string& id, float temperature, int waterColor) {
    return { "minecraft:" + id, NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", temperature),
        NBTElement::Float("downfall", 0.5f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Int("sky_color", 8364543),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", waterColor),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) };
}
std::pair<std::string, NBTElement> builtinBirchForestBiomeData(const std::string& id) {
    return { "minecraft:" + id, NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", 0.6f),
        NBTElement::Float("downfall", 0.6f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.overworld.forest"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Int("sky_color", 8037887),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) };
}
std::pair<std::string, NBTElement> builtinSnowyPlainsBiomeData(const std::string& id) {
    return { "minecraft:" + id, NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", 0.0f),
        NBTElement::Float("downfall", 0.5f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Int("sky_color", 8364543),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) };
}
std::pair<std::string, NBTElement> builtinLushCaveBiomeData(const std::string& id, bool hasMusic) {
    return { "minecraft:" + id, NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", 0.5f),
        NBTElement::Float("downfall", 0.5f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            (hasMusic ? NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.overworld.lush_caves"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }), NBTElement::Int("sky_color", 8103167) : NBTElement::Int("sky_color", 8103167)),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) }; // why the_void uses the same data (except for music)
}
std::pair<std::string, NBTElement> builtinWolfSoundData(const std::string& id, bool hasCustomId) {
    return { "minecraft:" + id, NBTElement::Compound({
        NBTElement::String("hurt_sound", "minecraft:entity.wolf" + (hasCustomId ? "_" + id : "") + ".hurt"), 
        NBTElement::String("pant_sound", "minecraft:entity.wolf" + (hasCustomId ? "_" + id : "") + ".pant"),
        NBTElement::String("whine_sound", "minecraft:entity.wolf" + (hasCustomId ? "_" + id : "") + ".whine"), 
        NBTElement::String("ambient_sound", "minecraft:entity.wolf" + (hasCustomId ? "_" + id : "") + ".ambient"), 
        NBTElement::String("death_sound", "minecraft:entity.wolf" + (hasCustomId ? "_" + id : "") + ".death"), 
        NBTElement::String("growl_sound", "minecraft:entity.wolf" + (hasCustomId ? "_" + id : "") + ".growl")
    }) };
}
std::pair<std::string, NBTElement> builtinWolfData(const std::string& id, const std::vector<std::string>& biomes) {
    NBTElement biomesList = NBTElement::List("biomes", {});
    NBTElement result = NBTElement::Compound({
        NBTElement::Compound("assets", {
            NBTElement::String("wild", "minecraft:entity/wolf/wolf_ashen"),
            NBTElement::String("tame", "minecraft:entity/wolf/wolf_ashen_tame"),
            NBTElement::String("angry", "minecraft:entity/wolf/wolf_ashen_angry")
        })
    });
    for (const std::string& biome : biomes) biomesList.m_childElements.push_back(NBTElement::String("minecraft:" + biome));
    if (biomes.size()) result.m_childElements.push_back(biomesList);
    return { "minecraft:" + id, result };
}
std::pair<std::string, NBTElement> builtinChatTypeData(
    const std::string& id, const std::map<std::string, std::vector<std::string>>& chatTypeData, const NBTElement& style = NBTElement::Byte(0)
) {
    bool fail = false;
    if (!chatTypeData.contains("chat_translate") || !chatTypeData.contains("chat_parameters") ||
        !chatTypeData.contains("narration_translate") || !chatTypeData.contains("narration_parameters")) fail = true;
    if (chatTypeData.contains("chat_translate")) if (!chatTypeData.at("chat_translate").size()) fail = true;
    if (chatTypeData.contains("narration_translate")) if (!chatTypeData.at("narration_translate").size()) fail = true;
    if (fail) return { "minecraft:" + id, NBTElement::Compound({
        NBTElement::Compound("chat", {
            NBTElement::String("translation_key", "chat.type.text"),
            NBTElement::List("parameters", { NBTElement::String("sender"), NBTElement::String("content") })
        }),
        NBTElement::Compound("narration", {
            NBTElement::String("translation_key", "chat.type.text.narrate"),
            NBTElement::List("parameters", { NBTElement::String("sender"), NBTElement::String("content") })
        })
    }) };
    NBTElement chatParameters = NBTElement::List("parameters", {});
    NBTElement narrationParameters = NBTElement::List("parameters", {});
    for (const std::string& parameter : chatTypeData.at("chat_parameters")) 
        chatParameters.m_childElements.push_back(NBTElement::String(parameter));
    for (const std::string& parameter : chatTypeData.at("narration_parameters")) 
        narrationParameters.m_childElements.push_back(NBTElement::String(parameter));
    NBTElement data = NBTElement::Compound("chat", {
        NBTElement::String("translation_key", chatTypeData.at("chat_translate")[0]),
        chatParameters
    });
    if (style.m_type == NBTElementType::Compound)   
        data.m_childElements.push_back(NBTElement::Compound("style", style.m_childElements));
    return { "minecraft:" + id, NBTElement::Compound({
        data,
        NBTElement::Compound("narration", {
            NBTElement::String("translation_key", chatTypeData.at("narration_translate")[0]),
            narrationParameters
        })
    }) };
}
std::pair<std::string, NBTElement> builtinDamageType(const std::string& id, float exhaustion, const std::string& mId, bool isAlways) {
    return { "minecrat:" + id, NBTElement::Compound({
        NBTElement::Float("exhaustion", exhaustion),
        NBTElement::String("message_id", mId), 
        NBTElement::String("scaling", (isAlways ? "always" : "when_caused_by_living_non_player"))
    }) };
}
std::pair<std::string, NBTElement> builtinDamageEffectType(const std::string& id, float exhaustion, const std::string& mId,
                                                           const std::string& effect, bool isAlways) {
    return { "minecrat:" + id, NBTElement::Compound({
        NBTElement::Float("exhaustion", exhaustion),
        NBTElement::String("message_id", mId), 
        NBTElement::String("effects", effect), 
        NBTElement::String("scaling", (isAlways ? "always" : "when_caused_by_living_non_player"))
    }) };
}
std::pair<std::string, NBTElement> builtinDamageMessageType(const std::string& id, float exhaustion, const std::string& mId,
                                                           const std::string& message, bool isAlways) {
    return { "minecrat:" + id, NBTElement::Compound({
        NBTElement::Float("exhaustion", exhaustion),
        NBTElement::String("message_id", mId), 
        NBTElement::String("death_message_type", message), 
        NBTElement::String("scaling", (isAlways ? "always" : "when_caused_by_living_non_player"))
    }) };
}
std::pair<std::string, NBTElement> builtinGoatHorn(const std::string& id, int soundId) {
    return { "minecraft:" + id, NBTElement::Compound({
        NBTElement::String("sound_event", "minecraft:item.goat_horn.sound." + std::to_string(soundId)),
        NBTElement::Float("use_duration", 7.0f),
        NBTElement::Float("range", 256.0f),
        NBTElement::Compound("description", TextComponentBuilder()
            .translatable(TextComponentBuilder::TranslatableBuilder()
                .translate("instrument.minecraft." + id).build()).build().encode().m_childElements
        )
    }) };
}
std::pair<std::string, NBTElement> builtinJukeBoxSongHorn(const std::string& id, float length, int comparator) {
    return { "minecraft:" + id, NBTElement::Compound({
        NBTElement::String("sound_event", "minecraft:music_disc." + id),
        NBTElement::Float("length_in_seconds", length),
        NBTElement::Int("comparator_output", comparator),
        NBTElement::Compound("description", TextComponentBuilder()
            .translatable(TextComponentBuilder::TranslatableBuilder()
                .translate("jukebox_song.minecraft." + id).build()).build().encode().m_childElements
        )
    }) };
}
std::pair<std::string, NBTElement> builtinEnchantment(const std::string& id, int minBase, int minInc, int maxBase, int maxInc, int anvil,
                                                      const std::vector<std::string>& slots, const std::string& supportedItems,
                                                      int weight, int maxLevel, const std::vector<NBTElement>& effects,
                                                      const std::string& exclusiveSet) {
    NBTElement slotsNBT = NBTElement::List("slots", {});
    for (const std::string& slot : slots) slotsNBT.m_childElements.push_back(NBTElement::String(slot));
    return { "minecraft:" + id, NBTElement::Compound({
        NBTElement::Compound("description", TextComponentBuilder()
            .translatable(TextComponentBuilder::TranslatableBuilder().translate("enchantment.minecraft." + id).build())
            .build().encode().m_childElements),
        NBTElement::Compound("min_cost", {
            NBTElement::Int("base", minBase),
            NBTElement::Int("per_level_above_first", minInc)
        }),
        NBTElement::Compound("max_cost", {
            NBTElement::Int("base", maxBase),
            NBTElement::Int("per_level_above_first", maxInc)
        }),
        NBTElement::Int("anvil_cost", anvil),
        slotsNBT,
        NBTElement::String("supported_items", supportedItems),
        NBTElement::String("exclusive_set", exclusiveSet),
        NBTElement::Int("weight", weight),
        NBTElement::Int("max_level", maxLevel),
        NBTElement::Compound("effects", effects)
    }) };
}
std::pair<std::string, NBTElement> builtinEnchantment(const std::string& id, int minBase, int minInc, int maxBase, int maxInc, int anvil,
                                                      const std::vector<std::string>& slots, const std::string& supportedItems,
                                                      int weight, int maxLevel, const std::vector<NBTElement>& effects,
                                                      const std::string& exclusiveSet, const std::string& primaryItems) {
    NBTElement slotsNBT = NBTElement::List("slots", {});
    for (const std::string& slot : slots) slotsNBT.m_childElements.push_back(NBTElement::String(slot));
    return { "minecraft:" + id, NBTElement::Compound({
        NBTElement::Compound("description", TextComponentBuilder()
            .translatable(TextComponentBuilder::TranslatableBuilder().translate("enchantment.minecraft." + id).build())
            .build().encode().m_childElements),
        NBTElement::Compound("min_cost", {
            NBTElement::Int("base", minBase),
            NBTElement::Int("per_level_above_first", minInc)
        }),
        NBTElement::Compound("max_cost", {
            NBTElement::Int("base", maxBase),
            NBTElement::Int("per_level_above_first", maxInc)
        }),
        NBTElement::Int("anvil_cost", anvil),
        slotsNBT,
        NBTElement::String("supported_items", supportedItems),
        NBTElement::String("primary_items", primaryItems),
        NBTElement::String("exclusive_set", exclusiveSet),
        NBTElement::Int("weight", weight),
        NBTElement::Int("max_level", maxLevel),
        NBTElement::Compound("effects", effects)
    }) };
}
std::pair<std::string, NBTElement> builtinEnchantment(const std::string& id, int minBase, int minInc, int maxBase, int maxInc, int anvil,
                                                      const std::vector<std::string>& slots, const std::string& supportedItems,
                                                      int weight, int maxLevel, const std::vector<NBTElement>& effects) {
    NBTElement slotsNBT = NBTElement::List("slots", {});
    for (const std::string& slot : slots) slotsNBT.m_childElements.push_back(NBTElement::String(slot));
    return { "minecraft:" + id, NBTElement::Compound({
        NBTElement::Compound("description", TextComponentBuilder()
            .translatable(TextComponentBuilder::TranslatableBuilder().translate("enchantment.minecraft." + id).build())
            .build().encode().m_childElements),
        NBTElement::Compound("min_cost", {
            NBTElement::Int("base", minBase),
            NBTElement::Int("per_level_above_first", minInc)
        }),
        NBTElement::Compound("max_cost", {
            NBTElement::Int("base", maxBase),
            NBTElement::Int("per_level_above_first", maxInc)
        }),
        NBTElement::Int("anvil_cost", anvil),
        slotsNBT,
        NBTElement::String("supported_items", supportedItems),
        NBTElement::Int("weight", weight),
        NBTElement::Int("max_level", maxLevel),
        NBTElement::Compound("effects", effects)
    }) };
}
NBTElement attributeBuilder(const std::string& id, const std::string& attr, const std::string& operation, 
                            float base, float perLevel, const std::string& type) {
    return NBTElement::Compound({
        NBTElement::String("id", "minecraft:" + id),
        NBTElement::String("attribute", "minecraft:" + attr),
        NBTElement::String("operation", operation),
        NBTElement::Compound("amount", {
            NBTElement::String("type", "minecraft:" + type),
            NBTElement::Float("base", base),
            NBTElement::Float("per_level_above_first", perLevel)
        })
    });
}
// END BUILTIN DATA GENERATORS

std::unordered_map<std::string, std::unordered_map<std::string, NBTElement>> g_registries = {
    { "chat_type", {
        builtinChatTypeData("chat", {
            { "chat_translate",  { "chat.type.text" } }, { "narration_translate",  { "chat.type.text.narrate" } },
            { "chat_parameters",  { "sender", "content" } }, { "narration_parameters",  { "sender", "content" } }
        }),
        builtinChatTypeData("emote_command", {
            { "chat_translate",  { "chat.type.emote" } }, { "narration_translate",  { "chat.type.emote" } },
            { "chat_parameters",  { "sender", "content" } }, { "narration_parameters",  { "sender", "content" } }
        }),
        builtinChatTypeData("msg_command_incoming", {
            { "chat_translate",  { "commands.message.display.incoming" } }, { "narration_translate",  { "chat.type.text.narrate" } },
            { "chat_parameters",  { "sender", "content" } }, { "narration_parameters",  { "sender", "content" } }
        }, NBTElement::Compound({
            NBTElement::Byte("italic", false), NBTElement::String("color", "gray") 
        })),
        builtinChatTypeData("msg_command_incoming", {
            { "chat_translate",  { "commands.message.display.outgoing" } }, { "narration_translate",  { "chat.type.text.narrate" } },
            { "chat_parameters",  { "sender", "content" } }, { "narration_parameters",  { "sender", "content" } }
        }, NBTElement::Compound({
            NBTElement::Byte("italic", false), NBTElement::String("color", "gray") 
        })),
        builtinChatTypeData("say_command", {
            { "chat_translate",  { "chat.type.announcement" } }, { "narration_translate",  { "chat.type.text.narrate" } },
            { "chat_parameters",  { "sender", "content" } }, { "narration_parameters",  { "sender", "content" } }
        }),
        builtinChatTypeData("team_msg_command_incoming", {
            { "chat_translate",  { "chat.type.team.text" } }, { "narration_translate",  { "chat.type.text.narrate" } },
            { "chat_parameters",  { "target", "sender", "content" } }, { "narration_parameters",  { "sender", "content" } }
        }),
        builtinChatTypeData("team_msg_command_outgoing", {
            { "chat_translate",  { "chat.type.team.text" } }, { "narration_translate",  { "chat.type.text.narrate" } },
            { "chat_parameters",  { "target", "sender", "content" } }, { "narration_parameters",  { "sender", "content" } }
        }),
    } },
    { "banner_pattern", {
        builtinBannerPatternData("base"), builtinBannerPatternData("border"), builtinBannerPatternData("bricks"),
        builtinBannerPatternData("circle"), builtinBannerPatternData("creeper"), builtinBannerPatternData("cross"),
        builtinBannerPatternData("curly_border"), builtinBannerPatternData("diagonal_left"), 
        builtinBannerPatternData("diagonal_right"), builtinBannerPatternData("diagonal_left_up"),
        builtinBannerPatternData("diagonal_right_up"), builtinBannerPatternData("flow"), builtinBannerPatternData("flower"),
        builtinBannerPatternData("globe"), builtinBannerPatternData("gradient"), builtinBannerPatternData("gradient_up"),
        builtinBannerPatternData("guster"), builtinBannerPatternData("half_horizontal"), 
        builtinBannerPatternData("half_horizontal_bottom"), builtinBannerPatternData("half_vertical"),
        builtinBannerPatternData("half_vertical_right"), builtinBannerPatternData("mojang"), builtinBannerPatternData("piglin"),
        builtinBannerPatternData("rhombus"), builtinBannerPatternData("skull"), builtinBannerPatternData("small_stripes"),
        builtinBannerPatternData("square_bottom_left"), builtinBannerPatternData("square_bottom_right"),
        builtinBannerPatternData("square_top_left"), builtinBannerPatternData("square_top_right"),
        builtinBannerPatternData("straight_cross"), builtinBannerPatternData("stripe_bottom"),
        builtinBannerPatternData("stripe_center"), builtinBannerPatternData("stripe_downleft"),
        builtinBannerPatternData("stripe_downright"), builtinBannerPatternData("stripe_left"),
        builtinBannerPatternData("stripe_middle"), builtinBannerPatternData("stripe_right"), 
        builtinBannerPatternData("stripe_top"), builtinBannerPatternData("triangle_bottom"),
        builtinBannerPatternData("triangle_top"), builtinBannerPatternData("triangles_bottom"),
        builtinBannerPatternData("triangles_top"),
    } },
    { "trim_pattern", {
        builtinTrimPatternData("bolt"), builtinTrimPatternData("coast"), builtinTrimPatternData("dune"),
        builtinTrimPatternData("eye"), builtinTrimPatternData("host"), builtinTrimPatternData("raiser"),
        builtinTrimPatternData("rib"), builtinTrimPatternData("sentry"), builtinTrimPatternData("shaper"),
        builtinTrimPatternData("silence"), builtinTrimPatternData("snout"), builtinTrimPatternData("spire"),
        builtinTrimPatternData("tide"), builtinTrimPatternData("vex"), builtinTrimPatternData("ward"),
        builtinTrimPatternData("wayfinder"), builtinTrimPatternData("wild")
    } },
    { "damage_type", {
        builtinDamageType("arrow", 0.1f, "arrow", false),
        builtinDamageMessageType("bad_respawn_point", 0.1f, "badRespawnPoint", "intentional_game_design", true),
        builtinDamageType("cactus", 0.1f, "cactus", false),
        builtinDamageEffectType("campfire", 0.1f, "inFire", "burning", false),
        builtinDamageType("cramming", 0.0f, "cramming", false),
        builtinDamageType("dragon_breath", 0.0f, "dragonBreath", false),
        builtinDamageEffectType("drown", 0.0f, "drown", "drowning", false),
        builtinDamageType("dry_out", 0.1f, "dryout", false),
        builtinDamageMessageType("ender_pearl", 0.0f, "fall", "fall_variants", false),
        builtinDamageType("explosion", 0.1f, "explosion", true),
        builtinDamageMessageType("fall", 0.0f, "fall", "fall_variants", false),
        builtinDamageType("falling_anvil", 0.1f, "anvil", false),
        builtinDamageType("falling_block", 0.1f, "fallingBlock", false),
        builtinDamageType("falling_stalactite", 0.1f, "fallingStalactite", false),
        builtinDamageEffectType("fireball", 0.1f, "fireball", "burning", false),
        builtinDamageType("fireworks", 0.1f, "fireworks", false),
        builtinDamageType("fly_into_wall", 0.0f, "flyIntoWall", false),
        builtinDamageEffectType("freeze", 0.0f, "freeze", "freezing", false),
        builtinDamageType("generic", 0.0f, "generic", false),
        builtinDamageType("generic_kill", 0.0f, "genericKill", false),
        builtinDamageEffectType("hot_floor", 0.1f, "hotFloor", "burning", false),
        builtinDamageEffectType("in_fire", 0.1f, "inFire", "burning", false),
        builtinDamageType("in_wall", 0.0f, "inWall", false),
        builtinDamageType("indirect_magic", 0.0f, "indirectMagic", false),
        builtinDamageEffectType("lava", 0.1f, "lava", "burning", false),
        builtinDamageType("lightning_bolt", 0.1f, "lightningBolt", false),
        builtinDamageType("mace_smash", 0.1f, "mace_smash", false),
        builtinDamageType("magic", 0.0f, "magic", false),
        builtinDamageType("mob_attack", 0.1f, "mob", false),
        builtinDamageType("mob_attack_no_aggro", 0.1f, "mob", false),
        builtinDamageType("mob_projectile", 0.1f, "mob", false),
        builtinDamageEffectType("on_fire", 0.0f, "onFire", "burning", false),
        builtinDamageType("out_of_world", 0.0f, "outOfWorld", false),
        builtinDamageType("outside_border", 0.0f, "outsideBorder", false),
        builtinDamageType("player_attack", 0.0f, "player", false),
        builtinDamageType("player_explosion", 0.1f, "explosion.player", true),
        builtinDamageType("sonic_boom", 0.1f, "sonic_boom", true),
        builtinDamageType("spit", 0.0f, "mob", false),
        builtinDamageType("stalagmite", 0.1f, "stalagmite", false),
        builtinDamageType("starve", 0.0f, "starve", false),
        builtinDamageType("sting", 0.0f, "sting", false),
        builtinDamageEffectType("sweet_berry_bush", 0.1f, "sweetBerryBush", "poking", false),
        builtinDamageEffectType("thorns", 0.1f, "thorns", "thorns", false),
        builtinDamageType("thrown", 0.1f, "thrown", false),
        builtinDamageType("trident", 0.1f, "trident", false),
        builtinDamageEffectType("unattributed_fireball", 0.1f, "onFire", "burning", false),
        builtinDamageType("wind_charge", 0.1f, "mob", false),
        builtinDamageType("wither", 0.0f, "wither", false),
        builtinDamageType("wither_skull", 0.1f, "witherSkull", false),
    } },
    { "trim_material", {
        builtinTrimMaterialData("amethyst", "9A5CC6"),builtinTrimMaterialData("copper", "B4684D"),
        builtinTrimMaterialData("diamond", "6EECD2", "diamond"),
        builtinTrimMaterialData("iron", "ECECEC", "iron"), builtinTrimMaterialData("gold", "DEB12D", "gold"),
        builtinTrimMaterialData("netherite", "625859", "netherite"), builtinTrimMaterialData("lapis", "416E97"),
        builtinTrimMaterialData("quartz", "E3D4C4"), builtinTrimMaterialData("redstone", "971607"),
        builtinTrimMaterialData("resin", "FC7812"), builtinTrimMaterialData("emerald", "11A036")
    } },
    { "dimension_type", {
        { "minecraft:overworld", NBTElement::Compound({
            NBTElement::String("infiniburn", "#minecraft:infiniburn_overworld"),
            NBTElement::String("effects", "minecraft:overworld"),
            NBTElement::Float("ambient_light", 0.0f),
            NBTElement::Byte("piglin_safe", false),
            NBTElement::Byte("has_raids", true),
            NBTElement::Byte("respawn_anchor_works", false),
            NBTElement::Byte("ultrawarm", false),
            NBTElement::Byte("natural", true),
            NBTElement::Byte("bed_works", true),
            NBTElement::Byte("has_skylight", true),
            NBTElement::Byte("has_ceiling", false),
            NBTElement::Int("monster_spawn_block_light_limit", 0),
            NBTElement::Int("min_y", -64),
            NBTElement::Int("height", 384),
            NBTElement::Int("logical_height", 384),
            NBTElement::Double("coordinate_scale", 1.0f),
            NBTElement::Compound("monster_spawn_light_level", {
                NBTElement::Int("min_inclusive", 0),
                NBTElement::Int("max_inclusive", 7),
                NBTElement::String("type",  "minecraft:uniform")
            })
        }) },
        { "minecraft:overworld_caves", NBTElement::Compound({
            NBTElement::String("infiniburn", "#minecraft:infiniburn_overworld"),
            NBTElement::String("effects", "minecraft:overworld"),
            NBTElement::Float("ambient_light", 0.0f),
            NBTElement::Byte("piglin_safe", false),
            NBTElement::Byte("has_raids", true),
            NBTElement::Byte("respawn_anchor_works", false),
            NBTElement::Byte("ultrawarm", false),
            NBTElement::Byte("natural", true),
            NBTElement::Byte("bed_works", true),
            NBTElement::Byte("has_skylight", true),
            NBTElement::Byte("has_ceiling", false),
            NBTElement::Int("monster_spawn_block_light_limit", 0),
            NBTElement::Int("min_y", -64),
            NBTElement::Int("height", 384),
            NBTElement::Int("logical_height", 384),
            NBTElement::Double("coordinate_scale", 1.0f),
            NBTElement::Compound("monster_spawn_light_level", {
                NBTElement::Int("min_inclusive", 0),
                NBTElement::Int("max_inclusive", 7),
                NBTElement::String("type",  "minecraft:uniform")
            })
        }) },
        { "minecraft:the_end", NBTElement::Compound({
            NBTElement::String("infiniburn", "#minecraft:infiniburn_end"),
            NBTElement::String("effects", "minecraft:the_end"),
            NBTElement::Float("ambient_light", 0.0f),
            NBTElement::Byte("piglin_safe", false),
            NBTElement::Byte("has_raids", true),
            NBTElement::Byte("respawn_anchor_works", false),
            NBTElement::Byte("ultrawarm", false),
            NBTElement::Byte("natural", false),
            NBTElement::Byte("bed_works", false),
            NBTElement::Byte("has_skylight", true),
            NBTElement::Byte("has_ceiling", false),
            NBTElement::Int("monster_spawn_block_light_limit", 0),
            NBTElement::Int("min_y", 0),
            NBTElement::Int("height", 256),
            NBTElement::Int("logical_height", 256),
            NBTElement::Long("fixed_type", 6000),
            NBTElement::Double("coordinate_scale", 1.0f),
            NBTElement::Compound("monster_spawn_light_level", {
                NBTElement::Int("min_inclusive", 0),
                NBTElement::Int("max_inclusive", 7),
                NBTElement::String("type",  "minecraft:uniform")
            })
        }) },
        { "minecraft:the_nether", NBTElement::Compound({
            NBTElement::String("infiniburn", "#minecraft:infiniburn_nether"),
            NBTElement::String("effects", "minecraft:the_nether"),
            NBTElement::Float("ambient_light", 0.1f),
            NBTElement::Byte("piglin_safe", true),
            NBTElement::Byte("has_raids", false),
            NBTElement::Byte("respawn_anchor_works", true),
            NBTElement::Byte("ultrawarm", true),
            NBTElement::Byte("natural", false),
            NBTElement::Byte("bed_works", false),
            NBTElement::Byte("has_skylight", false),
            NBTElement::Byte("has_ceiling", true),
            NBTElement::Int("monster_spawn_block_light_limit", 15),
            NBTElement::Int("min_y", 0),
            NBTElement::Int("height", 256),
            NBTElement::Int("logical_height", 128),
            NBTElement::Long("fixed_type", 18000),
            NBTElement::Double("coordinate_scale", 8.0f),
            NBTElement::Int("monster_spawn_light_level", 7)
        }) }
    } },
    { "worldgen/biome", {
        // builtinBadlandsBiomeData("badlands"),
        builtinJungleBiomeData("bamboo_jungle", 0.9f),
        builtinNetherBiomeData("basalt_deltas", 0.118093334f, "white_ash"),
    //     builtinPlainsBiomeData("beach"),
    //     builtinBirchForestBiomeData("birch_forest"),
    //     { "minecraft:cherry_grove", NBTElement::Compound({
    //         NBTElement::Byte("has_precipitation", true),
    //         NBTElement::Float("temperature", 0.5f),
    //         NBTElement::Float("downfall", 0.8f),
    //         NBTElement::Compound("effects", {
    //             NBTElement::Compound("mood_sound", {
    //                 NBTElement::String("sound", "minecraft:ambient.cave"),
    //                 NBTElement::Int("tick_delay", 6000),
    //                 NBTElement::Int("block_search_extent", 8),
    //                 NBTElement::Double("offset", 2.0)
    //             }),
    //             NBTElement::Compound("music", {
    //                 NBTElement::String("sound", "minecraft:music.overworld.cherry_grove"),
    //                 NBTElement::Int("min_delay", 12000),
    //                 NBTElement::Int("max_delay", 24000),
    //                 NBTElement::Byte("replace_current_music", false)
    //             }),
    //             NBTElement::Int("sky_color", 8103167),
    //             NBTElement::Int("foliage_color", 11983713),
    //             NBTElement::Int("grass_color", 11983713),
    //             NBTElement::Int("fog_color", 12638463),
    //             NBTElement::Int("water_color", 6141935),
    //             NBTElement::Int("water_fog_color", 6141935)
    //         })
    //     }) },
        builtinOceanBiomeData("cold_ocean", 4020182, 329011, false),
        builtinNetherBiomeData("crimson_forest", 0.025f, "crimson_spore"),
    //     { "minecraft:dark_forest", NBTElement::Compound({
    //         NBTElement::Byte("has_precipitation", true),
    //         NBTElement::Float("temperature", 0.7f),
    //         NBTElement::Float("downfall", 0.8f),
    //         NBTElement::Compound("effects", {
    //             NBTElement::Compound("mood_sound", {
    //                 NBTElement::String("sound", "minecraft:ambient.cave"),
    //                 NBTElement::Int("tick_delay", 6000),
    //                 NBTElement::Int("block_search_extent", 8),
    //                 NBTElement::Double("offset", 2.0)
    //             }),
    //             NBTElement::Compound("music", {
    //                 NBTElement::String("sound", "minecraft:music.overworld.forest"),
    //                 NBTElement::Int("min_delay", 12000),
    //                 NBTElement::Int("max_delay", 24000),
    //                 NBTElement::Byte("replace_current_music", false)
    //             }),
    //             NBTElement::String("grass_color_modifier", "dark_forest"),
    //             NBTElement::Int("sky_color", 7972607),
    //             NBTElement::Int("fog_color", 12638463),
    //             NBTElement::Int("dry_foliage_color", 8082228),
    //             NBTElement::Int("water_color", 4159204),
    //             NBTElement::Int("water_fog_color", 329011)
    //         })
    //     }) },
        builtinOceanBiomeData("deep_cold_ocean", 4020182, 329011, false),
    //     builtinCaveBiomeData("deep_dark"),
        builtinOceanBiomeData("deep_frozen_ocean", 3750089, 329011, true),
        builtinOceanBiomeData("deep_lukewarm_ocean", 4566514, 267827, false),
        builtinOceanBiomeData("deep_ocean", 4159204, 329011, false),
        builtinDesertBiomeData("desert", true),
        builtinCaveBiomeData("dripstone_caves"),
        builtinEndBiomeData("end_barrens"),
        builtinEndBiomeData("end_highlands"),
        builtinEndBiomeData("end_midlands"),
        builtinBadlandsBiomeData("eroded_badlands"),
        { "minecraft:flower_forest", NBTElement::Compound({
            NBTElement::Byte("has_precipitation", true),
            NBTElement::Float("temperature", 0.7f),
            NBTElement::Float("downfall", 0.8f),
            NBTElement::Compound("effects", {
                NBTElement::Compound("mood_sound", {
                    NBTElement::String("sound", "minecraft:ambient.cave"),
                    NBTElement::Int("tick_delay", 6000),
                    NBTElement::Int("block_search_extent", 8),
                    NBTElement::Double("offset", 2.0)
                }),
                NBTElement::Compound("music", {
                    NBTElement::String("sound", "minecraft:music.overworld.flower_forest"),
                    NBTElement::Int("min_delay", 12000),
                    NBTElement::Int("max_delay", 24000),
                    NBTElement::Byte("replace_current_music", false)
                }),
                NBTElement::Int("sky_color", 7972607),
                NBTElement::Int("fog_color", 12638463),
                NBTElement::Int("water_color", 4159204),
                NBTElement::Int("water_fog_color", 329011)
            })
        }) },
    //     { "minecraft:forest", NBTElement::Compound({
    //         NBTElement::Byte("has_precipitation", true),
    //         NBTElement::Float("temperature", 0.7f),
    //         NBTElement::Float("downfall", 0.8f),
    //         NBTElement::Compound("effects", {
    //             NBTElement::Compound("mood_sound", {
    //                 NBTElement::String("sound", "minecraft:ambient.cave"),
    //                 NBTElement::Int("tick_delay", 6000),
    //                 NBTElement::Int("block_search_extent", 8),
    //                 NBTElement::Double("offset", 2.0)
    //             }),
    //             NBTElement::Compound("music", {
    //                 NBTElement::String("sound", "minecraft:music.overworld.forest"),
    //                 NBTElement::Int("min_delay", 12000),
    //                 NBTElement::Int("max_delay", 24000),
    //                 NBTElement::Byte("replace_current_music", false)
    //             }),
    //             NBTElement::Int("sky_color", 7972607),
    //             NBTElement::Int("fog_color", 12638463),
    //             NBTElement::Int("water_color", 4159204),
    //             NBTElement::Int("water_fog_color", 329011)
    //         })
    //     }) },
        builtinOceanBiomeData("frozen_ocean", 3750089, 329011, true),
    //     builtinPeaksBiomeData("frozen_peaks", 0.9f, -0.7f, 8756735),
    //     builtinRiverBiomeData("frozen_river", 0.0f, 3750089),
    //     { "minecraft:grove", NBTElement::Compound({
    //         NBTElement::Byte("has_precipitation", true),
    //         NBTElement::Float("temperature", -0.2f),
    //         NBTElement::Float("downfall", 0.8f),
    //         NBTElement::Compound("effects", {
    //             NBTElement::Compound("mood_sound", {
    //                 NBTElement::String("sound", "minecraft:ambient.cave"),
    //                 NBTElement::Int("tick_delay", 6000),
    //                 NBTElement::Int("block_search_extent", 8),
    //                 NBTElement::Double("offset", 2.0)
    //             }),
    //             NBTElement::Compound("music", {
    //                 NBTElement::String("sound", "minecraft:music.overworld.grove"),
    //                 NBTElement::Int("min_delay", 12000),
    //                 NBTElement::Int("max_delay", 24000),
    //                 NBTElement::Byte("replace_current_music", false)
    //             }),
    //             NBTElement::Int("sky_color", 8495359),
    //             NBTElement::Int("fog_color", 12638463),
    //             NBTElement::Int("water_color", 4159204),
    //             NBTElement::Int("water_fog_color", 329011)
    //         })
    //     }) },
    //     builtinSnowyPlainsBiomeData("ice_spikes"),
    //     builtinPeaksBiomeData("jagged_peaks", 0.9f, -0.7f, 8756735),
        builtinJungleBiomeData("jungle", 0.9f),
        builtinOceanBiomeData("lukewarm_ocean", 4566514, 267827, false),
    //     builtinLushCaveBiomeData("lush_caves", true),
    //     { "minecraft:mangrove_swamp", NBTElement::Compound({
    //         NBTElement::Byte("has_precipitation", true),
    //         NBTElement::Float("temperature", 0.8f),
    //         NBTElement::Float("downfall", 0.9f),
    //         NBTElement::Compound("effects", {
    //             NBTElement::Compound("mood_sound", {
    //                 NBTElement::String("sound", "minecraft:ambient.cave"),
    //                 NBTElement::Int("tick_delay", 6000),
    //                 NBTElement::Int("block_search_extent", 8),
    //                 NBTElement::Double("offset", 2.0)
    //             }),
    //             NBTElement::Compound("music", {
    //                 NBTElement::String("sound", "minecraft:music.overworld.swamp"),
    //                 NBTElement::Int("min_delay", 12000),
    //                 NBTElement::Int("max_delay", 24000),
    //                 NBTElement::Byte("replace_current_music", false)
    //             }),
    //             NBTElement::String("grass_color_modifier", "swamp"),
    //             NBTElement::Int("sky_color", 7907327),
    //             NBTElement::Int("dry_foliage_color", 8082228),
    //             NBTElement::Int("foliage_color", 9285927),
    //             NBTElement::Int("fog_color", 12638463),
    //             NBTElement::Int("water_color", 3832426),
    //             NBTElement::Int("water_fog_color", 5077600)
    //         })
    //     }) },
    //     { "minecraft:meadow", NBTElement::Compound({
    //         NBTElement::Byte("has_precipitation", true),
    //         NBTElement::Float("temperature", 0.5f),
    //         NBTElement::Float("downfall", 0.8f),
    //         NBTElement::Compound("effects", {
    //             NBTElement::Compound("mood_sound", {
    //                 NBTElement::String("sound", "minecraft:ambient.cave"),
    //                 NBTElement::Int("tick_delay", 6000),
    //                 NBTElement::Int("block_search_extent", 8),
    //                 NBTElement::Double("offset", 2.0)
    //             }),
    //             NBTElement::Compound("music", {
    //                 NBTElement::String("sound", "minecraft:music.overworld.meadow"),
    //                 NBTElement::Int("min_delay", 12000),
    //                 NBTElement::Int("max_delay", 24000),
    //                 NBTElement::Byte("replace_current_music", false)
    //             }),
    //             NBTElement::Int("sky_color", 8103167),
    //             NBTElement::Int("fog_color", 12638463),
    //             NBTElement::Int("water_color", 937679),
    //             NBTElement::Int("water_fog_color", 329011)
    //         })
    //     }) },
    //     { "minecraft:mushroom_fields", NBTElement::Compound({
    //         NBTElement::Byte("has_precipitation", true),
    //         NBTElement::Float("temperature", 0.9f),
    //         NBTElement::Float("downfall", 1.0f),
    //         NBTElement::Compound("effects", {
    //             NBTElement::Compound("mood_sound", {
    //                 NBTElement::String("sound", "minecraft:ambient.cave"),
    //                 NBTElement::Int("tick_delay", 6000),
    //                 NBTElement::Int("block_search_extent", 8),
    //                 NBTElement::Double("offset", 2.0)
    //             }),
    //             NBTElement::Int("sky_color", 7842047),
    //             NBTElement::Int("fog_color", 12638463),
    //             NBTElement::Int("water_color", 4159204),
    //             NBTElement::Int("water_fog_color", 329011)
    //         })
    //     }) },
        builtinNetherBiomeData("nether_wastes", -1.0f, ""),
        builtinOceanBiomeData("ocean", 4159204, 329011, false),
        // builtinBirchForestBiomeData("old_growth_birch_forest"),
        // builtinTaigaBiomeData("old_growth_pine_taiga", "old_growth_taiga", 8168447),
        // builtinTaigaBiomeData("old_growth_spruce_taiga", "old_growth_taiga", 8233983),
        // { "minecraft:pale_garden", NBTElement::Compound({
        //     NBTElement::Byte("has_precipitation", true),
        //     NBTElement::Float("temperature", 0.7f),
        //     NBTElement::Float("downfall", 0.8f),
        //     NBTElement::Compound("effects", {
        //         NBTElement::Compound("mood_sound", {
        //             NBTElement::String("sound", "minecraft:ambient.cave"),
        //             NBTElement::Int("tick_delay", 6000),
        //             NBTElement::Int("block_search_extent", 8),
        //             NBTElement::Double("offset", 2.0)
        //         }),
        //         NBTElement::Int("sky_color", 12171705),
        //         NBTElement::Int("dry_foliage_color", 10528412),
        //         NBTElement::Int("foliage_color", 8883574),
        //         NBTElement::Int("grass_color", 7832178),
        //         NBTElement::Int("fog_color", 8484720),
        //         NBTElement::Int("water_color", 7768221),
        //         NBTElement::Int("water_fog_color", 5597568)
        //     })
        // }) },
        // builtinPlainsBiomeData("plains"),
        // builtinRiverBiomeData("river", 0.5f, 4159204),
        builtinDesertBiomeData("savanna", false),
        builtinDesertBiomeData("savanna_plateau", false),
    //     builtinEndBiomeData("small_end_islands"),
    //     { "minecraft:snowy_beach", NBTElement::Compound({
    //         NBTElement::Byte("has_precipitation", true),
    //         NBTElement::Float("temperature", 0.05f),
    //         NBTElement::Float("downfall", 0.3f),
    //         NBTElement::Compound("effects", {
    //             NBTElement::Compound("mood_sound", {
    //                 NBTElement::String("sound", "minecraft:ambient.cave"),
    //                 NBTElement::Int("tick_delay", 6000),
    //                 NBTElement::Int("block_search_extent", 8),
    //                 NBTElement::Double("offset", 2.0)
    //             }),
    //             NBTElement::Int("sky_color", 8364543),
    //             NBTElement::Int("fog_color", 12638463),
    //             NBTElement::Int("water_color", 4020182),
    //             NBTElement::Int("water_fog_color", 329011)
    //         })
    //     }) },
    //     builtinSnowyPlainsBiomeData("snowy_plains"),
    //     { "minecraft:snowy_slopes", NBTElement::Compound({
    //         NBTElement::Byte("has_precipitation", true),
    //         NBTElement::Float("temperature", -0.3f),
    //         NBTElement::Float("downfall", 0.9f),
    //         NBTElement::Compound("effects", {
    //             NBTElement::Compound("mood_sound", {
    //                 NBTElement::String("sound", "minecraft:ambient.cave"),
    //                 NBTElement::Int("tick_delay", 6000),
    //                 NBTElement::Int("block_search_extent", 8),
    //                 NBTElement::Double("offset", 2.0)
    //             }),
    //             NBTElement::Compound("music", {
    //                 NBTElement::String("sound", "minecraft:music.overworld.snowy_slopes"),
    //                 NBTElement::Int("min_delay", 12000),
    //                 NBTElement::Int("max_delay", 24000),
    //                 NBTElement::Byte("replace_current_music", false)
    //             }),
    //             NBTElement::Int("sky_color", 8560639),
    //             NBTElement::Int("fog_color", 12638463),
    //             NBTElement::Int("water_color", 4159204),
    //             NBTElement::Int("water_fog_color", 329011)
    //         })
    //     }) },
    //     { "minecraft:snowy_taiga", NBTElement::Compound({
    //         NBTElement::Byte("has_precipitation", true),
    //         NBTElement::Float("temperature", -0.5f),
    //         NBTElement::Float("downfall", 0.4f),
    //         NBTElement::Compound("effects", {
    //             NBTElement::Compound("mood_sound", {
    //                 NBTElement::String("sound", "minecraft:ambient.cave"),
    //                 NBTElement::Int("tick_delay", 6000),
    //                 NBTElement::Int("block_search_extent", 8),
    //                 NBTElement::Double("offset", 2.0)
    //             }),
    //             NBTElement::Int("sky_color", 8625919),
    //             NBTElement::Int("fog_color", 12638463),
    //             NBTElement::Int("water_color", 4020182),
    //             NBTElement::Int("water_fog_color", 329011)
    //         })
    //     }) },
        builtinNetherBiomeData("soul_sand_valley", 0.00625f, "ash"),
        builtinJungleBiomeData("sparse_jungle", 0.8f),
    //     builtinPeaksBiomeData("stony_peaks", 0.3f, 1.0f, 7776511),
    //     { "minecraft:stony_shore", NBTElement::Compound({
    //         NBTElement::Byte("has_precipitation", true),
    //         NBTElement::Float("temperature", 0.2f),
    //         NBTElement::Float("downfall", 0.3f),
    //         NBTElement::Compound("effects", {
    //             NBTElement::Compound("mood_sound", {
    //                 NBTElement::String("sound", "minecraft:ambient.cave"),
    //                 NBTElement::Int("tick_delay", 6000),
    //                 NBTElement::Int("block_search_extent", 8),
    //                 NBTElement::Double("offset", 2.0)
    //             }),
    //             NBTElement::Int("sky_color", 8233727),
    //             NBTElement::Int("fog_color", 12638463),
    //             NBTElement::Int("water_color", 4159204),
    //             NBTElement::Int("water_fog_color", 329011)
    //         })
    //     }) },
    //     builtinPlainsBiomeData("sunflower_plains"),
    //     { "minecraft:swamp", NBTElement::Compound({
    //         NBTElement::Byte("has_precipitation", true),
    //         NBTElement::Float("temperature", 0.8f),
    //         NBTElement::Float("downfall", 0.9f),
    //         NBTElement::Compound("effects", {
    //             NBTElement::Compound("mood_sound", {
    //                 NBTElement::String("sound", "minecraft:ambient.cave"),
    //                 NBTElement::Int("tick_delay", 6000),
    //                 NBTElement::Int("block_search_extent", 8),
    //                 NBTElement::Double("offset", 2.0)
    //             }),
    //             NBTElement::Compound("music", {
    //                 NBTElement::String("sound", "minecraft:music.overworld.swamp"),
    //                 NBTElement::Int("min_delay", 12000),
    //                 NBTElement::Int("max_delay", 24000),
    //                 NBTElement::Byte("replace_current_music", false)
    //             }),
    //             NBTElement::String("grass_color_modifier", "swamp"),
    //             NBTElement::Int("sky_color", 7907327),
    //             NBTElement::Int("dry_foliage_color", 8082228),
    //             NBTElement::Int("foliage_color", 6975545),
    //             NBTElement::Int("fog_color", 12638463),
    //             NBTElement::Int("water_color", 6388580),
    //             NBTElement::Int("water_fog_color", 2302743)
    //         })
    //     }) },
    //     builtinTaigaBiomeData("taiga", "taiga", 8233983),
    //     builtinEndBiomeData("the_end"),
    //     builtinLushCaveBiomeData("the_void", false),
        builtinNetherBiomeData("warped_forest", 0.01428f, "warped_spore"),
        // builtinWindsweptBiomeData("minecraft:windswept_forest"),
        // builtinWindsweptBiomeData("minecraft:windswept_gravelly_hills"),
        // builtinWindsweptBiomeData("minecraft:windswept_hills"),
        builtinDesertBiomeData("windswept_savanna", false),
    //     builtinBadlandsBiomeData("wooded_badlands")
    } },
    { "pig_variant", builtinTemperatureMobVariantData("pig") },
    { "frog_variant", builtinTemperatureMobVariantData("frog") },
    { "cow_variant", builtinTemperatureMobVariantData("cow") },
    { "chicken_variant", builtinTemperatureMobVariantData("chicken") },
    { "wolf_variant", {
        builtinWolfData("ashen", { "snowy_taiga" }), 
        builtinWolfData("chestnut", { "old_growth_pine_taiga" }),
        builtinWolfData("pale", { "old_growth_spruce_taiga" }),
        builtinWolfData("rusty", { "sparse_jungle" }),
        builtinWolfData("snowy", { "grove" }),
        builtinWolfData("spotted", { "savanna_plateau" }),
        builtinWolfData("striped", { "wooded_badlands" }),
            builtinWolfData("woods", { "forest" })
    } },
    { "wolf_sound_variant", {
        builtinWolfSoundData("angry", true), 
        builtinWolfSoundData("big", true), 
        builtinWolfSoundData("classic", false),
        builtinWolfSoundData("cute", true), 
        builtinWolfSoundData("grumpy", true), 
        builtinWolfSoundData("puglin", true),
        builtinWolfSoundData("sad", true)
    } },
    { "test_environment", {
        { "minecraft:default", NBTElement::Compound({ 
            NBTElement::String("type", "minecraft:all_of"), NBTElement::List("definitions", {}) 
        }) }
    } },
    { "test_instance", {
        { "minecrat:always_pass", NBTElement::Compound({
            NBTElement::Byte("required", false), NBTElement::Int("max_ticks", 1), NBTElement::Int("setup_ticks", 1),
            NBTElement::String("function", "minecraft:always_pass"), NBTElement::String("environment", "minecraft:default"),
            NBTElement::String("structure", "minecraft:empty"), NBTElement::String("type", "minecraft:function")
        }) }
    } },
    { "painting_variant", {
        builtinPainitingData("alban", 1, 1), builtinPainitingData("aztec", 1, 1), builtinPainitingData("aztec2", 1, 1), 
        builtinPainitingData("backyard", 3, 4), builtinPainitingData("baroque", 2, 2), builtinPainitingData("bomb", 1, 1),
        builtinPainitingData("bouquet", 3, 3), builtinPainitingData("burning_skull", 4, 4), builtinPainitingData("bust", 2, 2),
        builtinPainitingData("cavebird", 3, 3), builtinPainitingData("changing", 4, 2), builtinPainitingData("cotan", 3, 3),
        builtinPainitingData("courbet", 2, 1), builtinPainitingData("creebet", 2, 1), builtinPainitingData("donkey_kong", 4, 3),
        builtinPainitingData("earth", 2, 2, false), builtinPainitingData("endboss", 3, 3), builtinPainitingData("fern", 3, 3),
        builtinPainitingData("fighters", 4, 2), builtinPainitingData("finding", 4, 2), builtinPainitingData("fire", 2, 2, false),
        builtinPainitingData("graham", 1, 2), builtinPainitingData("humble", 2, 2), builtinPainitingData("kebab", 1, 1), 
        builtinPainitingData("lowmist", 4, 2), builtinPainitingData("match", 2, 2), builtinPainitingData("meditative", 1, 1),
        builtinPainitingData("orb", 4, 4), builtinPainitingData("owlemons", 3, 3), builtinPainitingData("passage", 4, 2),
        builtinPainitingData("pigscene", 4, 4), builtinPainitingData("plant", 1, 1), builtinPainitingData("pointer", 4, 4),
        builtinPainitingData("pond", 3, 4), builtinPainitingData("pool", 2, 1), builtinPainitingData("prairie_ride", 1, 2),
        builtinPainitingData("sea", 2, 1), builtinPainitingData("skeleton", 4, 3), builtinPainitingData("skull_and_roses", 2, 2),
        builtinPainitingData("stage", 2, 2), builtinPainitingData("sunflowers", 3, 3), builtinPainitingData("sunset", 2, 1),
        builtinPainitingData("tides", 3, 3), builtinPainitingData("unpacked", 4, 4), builtinPainitingData("void", 2, 2),
        builtinPainitingData("wanderer", 1, 2), builtinPainitingData("wasteland", 1, 1), builtinPainitingData("water", 2, 2, false),
        builtinPainitingData("wind", 2, 2, false), builtinPainitingData("wither", 2, 2, false)
    } },
    { "cat_variant", {
        builtinMobVariantData("cat", "all_black"), builtinMobVariantData("cat", "black"), builtinMobVariantData("cat", "british_shorthair"),
        builtinMobVariantData("cat", "tabby"), builtinMobVariantData("cat", "ragdoll"), builtinMobVariantData("cat", "persian"),
        builtinMobVariantData("cat", "jellie"), builtinMobVariantData("cat", "calico"), builtinMobVariantData("cat", "red"),
        builtinMobVariantData("cat", "siamese"), builtinMobVariantData("cat", "white")
    } },
    { "instrument", {
        builtinGoatHorn("admire_goat_horn", 4),
        builtinGoatHorn("call_goat_horn", 5),
        builtinGoatHorn("dream_goat_horn", 7),
        builtinGoatHorn("feel_goat_horn", 3),
        builtinGoatHorn("ponder_goat_horn", 0),
        builtinGoatHorn("seek_goat_horn", 2),
        builtinGoatHorn("sing_goat_horn", 1),
        builtinGoatHorn("yearn_goat_horn", 6)
    } },
    { "jukebox_song", {
        builtinJukeBoxSongHorn("11", 71.0f, 11), 
        builtinJukeBoxSongHorn("13", 178.0f, 1),
        builtinJukeBoxSongHorn("5", 178.0f, 15),
        builtinJukeBoxSongHorn("blocks", 345.0f, 3),
        builtinJukeBoxSongHorn("cat", 185.0f, 2),
        builtinJukeBoxSongHorn("chirp", 185.0f, 4),
        builtinJukeBoxSongHorn("creator", 176.0f, 12),
        builtinJukeBoxSongHorn("creator_music_box", 73.0f, 11),
        builtinJukeBoxSongHorn("far", 174.0f, 5),
        builtinJukeBoxSongHorn("mall", 197.0f, 6),
        builtinJukeBoxSongHorn("mellohi", 96.0f, 7),
        builtinJukeBoxSongHorn("otherside", 195.0f, 14),
        builtinJukeBoxSongHorn("pigstep", 149.0f, 13),
        builtinJukeBoxSongHorn("precipice", 299.0f, 13),
        builtinJukeBoxSongHorn("relic", 218.0f, 14),
        builtinJukeBoxSongHorn("stal", 150.0f, 8),
        builtinJukeBoxSongHorn("strad", 188.0f, 9),
        builtinJukeBoxSongHorn("wait", 238.0f, 12),
        builtinJukeBoxSongHorn("ward", 251.0f, 10)
    } },
    { "enchantment", {
        builtinEnchantment("aqua_affinity", 1, 0, 41, 0, 4, { "head" }, "#minecraft:enchantable/head_armor", 2, 1, {
            NBTElement::List("minecraft:attributes", {
                attributeBuilder("enchantment.aqua_affinity", "submerged_mining_speed", "add_multiplied_total", 4.0f, 4.0f, "linear")
            })
        }),
        builtinEnchantment("binding_curse", 25, 0, 50, 0, 8, { "armor" }, "#minecraft:enchantable/equippable", 1, 1, {
            NBTElement::Compound("minecraft:prevent_armor_change",{})
        }),
        builtinEnchantment("vanishing_curse", 25, 0, 50, 0, 8, { "any" }, "#minecraft:enchantable/vanishing", 1, 1, {
            NBTElement::Compound("minecraft:prevent_equipment_drop",{})
        }),
        builtinEnchantment("blast_protection", 5, 8, 13, 8, 4, { "armor" }, "#minecraft:enchantable/armor", 2, 4, {
            NBTElement::List("minecraft:attributes", {
                attributeBuilder("enchantment.blast_protection", "explosion_knockback_resistance", "add_value", 0.15f, 0.15f, "linear")
            }),
            NBTElement::List("minecraft:damage_protection", {NBTElement::Compound({
                NBTElement::Compound("effect", {
                    NBTElement::String("type", "minecraft:add"),
                    NBTElement::Compound("value", {
                        NBTElement::Float("base", 2.0f),
                        NBTElement::Float("per_level_above_first", 2.0f),
                        NBTElement::String("type", "minecraft:linear")
                    })
                }),
                NBTElement::Compound("requirements", {
                    NBTElement::Compound("predicate", {NBTElement::List("tags", {
                        NBTElement::Compound({
                            NBTElement::Byte("expected", true),
                            NBTElement::String("id", "minecraft:is_explosion")
                        }),
                        NBTElement::Compound({
                            NBTElement::Byte("expected", false),
                            NBTElement::String("id", "minecraft:bypasses_invulnerability")
                        })
                    })}),
                    NBTElement::String("condition", "minecraft:damage_source_properties")
                })
            })})
        }, "#minecraft:exclusive_set/armor"),
        builtinEnchantment("depth_strider", 10, 10, 10, 10, 4, { "feet" }, "#minecraft:enchantable/foot_armor", 2, 3, {
            NBTElement::List("minecraft:attributes", {
                attributeBuilder("enchantment.depth_strider", "water_movement_efficiency", "add_value", 0.33333334f, 0.33333334f, "linear")
            })
        }, "#minecraft:exclusive_set/boots"),
        builtinEnchantment("feather_falling", 5, 6, 11, 6, 2, { "armor" }, "#minecraft:enchantable/foot_armor", 5, 4, {
            NBTElement::List("minecraft:damage_protection", {NBTElement::Compound({
                NBTElement::Compound("effect", {
                    NBTElement::String("type", "minecraft:add"),
                    NBTElement::Compound("value", {
                        NBTElement::Float("base", 3.0f),
                        NBTElement::Float("per_level_above_first", 3.0f),
                        NBTElement::String("type", "minecraft:linear")
                    })
                }),
                NBTElement::Compound("requirements", {
                    NBTElement::Compound("predicate", {NBTElement::List("tags", {
                        NBTElement::Compound({
                            NBTElement::Byte("expected", true),
                            NBTElement::String("id", "minecraft:is_fall")
                        }),
                        NBTElement::Compound({
                            NBTElement::Byte("expected", false),
                            NBTElement::String("id", "minecraft:bypasses_invulnerability")
                        })
                    })}),
                    NBTElement::String("condition", "minecraft:damage_source_properties")
                })
            })})
        }),
        builtinEnchantment("protection", 1, 11, 12, 11, 1, { "armor" }, "#minecraft:enchantable/armor", 10, 4, {
            NBTElement::List("minecraft:damage_protection", {NBTElement::Compound({
                NBTElement::Compound("effect", {
                    NBTElement::String("type", "minecraft:add"),
                    NBTElement::Compound("value", {
                        NBTElement::Float("base", 1.0f),
                        NBTElement::Float("per_level_above_first", 1.0f),
                        NBTElement::String("type", "minecraft:linear")
                    })
                }),
                NBTElement::Compound("requirements", {
                    NBTElement::Compound("predicate", {NBTElement::List("tags", {
                        NBTElement::Compound({
                            NBTElement::Byte("expected", false),
                            NBTElement::String("id", "minecraft:bypasses_invulnerability")
                        })
                    })}),
                    NBTElement::String("condition", "minecraft:damage_source_properties")
                })
            })})
        }, "#minecraft:exclusive_set/armor"),
        builtinEnchantment("fire_protection", 10, 8, 18, 8, 2, { "armor" }, "#minecraft:enchantable/armor", 5, 4, {
            NBTElement::List("minecraft:attributes", {
                attributeBuilder("enchantment.fire_protection", "burning_time", "add_value", -0.15f, -0.15f, "linear")
            }),
            NBTElement::List("minecraft:damage_protection", {NBTElement::Compound({
                NBTElement::Compound("effect", {
                    NBTElement::String("type", "minecraft:add"),
                    NBTElement::Compound("value", {
                        NBTElement::Float("base", 2.0f),
                        NBTElement::Float("per_level_above_first", 2.0f),
                        NBTElement::String("type", "minecraft:linear")
                    })
                }),
                NBTElement::Compound("requirements", {
                    NBTElement::Compound("predicate", {NBTElement::List("tags", {
                        NBTElement::Compound({
                            NBTElement::Byte("expected", true),
                            NBTElement::String("id", "minecraft:is_fire")
                        }),
                        NBTElement::Compound({
                            NBTElement::Byte("expected", false),
                            NBTElement::String("id", "minecraft:bypasses_invulnerability")
                        })
                    })}),
                    NBTElement::String("condition", "minecraft:all_of")
                })
            })})
        }, "#minecraft:exclusive_set/armor"),
        builtinEnchantment("projectile_protection", 3, 6, 9, 6, 2, { "armor" }, "#minecraft:enchantable/armor", 5, 4, {
            NBTElement::List("minecraft:damage_protection", {NBTElement::Compound({
                NBTElement::Compound("effect", {
                    NBTElement::String("type", "minecraft:add"),
                    NBTElement::Compound("value", {
                        NBTElement::Float("base", 2.0f),
                        NBTElement::Float("per_level_above_first", 2.0f),
                        NBTElement::String("type", "minecraft:linear")
                    })
                }),
                NBTElement::Compound("requirements", {
                    NBTElement::Compound("predicate", {NBTElement::List("tags", {
                        NBTElement::Compound({
                            NBTElement::Byte("expected", true),
                            NBTElement::String("id", "minecraft:is_projectile")
                        }),
                        NBTElement::Compound({
                            NBTElement::Byte("expected", false),
                            NBTElement::String("id", "minecraft:bypasses_invulnerability")
                        })
                    })}),
                    NBTElement::String("condition", "minecraft:damage_source_properties")
                })
            })})
        }, "#minecraft:exclusive_set/armor"),
    } }
};
    // "bane_of_arthropods": {
    //   "min_cost": {
    //     "base": 5,
    //     "per_level_above_first": 8
    //   },
    //   "max_cost": {
    //     "base": 25,
    //     "per_level_above_first": 8
    //   },
    //   "anvil_cost": 2,
    //   "slots": [
    //     "mainhand"
    //   ],
    //   "supported_items": "#minecraft:enchantable/weapon",
    //   "primary_items": "#minecraft:enchantable/sword",
    //   "weight": 5,
    //   "max_level": 5,
    //   "exclusive_set": "#minecraft:exclusive_set/damage",
    //   "effects": {
    //     "minecraft:damage": [
    //       {
    //         "effect": {
    //           "value": {
    //             "base": 2.5,
    //             "per_level_above_first": 2.5,
    //             "type": "minecraft:linear"
    //           },
    //           "type": "minecraft:add"
    //         },
    //         "requirements": {
    //           "predicate": {
    //             "type": "#minecraft:sensitive_to_bane_of_arthropods"
    //           },
    //           "entity": "this",
    //           "condition": "minecraft:entity_properties"
    //         }
    //       }
    //     ],
    //     "minecraft:post_attack": [
    //       {
    //         "enchanted": "attacker",
    //         "affected": "victim",
    //         "effect": {
    //           "max_duration": {
    //             "base": 1.5,
    //             "per_level_above_first": 0.5,
    //             "type": "minecraft:linear"
    //           },
    //           "min_amplifier": 3.0,
    //           "max_amplifier": 3.0,
    //           "to_apply": "minecraft:slowness",
    //           "min_duration": 1.5,
    //           "type": "minecraft:apply_mob_effect"
    //         },
    //         "requirements": {
    //           "terms": [
    //             {
    //               "predicate": {
    //                 "type": "#minecraft:sensitive_to_bane_of_arthropods"
    //               },
    //               "entity": "this",
    //               "condition": "minecraft:entity_properties"
    //             },
    //             {
    //               "predicate": {
    //                 "is_direct": true
    //               },
    //               "condition": "minecraft:damage_source_properties"
    //             }
    //           ],
    //           "condition": "minecraft:all_of"
    //         }
    //       }
    //     ]
    //   }
    // },
    // "breach": {
    //   "description": {
    //     "translate": "enchantment.minecraft.breach"
    //   },
    //   "min_cost": {
    //     "base": 15,
    //     "per_level_above_first": 9
    //   },
    //   "max_cost": {
    //     "base": 65,
    //     "per_level_above_first": 9
    //   },
    //   "anvil_cost": 4,
    //   "slots": [
    //     "mainhand"
    //   ],
    //   "supported_items": "#minecraft:enchantable/mace",
    //   "weight": 2,
    //   "max_level": 4,
    //   "exclusive_set": "#minecraft:exclusive_set/damage",
    //   "effects": {
    //     "minecraft:armor_effectiveness": [
    //       {
    //         "effect": {
    //           "value": {
    //             "base": -0.15,
    //             "per_level_above_first": -0.15,
    //             "type": "minecraft:linear"
    //           },
    //           "type": "minecraft:add"
    //         }
    //       }
    //     ]
    //   }
    // },
    // "channeling": {
    //   "description": {
    //     "translate": "enchantment.minecraft.channeling"
    //   },
    //   "min_cost": {
    //     "base": 25,
    //     "per_level_above_first": 0
    //   },
    //   "max_cost": {
    //     "base": 50,
    //     "per_level_above_first": 0
    //   },
    //   "anvil_cost": 8,
    //   "slots": [
    //     "mainhand"
    //   ],
    //   "supported_items": "#minecraft:enchantable/trident",
    //   "weight": 1,
    //   "max_level": 1,
    //   "effects": {
    //     "minecraft:hit_block": [
    //       {
    //         "effect": {
    //           "effects": [
    //             {
    //               "entity": "minecraft:lightning_bolt",
    //               "type": "minecraft:summon_entity"
    //             },
    //             {
    //               "sound": "minecraft:item.trident.thunder",
    //               "volume": 5.0,
    //               "pitch": 1.0,
    //               "type": "minecraft:play_sound"
    //             }
    //           ],
    //           "type": "minecraft:all_of"
    //         },
    //         "requirements": {
    //           "terms": [
    //             {
    //               "thundering": true,
    //               "condition": "minecraft:weather_check"
    //             },
    //             {
    //               "predicate": {
    //                 "type": "minecraft:trident"
    //               },
    //               "entity": "this",
    //               "condition": "minecraft:entity_properties"
    //             },
    //             {
    //               "predicate": {
    //                 "can_see_sky": true
    //               },
    //               "condition": "minecraft:location_check"
    //             },
    //             {
    //               "block": "minecraft:lightning_rod",
    //               "condition": "minecraft:block_state_property"
    //             }
    //           ],
    //           "condition": "minecraft:all_of"
    //         }
    //       }
    //     ],
    //     "minecraft:post_attack": [
    //       {
    //         "enchanted": "attacker",
    //         "affected": "victim",
    //         "effect": {
    //           "effects": [
    //             {
    //               "entity": "minecraft:lightning_bolt",
    //               "type": "minecraft:summon_entity"
    //             },
    //             {
    //               "sound": "minecraft:item.trident.thunder",
    //               "volume": 5.0,
    //               "pitch": 1.0,
    //               "type": "minecraft:play_sound"
    //             }
    //           ],
    //           "type": "minecraft:all_of"
    //         },
    //         "requirements": {
    //           "terms": [
    //             {
    //               "thundering": true,
    //               "condition": "minecraft:weather_check"
    //             },
    //             {
    //               "predicate": {
    //                 "location": {
    //                   "can_see_sky": true
    //                 }
    //               },
    //               "entity": "this",
    //               "condition": "minecraft:entity_properties"
    //             },
    //             {
    //               "predicate": {
    //                 "type": "minecraft:trident"
    //               },
    //               "entity": "direct_attacker",
    //               "condition": "minecraft:entity_properties"
    //             }
    //           ],
    //           "condition": "minecraft:all_of"
    //         }
    //       }
    //     ]
    //   }
    // },
    // "density": {
    //   "description": {
    //     "translate": "enchantment.minecraft.density"
    //   },
    //   "min_cost": {
    //     "base": 5,
    //     "per_level_above_first": 8
    //   },
    //   "max_cost": {
    //     "base": 25,
    //     "per_level_above_first": 8
    //   },
    //   "anvil_cost": 2,
    //   "slots": [
    //     "mainhand"
    //   ],
    //   "supported_items": "#minecraft:enchantable/mace",
    //   "weight": 5,
    //   "max_level": 5,
    //   "exclusive_set": "#minecraft:exclusive_set/damage",
    //   "effects": {
    //     "minecraft:smash_damage_per_fallen_block": [
    //       {
    //         "effect": {
    //           "value": {
    //             "base": 0.5,
    //             "per_level_above_first": 0.5,
    //             "type": "minecraft:linear"
    //           },
    //           "type": "minecraft:add"
    //         }
    //       }
    //     ]
    //   }
    // },
    // "efficiency": {
    //   "description": {
    //     "translate": "enchantment.minecraft.efficiency"
    //   },
    //   "min_cost": {
    //     "base": 1,
    //     "per_level_above_first": 10
    //   },
    //   "max_cost": {
    //     "base": 51,
    //     "per_level_above_first": 10
    //   },
    //   "anvil_cost": 1,
    //   "slots": [
    //     "mainhand"
    //   ],
    //   "supported_items": "#minecraft:enchantable/mining",
    //   "weight": 10,
    //   "max_level": 5,
    //   "effects": {
    //     "minecraft:attributes": [
    //       {
    //         "id": "minecraft:enchantment.efficiency",
    //         "attribute": "minecraft:mining_efficiency",
    //         "amount": {
    //           "added": 1.0,
    //           "type": "minecraft:levels_squared"
    //         },
    //         "operation": "add_value"
    //       }
    //     ]
    //   }
    // },
    // "fire_aspect": {
    //   "description": {
    //     "translate": "enchantment.minecraft.fire_aspect"
    //   },
    //   "min_cost": {
    //     "base": 10,
    //     "per_level_above_first": 20
    //   },
    //   "max_cost": {
    //     "base": 60,
    //     "per_level_above_first": 20
    //   },
    //   "anvil_cost": 4,
    //   "slots": [
    //     "mainhand"
    //   ],
    //   "supported_items": "#minecraft:enchantable/fire_aspect",
    //   "primary_items": "#minecraft:enchantable/sword",
    //   "weight": 2,
    //   "max_level": 2,
    //   "effects": {
    //     "minecraft:post_attack": [
    //       {
    //         "enchanted": "attacker",
    //         "affected": "victim",
    //         "effect": {
    //           "duration": {
    //             "base": 4.0,
    //             "per_level_above_first": 4.0,
    //             "type": "minecraft:linear"
    //           },
    //           "type": "minecraft:ignite"
    //         },
    //         "requirements": {
    //           "predicate": {
    //             "is_direct": true
    //           },
    //           "condition": "minecraft:damage_source_properties"
    //         }
    //       }
    //     ]
    //   }
    // },
    // "flame": {
    //   "description": {
    //     "translate": "enchantment.minecraft.flame"
    //   },
    //   "min_cost": {
    //     "base": 20,
    //     "per_level_above_first": 0
    //   },
    //   "max_cost": {
    //     "base": 50,
    //     "per_level_above_first": 0
    //   },
    //   "anvil_cost": 4,
    //   "slots": [
    //     "mainhand"
    //   ],
    //   "supported_items": "#minecraft:enchantable/bow",
    //   "weight": 2,
    //   "max_level": 1,
    //   "effects": {
    //     "minecraft:projectile_spawned": [
    //       {
    //         "effect": {
    //           "duration": 100.0,
    //           "type": "minecraft:ignite"
    //         }
    //       }
    //     ]
    //   }
    // },
    // "fortune": {
    //   "description": {
    //     "translate": "enchantment.minecraft.fortune"
    //   },
    //   "min_cost": {
    //     "base": 15,
    //     "per_level_above_first": 9
    //   },
    //   "max_cost": {
    //     "base": 65,
    //     "per_level_above_first": 9
    //   },
    //   "anvil_cost": 4,
    //   "slots": [
    //     "mainhand"
    //   ],
    //   "supported_items": "#minecraft:enchantable/mining_loot",
    //   "weight": 2,
    //   "max_level": 3,
    //   "exclusive_set": "#minecraft:exclusive_set/mining"
    // },
    // "frost_walker": {
    //   "description": {
    //     "translate": "enchantment.minecraft.frost_walker"
    //   },
    //   "min_cost": {
    //     "base": 10,
    //     "per_level_above_first": 10
    //   },
    //   "max_cost": {
    //     "base": 25,
    //     "per_level_above_first": 10
    //   },
    //   "anvil_cost": 4,
    //   "slots": [
    //     "feet"
    //   ],
    //   "supported_items": "#minecraft:enchantable/foot_armor",
    //   "weight": 2,
    //   "max_level": 2,
    //   "exclusive_set": "#minecraft:exclusive_set/boots",
    //   "effects": {
    //     "minecraft:damage_immunity": [
    //       {
    //         "effect": {},
    //         "requirements": {
    //           "predicate": {
    //             "tags": [
    //               {
    //                 "id": "minecraft:burn_from_stepping",
    //                 "expected": true
    //               },
    //               {
    //                 "id": "minecraft:bypasses_invulnerability",
    //                 "expected": false
    //               }
    //             ]
    //           },
    //           "condition": "minecraft:damage_source_properties"
    //         }
    //       }
    //     ],
    //     "minecraft:location_changed": [
    //       {
    //         "effect": {
    //           "predicate": {
    //             "predicates": [
    //               {
    //                 "offset": [
    //                   0,
    //                   1,
    //                   0
    //                 ],
    //                 "tag": "minecraft:air",
    //                 "type": "minecraft:matching_block_tag"
    //               },
    //               {
    //                 "blocks": "minecraft:water",
    //                 "type": "minecraft:matching_blocks"
    //               },
    //               {
    //                 "fluids": "minecraft:water",
    //                 "type": "minecraft:matching_fluids"
    //               },
    //               {
    //                 "type": "minecraft:unobstructed"
    //               }
    //             ],
    //             "type": "minecraft:all_of"
    //           },
    //           "block_state": {
    //             "state": {
    //               "Properties": {
    //                 "age": "0"
    //               },
    //               "Name": "minecraft:frosted_ice"
    //             },
    //             "type": "minecraft:simple_state_provider"
    //           },
    //           "trigger_game_event": "minecraft:block_place",
    //           "radius": {
    //             "value": {
    //               "base": 3.0,
    //               "per_level_above_first": 1.0,
    //               "type": "minecraft:linear"
    //             },
    //             "min": 0.0,
    //             "max": 16.0,
    //             "type": "minecraft:clamped"
    //           },
    //           "height": 1.0,
    //           "offset": [
    //             0,
    //             -1,
    //             0
    //           ],
    //           "type": "minecraft:replace_disk"
    //         },
    //         "requirements": {
    //           "terms": [
    //             {
    //               "predicate": {
    //                 "flags": {
    //                   "is_on_ground": true
    //                 }
    //               },
    //               "entity": "this",
    //               "condition": "minecraft:entity_properties"
    //             },
    //             {
    //               "term": {
    //                 "predicate": {
    //                   "vehicle": {}
    //                 },
    //                 "entity": "this",
    //                 "condition": "minecraft:entity_properties"
    //               },
    //               "condition": "minecraft:inverted"
    //             }
    //           ],
    //           "condition": "minecraft:all_of"
    //         }
    //       }
    //     ]
    //   }
    // },
    // "impaling": {
    //   "description": {
    //     "translate": "enchantment.minecraft.impaling"
    //   },
    //   "min_cost": {
    //     "base": 1,
    //     "per_level_above_first": 8
    //   },
    //   "max_cost": {
    //     "base": 21,
    //     "per_level_above_first": 8
    //   },
    //   "anvil_cost": 4,
    //   "slots": [
    //     "mainhand"
    //   ],
    //   "supported_items": "#minecraft:enchantable/trident",
    //   "weight": 2,
    //   "max_level": 5,
    //   "exclusive_set": "#minecraft:exclusive_set/damage",
    //   "effects": {
    //     "minecraft:damage": [
    //       {
    //         "effect": {
    //           "value": {
    //             "base": 2.5,
    //             "per_level_above_first": 2.5,
    //             "type": "minecraft:linear"
    //           },
    //           "type": "minecraft:add"
    //         },
    //         "requirements": {
    //           "predicate": {
    //             "type": "#minecraft:sensitive_to_impaling"
    //           },
    //           "entity": "this",
    //           "condition": "minecraft:entity_properties"
    //         }
    //       }
    //     ]
    //   }
    // },
    // "infinity": {
    //   "description": {
    //     "translate": "enchantment.minecraft.infinity"
    //   },
    //   "min_cost": {
    //     "base": 20,
    //     "per_level_above_first": 0
    //   },
    //   "max_cost": {
    //     "base": 50,
    //     "per_level_above_first": 0
    //   },
    //   "anvil_cost": 8,
    //   "slots": [
    //     "mainhand"
    //   ],
    //   "supported_items": "#minecraft:enchantable/bow",
    //   "weight": 1,
    //   "max_level": 1,
    //   "exclusive_set": "#minecraft:exclusive_set/bow",
    //   "effects": {
    //     "minecraft:ammo_use": [
    //       {
    //         "effect": {
    //           "value": 0.0,
    //           "type": "minecraft:set"
    //         },
    //         "requirements": {
    //           "predicate": {
    //             "items": "minecraft:arrow"
    //           },
    //           "condition": "minecraft:match_tool"
    //         }
    //       }
    //     ]
    //   }
    // },
    // "knockback": {
    //   "description": {
    //     "translate": "enchantment.minecraft.knockback"
    //   },
    //   "min_cost": {
    //     "base": 5,
    //     "per_level_above_first": 20
    //   },
    //   "max_cost": {
    //     "base": 55,
    //     "per_level_above_first": 20
    //   },
    //   "anvil_cost": 2,
    //   "slots": [
    //     "mainhand"
    //   ],
    //   "supported_items": "#minecraft:enchantable/sword",
    //   "weight": 5,
    //   "max_level": 2,
    //   "effects": {
    //     "minecraft:knockback": [
    //       {
    //         "effect": {
    //           "value": {
    //             "base": 1.0,
    //             "per_level_above_first": 1.0,
    //             "type": "minecraft:linear"
    //           },
    //           "type": "minecraft:add"
    //         }
    //       }
    //     ]
    //   }
    // },
    // "looting": {
    //   "description": {
    //     "translate": "enchantment.minecraft.looting"
    //   },
    //   "min_cost": {
    //     "base": 15,
    //     "per_level_above_first": 9
    //   },
    //   "max_cost": {
    //     "base": 65,
    //     "per_level_above_first": 9
    //   },
    //   "anvil_cost": 4,
    //   "slots": [
    //     "mainhand"
    //   ],
    //   "supported_items": "#minecraft:enchantable/sword",
    //   "weight": 2,
    //   "max_level": 3,
    //   "effects": {
    //     "minecraft:equipment_drops": [
    //       {
    //         "enchanted": "attacker",
    //         "effect": {
    //           "value": {
    //             "base": 0.01,
    //             "per_level_above_first": 0.01,
    //             "type": "minecraft:linear"
    //           },
    //           "type": "minecraft:add"
    //         },
    //         "requirements": {
    //           "predicate": {
    //             "type": "minecraft:player"
    //           },
    //           "entity": "attacker",
    //           "condition": "minecraft:entity_properties"
    //         }
    //       }
    //     ]
    //   }
    // },
    // "loyalty": {
    //   "description": {
    //     "translate": "enchantment.minecraft.loyalty"
    //   },
    //   "min_cost": {
    //     "base": 12,
    //     "per_level_above_first": 7
    //   },
    //   "max_cost": {
    //     "base": 50,
    //     "per_level_above_first": 0
    //   },
    //   "anvil_cost": 2,
    //   "slots": [
    //     "mainhand"
    //   ],
    //   "supported_items": "#minecraft:enchantable/trident",
    //   "weight": 5,
    //   "max_level": 3,
    //   "effects": {
    //     "minecraft:trident_return_acceleration": [
    //       {
    //         "effect": {
    //           "value": {
    //             "base": 1.0,
    //             "per_level_above_first": 1.0,
    //             "type": "minecraft:linear"
    //           },
    //           "type": "minecraft:add"
    //         }
    //       }
    //     ]
    //   }
    // },
    // "luck_of_the_sea": {
    //   "description": {
    //     "translate": "enchantment.minecraft.luck_of_the_sea"
    //   },
    //   "min_cost": {
    //     "base": 15,
    //     "per_level_above_first": 9
    //   },
    //   "max_cost": {
    //     "base": 65,
    //     "per_level_above_first": 9
    //   },
    //   "anvil_cost": 4,
    //   "slots": [
    //     "mainhand"
    //   ],
    //   "supported_items": "#minecraft:enchantable/fishing",
    //   "weight": 2,
    //   "max_level": 3,
    //   "effects": {
    //     "minecraft:fishing_luck_bonus": [
    //       {
    //         "effect": {
    //           "value": {
    //             "base": 1.0,
    //             "per_level_above_first": 1.0,
    //             "type": "minecraft:linear"
    //           },
    //           "type": "minecraft:add"
    //         }
    //       }
    //     ]
    //   }
    // },
    // "lure": {
    //   "description": {
    //     "translate": "enchantment.minecraft.lure"
    //   },
    //   "min_cost": {
    //     "base": 15,
    //     "per_level_above_first": 9
    //   },
    //   "max_cost": {
    //     "base": 65,
    //     "per_level_above_first": 9
    //   },
    //   "anvil_cost": 4,
    //   "slots": [
    //     "mainhand"
    //   ],
    //   "supported_items": "#minecraft:enchantable/fishing",
    //   "weight": 2,
    //   "max_level": 3,
    //   "effects": {
    //     "minecraft:fishing_time_reduction": [
    //       {
    //         "effect": {
    //           "value": {
    //             "base": 5.0,
    //             "per_level_above_first": 5.0,
    //             "type": "minecraft:linear"
    //           },
    //           "type": "minecraft:add"
    //         }
    //       }
    //     ]
    //   }
    // },
    // "mending": {
    //   "description": {
    //     "translate": "enchantment.minecraft.mending"
    //   },
    //   "min_cost": {
    //     "base": 25,
    //     "per_level_above_first": 25
    //   },
    //   "max_cost": {
    //     "base": 75,
    //     "per_level_above_first": 25
    //   },
    //   "anvil_cost": 4,
    //   "slots": [
    //     "any"
    //   ],
    //   "supported_items": "#minecraft:enchantable/durability",
    //   "weight": 2,
    //   "max_level": 1,
    //   "effects": {
    //     "minecraft:repair_with_xp": [
    //       {
    //         "effect": {
    //           "factor": 2.0,
    //           "type": "minecraft:multiply"
    //         }
    //       }
    //     ]
    //   }
    // },
    // "multishot": {
    //   "description": {
    //     "translate": "enchantment.minecraft.multishot"
    //   },
    //   "min_cost": {
    //     "base": 20,
    //     "per_level_above_first": 0
    //   },
    //   "max_cost": {
    //     "base": 50,
    //     "per_level_above_first": 0
    //   },
    //   "anvil_cost": 4,
    //   "slots": [
    //     "mainhand"
    //   ],
    //   "supported_items": "#minecraft:enchantable/crossbow",
    //   "weight": 2,
    //   "max_level": 1,
    //   "exclusive_set": "#minecraft:exclusive_set/crossbow",
    //   "effects": {
    //     "minecraft:projectile_count": [
    //       {
    //         "effect": {
    //           "value": {
    //             "base": 2.0,
    //             "per_level_above_first": 2.0,
    //             "type": "minecraft:linear"
    //           },
    //           "type": "minecraft:add"
    //         }
    //       }
    //     ],
    //     "minecraft:projectile_spread": [
    //       {
    //         "effect": {
    //           "value": {
    //             "base": 10.0,
    //             "per_level_above_first": 10.0,
    //             "type": "minecraft:linear"
    //           },
    //           "type": "minecraft:add"
    //         }
    //       }
    //     ]
    //   }
    // },
    // "piercing": {
    //   "description": {
    //     "translate": "enchantment.minecraft.piercing"
    //   },
    //   "min_cost": {
    //     "base": 1,
    //     "per_level_above_first": 10
    //   },
    //   "max_cost": {
    //     "base": 50,
    //     "per_level_above_first": 0
    //   },
    //   "anvil_cost": 1,
    //   "slots": [
    //     "mainhand"
    //   ],
    //   "supported_items": "#minecraft:enchantable/crossbow",
    //   "weight": 10,
    //   "max_level": 4,
    //   "exclusive_set": "#minecraft:exclusive_set/crossbow",
    //   "effects": {
    //     "minecraft:projectile_piercing": [
    //       {
    //         "effect": {
    //           "value": {
    //             "base": 1.0,
    //             "per_level_above_first": 1.0,
    //             "type": "minecraft:linear"
    //           },
    //           "type": "minecraft:add"
    //         }
    //       }
    //     ]
    //   }
    // },
    // "power": {
    //   "description": {
    //     "translate": "enchantment.minecraft.power"
    //   },
    //   "min_cost": {
    //     "base": 1,
    //     "per_level_above_first": 10
    //   },
    //   "max_cost": {
    //     "base": 16,
    //     "per_level_above_first": 10
    //   },
    //   "anvil_cost": 1,
    //   "slots": [
    //     "mainhand"
    //   ],
    //   "supported_items": "#minecraft:enchantable/bow",
    //   "weight": 10,
    //   "max_level": 5,
    //   "effects": {
    //     "minecraft:damage": [
    //       {
    //         "effect": {
    //           "value": {
    //             "base": 1.0,
    //             "per_level_above_first": 0.5,
    //             "type": "minecraft:linear"
    //           },
    //           "type": "minecraft:add"
    //         },
    //         "requirements": {
    //           "predicate": {
    //             "type": "#minecraft:arrows"
    //           },
    //           "entity": "direct_attacker",
    //           "condition": "minecraft:entity_properties"
    //         }
    //       }
    //     ]
    //   }
    // },
    // "punch": {
    //   "description": {
    //     "translate": "enchantment.minecraft.punch"
    //   },
    //   "min_cost": {
    //     "base": 12,
    //     "per_level_above_first": 20
    //   },
    //   "max_cost": {
    //     "base": 37,
    //     "per_level_above_first": 20
    //   },
    //   "anvil_cost": 4,
    //   "slots": [
    //     "mainhand"
    //   ],
    //   "supported_items": "#minecraft:enchantable/bow",
    //   "weight": 2,
    //   "max_level": 2,
    //   "effects": {
    //     "minecraft:knockback": [
    //       {
    //         "effect": {
    //           "value": {
    //             "base": 1.0,
    //             "per_level_above_first": 1.0,
    //             "type": "minecraft:linear"
    //           },
    //           "type": "minecraft:add"
    //         },
    //         "requirements": {
    //           "predicate": {
    //             "type": "#minecraft:arrows"
    //           },
    //           "entity": "direct_attacker",
    //           "condition": "minecraft:entity_properties"
    //         }
    //       }
    //     ]
    //   }
    // },
    // "quick_charge": {
    //   "description": {
    //     "translate": "enchantment.minecraft.quick_charge"
    //   },
    //   "min_cost": {
    //     "base": 12,
    //     "per_level_above_first": 20
    //   },
    //   "max_cost": {
    //     "base": 50,
    //     "per_level_above_first": 0
    //   },
    //   "anvil_cost": 2,
    //   "slots": [
    //     "mainhand",
    //     "offhand"
    //   ],
    //   "supported_items": "#minecraft:enchantable/crossbow",
    //   "weight": 5,
    //   "max_level": 3,
    //   "effects": {
    //     "minecraft:crossbow_charge_time": {
    //       "value": {
    //         "base": -0.25,
    //         "per_level_above_first": -0.25,
    //         "type": "minecraft:linear"
    //       },
    //       "type": "minecraft:add"
    //     },
    //     "minecraft:crossbow_charging_sounds": [
    //       {
    //         "start": "minecraft:item.crossbow.quick_charge_1",
    //         "end": "minecraft:item.crossbow.loading_end"
    //       },
    //       {
    //         "start": "minecraft:item.crossbow.quick_charge_2",
    //         "end": "minecraft:item.crossbow.loading_end"
    //       },
    //       {
    //         "start": "minecraft:item.crossbow.quick_charge_3",
    //         "end": "minecraft:item.crossbow.loading_end"
    //       }
    //     ]
    //   }
    // },
    // "respiration": {
    //   "description": {
    //     "translate": "enchantment.minecraft.respiration"
    //   },
    //   "min_cost": {
    //     "base": 10,
    //     "per_level_above_first": 10
    //   },
    //   "max_cost": {
    //     "base": 40,
    //     "per_level_above_first": 10
    //   },
    //   "anvil_cost": 4,
    //   "slots": [
    //     "head"
    //   ],
    //   "supported_items": "#minecraft:enchantable/head_armor",
    //   "weight": 2,
    //   "max_level": 3,
    //   "effects": {
    //     "minecraft:attributes": [
    //       {
    //         "id": "minecraft:enchantment.respiration",
    //         "attribute": "minecraft:oxygen_bonus",
    //         "amount": {
    //           "base": 1.0,
    //           "per_level_above_first": 1.0,
    //           "type": "minecraft:linear"
    //         },
    //         "operation": "add_value"
    //       }
    //     ]
    //   }
    // },
    // "riptide": {
    //   "description": {
    //     "translate": "enchantment.minecraft.riptide"
    //   },
    //   "min_cost": {
    //     "base": 17,
    //     "per_level_above_first": 7
    //   },
    //   "max_cost": {
    //     "base": 50,
    //     "per_level_above_first": 0
    //   },
    //   "anvil_cost": 4,
    //   "slots": [
    //     "hand"
    //   ],
    //   "supported_items": "#minecraft:enchantable/trident",
    //   "weight": 2,
    //   "max_level": 3,
    //   "exclusive_set": "#minecraft:exclusive_set/riptide",
    //   "effects": {
    //     "minecraft:trident_sound": [
    //       "minecraft:item.trident.riptide_1",
    //       "minecraft:item.trident.riptide_2",
    //       "minecraft:item.trident.riptide_3"
    //     ],
    //     "minecraft:trident_spin_attack_strength": {
    //       "value": {
    //         "base": 1.5,
    //         "per_level_above_first": 0.75,
    //         "type": "minecraft:linear"
    //       },
    //       "type": "minecraft:add"
    //     }
    //   }
    // },
    // "sharpness": {
    //   "description": {
    //     "translate": "enchantment.minecraft.sharpness"
    //   },
    //   "min_cost": {
    //     "base": 1,
    //     "per_level_above_first": 11
    //   },
    //   "max_cost": {
    //     "base": 21,
    //     "per_level_above_first": 11
    //   },
    //   "anvil_cost": 1,
    //   "slots": [
    //     "mainhand"
    //   ],
    //   "supported_items": "#minecraft:enchantable/sharp_weapon",
    //   "primary_items": "#minecraft:enchantable/sword",
    //   "weight": 10,
    //   "max_level": 5,
    //   "exclusive_set": "#minecraft:exclusive_set/damage",
    //   "effects": {
    //     "minecraft:damage": [
    //       {
    //         "effect": {
    //           "value": {
    //             "base": 1.0,
    //             "per_level_above_first": 0.5,
    //             "type": "minecraft:linear"
    //           },
    //           "type": "minecraft:add"
    //         }
    //       }
    //     ]
    //   }
    // },
    // "silk_touch": {
    //   "description": {
    //     "translate": "enchantment.minecraft.silk_touch"
    //   },
    //   "min_cost": {
    //     "base": 15,
    //     "per_level_above_first": 0
    //   },
    //   "max_cost": {
    //     "base": 65,
    //     "per_level_above_first": 0
    //   },
    //   "anvil_cost": 8,
    //   "slots": [
    //     "mainhand"
    //   ],
    //   "supported_items": "#minecraft:enchantable/mining_loot",
    //   "weight": 1,
    //   "max_level": 1,
    //   "exclusive_set": "#minecraft:exclusive_set/mining",
    //   "effects": {
    //     "minecraft:block_experience": [
    //       {
    //         "effect": {
    //           "value": 0.0,
    //           "type": "minecraft:set"
    //         }
    //       }
    //     ]
    //   }
    // },
    // "smite": {
    //   "description": {
    //     "translate": "enchantment.minecraft.smite"
    //   },
    //   "min_cost": {
    //     "base": 5,
    //     "per_level_above_first": 8
    //   },
    //   "max_cost": {
    //     "base": 25,
    //     "per_level_above_first": 8
    //   },
    //   "anvil_cost": 2,
    //   "slots": [
    //     "mainhand"
    //   ],
    //   "supported_items": "#minecraft:enchantable/weapon",
    //   "primary_items": "#minecraft:enchantable/sword",
    //   "weight": 5,
    //   "max_level": 5,
    //   "exclusive_set": "#minecraft:exclusive_set/damage",
    //   "effects": {
    //     "minecraft:damage": [
    //       {
    //         "effect": {
    //           "value": {
    //             "base": 2.5,
    //             "per_level_above_first": 2.5,
    //             "type": "minecraft:linear"
    //           },
    //           "type": "minecraft:add"
    //         },
    //         "requirements": {
    //           "predicate": {
    //             "type": "#minecraft:sensitive_to_smite"
    //           },
    //           "entity": "this",
    //           "condition": "minecraft:entity_properties"
    //         }
    //       }
    //     ]
    //   }
    // },
    // "soul_speed": {
    //   "description": {
    //     "translate": "enchantment.minecraft.soul_speed"
    //   },
    //   "min_cost": {
    //     "base": 10,
    //     "per_level_above_first": 10
    //   },
    //   "max_cost": {
    //     "base": 25,
    //     "per_level_above_first": 10
    //   },
    //   "anvil_cost": 8,
    //   "slots": [
    //     "feet"
    //   ],
    //   "supported_items": "#minecraft:enchantable/foot_armor",
    //   "weight": 1,
    //   "max_level": 3,
    //   "effects": {
    //     "minecraft:location_changed": [
    //       {
    //         "effect": {
    //           "effects": [
    //             {
    //               "id": "minecraft:enchantment.soul_speed",
    //               "attribute": "minecraft:movement_speed",
    //               "amount": {
    //                 "base": 0.0405,
    //                 "per_level_above_first": 0.0105,
    //                 "type": "minecraft:linear"
    //               },
    //               "operation": "add_value",
    //               "type": "minecraft:attribute"
    //             },
    //             {
    //               "id": "minecraft:enchantment.soul_speed",
    //               "attribute": "minecraft:movement_efficiency",
    //               "amount": 1.0,
    //               "operation": "add_value",
    //               "type": "minecraft:attribute"
    //             }
    //           ],
    //           "type": "minecraft:all_of"
    //         },
    //         "requirements": {
    //           "terms": [
    //             {
    //               "term": {
    //                 "predicate": {
    //                   "vehicle": {}
    //                 },
    //                 "entity": "this",
    //                 "condition": "minecraft:entity_properties"
    //               },
    //               "condition": "minecraft:inverted"
    //             },
    //             {
    //               "terms": [
    //                 {
    //                   "terms": [
    //                     {
    //                       "active": true,
    //                       "condition": "minecraft:enchantment_active_check"
    //                     },
    //                     {
    //                       "predicate": {
    //                         "flags": {
    //                           "is_flying": false
    //                         }
    //                       },
    //                       "entity": "this",
    //                       "condition": "minecraft:entity_properties"
    //                     },
    //                     {
    //                       "terms": [
    //                         {
    //                           "predicate": {
    //                             "movement_affected_by": {
    //                               "block": {
    //                                 "blocks": "#minecraft:soul_speed_blocks"
    //                               }
    //                             }
    //                           },
    //                           "entity": "this",
    //                           "condition": "minecraft:entity_properties"
    //                         },
    //                         {
    //                           "predicate": {
    //                             "flags": {
    //                               "is_on_ground": false
    //                             }
    //                           },
    //                           "entity": "this",
    //                           "condition": "minecraft:entity_properties"
    //                         }
    //                       ],
    //                       "condition": "minecraft:any_of"
    //                     }
    //                   ],
    //                   "condition": "minecraft:all_of"
    //                 },
    //                 {
    //                   "terms": [
    //                     {
    //                       "active": false,
    //                       "condition": "minecraft:enchantment_active_check"
    //                     },
    //                     {
    //                       "predicate": {
    //                         "flags": {
    //                           "is_flying": false
    //                         },
    //                         "movement_affected_by": {
    //                           "block": {
    //                             "blocks": "#minecraft:soul_speed_blocks"
    //                           }
    //                         }
    //                       },
    //                       "entity": "this",
    //                       "condition": "minecraft:entity_properties"
    //                     }
    //                   ],
    //                   "condition": "minecraft:all_of"
    //                 }
    //               ],
    //               "condition": "minecraft:any_of"
    //             }
    //           ],
    //           "condition": "minecraft:all_of"
    //         }
    //       },
    //       {
    //         "effect": {
    //           "amount": 1.0,
    //           "type": "minecraft:change_item_damage"
    //         },
    //         "requirements": {
    //           "terms": [
    //             {
    //               "chance": {
    //                 "amount": 0.04,
    //                 "type": "minecraft:enchantment_level"
    //               },
    //               "condition": "minecraft:random_chance"
    //             },
    //             {
    //               "predicate": {
    //                 "flags": {
    //                   "is_on_ground": true
    //                 },
    //                 "movement_affected_by": {
    //                   "block": {
    //                     "blocks": "#minecraft:soul_speed_blocks"
    //                   }
    //                 }
    //               },
    //               "entity": "this",
    //               "condition": "minecraft:entity_properties"
    //             }
    //           ],
    //           "condition": "minecraft:all_of"
    //         }
    //       }
    //     ],
    //     "minecraft:tick": [
    //       {
    //         "effect": {
    //           "horizontal_velocity": {
    //             "movement_scale": -0.2
    //           },
    //           "vertical_velocity": {
    //             "base": 0.1
    //           },
    //           "speed": 1.0,
    //           "particle": {
    //             "type": "minecraft:soul"
    //           },
    //           "horizontal_position": {
    //             "type": "in_bounding_box"
    //           },
    //           "vertical_position": {
    //             "type": "entity_position",
    //             "offset": 0.1
    //           },
    //           "type": "minecraft:spawn_particles"
    //         },
    //         "requirements": {
    //           "predicate": {
    //             "periodic_tick": 5,
    //             "flags": {
    //               "is_flying": false,
    //               "is_on_ground": true
    //             },
    //             "movement": {
    //               "horizontal_speed": {
    //                 "min": 9.999999747378752E-6
    //               }
    //             },
    //             "movement_affected_by": {
    //               "block": {
    //                 "blocks": "#minecraft:soul_speed_blocks"
    //               }
    //             }
    //           },
    //           "entity": "this",
    //           "condition": "minecraft:entity_properties"
    //         }
    //       },
    //       {
    //         "effect": {
    //           "sound": "minecraft:particle.soul_escape",
    //           "volume": 0.6,
    //           "pitch": {
    //             "min_inclusive": 0.6,
    //             "max_exclusive": 1.0,
    //             "type": "minecraft:uniform"
    //           },
    //           "type": "minecraft:play_sound"
    //         },
    //         "requirements": {
    //           "terms": [
    //             {
    //               "chance": 0.35,
    //               "condition": "minecraft:random_chance"
    //             },
    //             {
    //               "predicate": {
    //                 "periodic_tick": 5,
    //                 "flags": {
    //                   "is_flying": false,
    //                   "is_on_ground": true
    //                 },
    //                 "movement": {
    //                   "horizontal_speed": {
    //                     "min": 9.999999747378752E-6
    //                   }
    //                 },
    //                 "movement_affected_by": {
    //                   "block": {
    //                     "blocks": "#minecraft:soul_speed_blocks"
    //                   }
    //                 }
    //               },
    //               "entity": "this",
    //               "condition": "minecraft:entity_properties"
    //             }
    //           ],
    //           "condition": "minecraft:all_of"
    //         }
    //       }
    //     ]
    //   }
    // },
    // "sweeping_edge": {
    //   "description": {
    //     "translate": "enchantment.minecraft.sweeping_edge"
    //   },
    //   "min_cost": {
    //     "base": 5,
    //     "per_level_above_first": 9
    //   },
    //   "max_cost": {
    //     "base": 20,
    //     "per_level_above_first": 9
    //   },
    //   "anvil_cost": 4,
    //   "slots": [
    //     "mainhand"
    //   ],
    //   "supported_items": "#minecraft:enchantable/sword",
    //   "weight": 2,
    //   "max_level": 3,
    //   "effects": {
    //     "minecraft:attributes": [
    //       {
    //         "id": "minecraft:enchantment.sweeping_edge",
    //         "attribute": "minecraft:sweeping_damage_ratio",
    //         "amount": {
    //           "numerator": {
    //             "base": 1.0,
    //             "per_level_above_first": 1.0,
    //             "type": "minecraft:linear"
    //           },
    //           "denominator": {
    //             "base": 2.0,
    //             "per_level_above_first": 1.0,
    //             "type": "minecraft:linear"
    //           },
    //           "type": "minecraft:fraction"
    //         },
    //         "operation": "add_value"
    //       }
    //     ]
    //   }
    // },
    // "swift_sneak": {
    //   "description": {
    //     "translate": "enchantment.minecraft.swift_sneak"
    //   },
    //   "min_cost": {
    //     "base": 25,
    //     "per_level_above_first": 25
    //   },
    //   "max_cost": {
    //     "base": 75,
    //     "per_level_above_first": 25
    //   },
    //   "anvil_cost": 8,
    //   "slots": [
    //     "legs"
    //   ],
    //   "supported_items": "#minecraft:enchantable/leg_armor",
    //   "weight": 1,
    //   "max_level": 3,
    //   "effects": {
    //     "minecraft:attributes": [
    //       {
    //         "id": "minecraft:enchantment.swift_sneak",
    //         "attribute": "minecraft:sneaking_speed",
    //         "amount": {
    //           "base": 0.15,
    //           "per_level_above_first": 0.15,
    //           "type": "minecraft:linear"
    //         },
    //         "operation": "add_value"
    //       }
    //     ]
    //   }
    // },
    // "thorns": {
    //   "description": {
    //     "translate": "enchantment.minecraft.thorns"
    //   },
    //   "min_cost": {
    //     "base": 10,
    //     "per_level_above_first": 20
    //   },
    //   "max_cost": {
    //     "base": 60,
    //     "per_level_above_first": 20
    //   },
    //   "anvil_cost": 8,
    //   "slots": [
    //     "any"
    //   ],
    //   "supported_items": "#minecraft:enchantable/armor",
    //   "primary_items": "#minecraft:enchantable/chest_armor",
    //   "weight": 1,
    //   "max_level": 3,
    //   "effects": {
    //     "minecraft:post_attack": [
    //       {
    //         "enchanted": "victim",
    //         "affected": "attacker",
    //         "effect": {
    //           "effects": [
    //             {
    //               "min_damage": 1.0,
    //               "max_damage": 5.0,
    //               "damage_type": "minecraft:thorns",
    //               "type": "minecraft:damage_entity"
    //             },
    //             {
    //               "amount": 2.0,
    //               "type": "minecraft:change_item_damage"
    //             }
    //           ],
    //           "type": "minecraft:all_of"
    //         },
    //         "requirements": {
    //           "chance": {
    //             "amount": {
    //               "base": 0.15,
    //               "per_level_above_first": 0.15,
    //               "type": "minecraft:linear"
    //             },
    //             "type": "minecraft:enchantment_level"
    //           },
    //           "condition": "minecraft:random_chance"
    //         }
    //       }
    //     ]
    //   }
    // },
    // "unbreaking": {
    //   "description": {
    //     "translate": "enchantment.minecraft.unbreaking"
    //   },
    //   "min_cost": {
    //     "base": 5,
    //     "per_level_above_first": 8
    //   },
    //   "max_cost": {
    //     "base": 55,
    //     "per_level_above_first": 8
    //   },
    //   "anvil_cost": 2,
    //   "slots": [
    //     "any"
    //   ],
    //   "supported_items": "#minecraft:enchantable/durability",
    //   "weight": 5,
    //   "max_level": 3,
    //   "effects": {
    //     "minecraft:item_damage": [
    //       {
    //         "effect": {
    //           "chance": {
    //             "numerator": {
    //               "base": 2.0,
    //               "per_level_above_first": 2.0,
    //               "type": "minecraft:linear"
    //             },
    //             "denominator": {
    //               "base": 10.0,
    //               "per_level_above_first": 5.0,
    //               "type": "minecraft:linear"
    //             },
    //             "type": "minecraft:fraction"
    //           },
    //           "type": "minecraft:remove_binomial"
    //         },
    //         "requirements": {
    //           "predicate": {
    //             "items": "#minecraft:enchantable/armor"
    //           },
    //           "condition": "minecraft:match_tool"
    //         }
    //       },
    //       {
    //         "effect": {
    //           "chance": {
    //             "numerator": {
    //               "base": 1.0,
    //               "per_level_above_first": 1.0,
    //               "type": "minecraft:linear"
    //             },
    //             "denominator": {
    //               "base": 2.0,
    //               "per_level_above_first": 1.0,
    //               "type": "minecraft:linear"
    //             },
    //             "type": "minecraft:fraction"
    //           },
    //           "type": "minecraft:remove_binomial"
    //         },
    //         "requirements": {
    //           "term": {
    //             "predicate": {
    //               "items": "#minecraft:enchantable/armor"
    //             },
    //             "condition": "minecraft:match_tool"
    //           },
    //           "condition": "minecraft:inverted"
    //         }
    //       }
    //     ]
    //   }
    // },
    // "wind_burst": {
    //   "description": {
    //     "translate": "enchantment.minecraft.wind_burst"
    //   },
    //   "min_cost": {
    //     "base": 15,
    //     "per_level_above_first": 9
    //   },
    //   "max_cost": {
    //     "base": 65,
    //     "per_level_above_first": 9
    //   },
    //   "anvil_cost": 4,
    //   "slots": [
    //     "mainhand"
    //   ],
    //   "supported_items": "#minecraft:enchantable/mace",
    //   "weight": 2,
    //   "max_level": 3,
    //   "effects": {
    //     "minecraft:post_attack": [
    //       {
    //         "enchanted": "attacker",
    //         "affected": "attacker",
    //         "effect": {
    //           "small_particle": {
    //             "type": "minecraft:gust_emitter_small"
    //           },
    //           "large_particle": {
    //             "type": "minecraft:gust_emitter_large"
    //           },
    //           "sound": "minecraft:entity.wind_charge.wind_burst",
    //           "radius": 3.5,
    //           "block_interaction": "trigger",
    //           "knockback_multiplier": {
    //             "values": [
    //               1.2,
    //               1.75,
    //               2.2
    //             ],
    //             "fallback": {
    //               "base": 1.5,
    //               "per_level_above_first": 0.35,
    //               "type": "minecraft:linear"
    //             },
    //             "type": "minecraft:lookup"
    //           },
    //           "immune_blocks": "#minecraft:blocks_wind_charge_explosions",
    //           "type": "minecraft:explode"
    //         },
    //         "requirements": {
    //           "predicate": {
    //             "flags": {
    //               "is_flying": false
    //             },
    //             "movement": {
    //               "fall_distance": {
    //                 "min": 1.5
    //               }
    //             }
    //           },
    //           "entity": "direct_attacker",
    //           "condition": "minecraft:entity_properties"
    //         }
    //       }
    //     ]
    //   }
    // }

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
    { "minecraft:clear_effects", [](const ConsumeEffectData&) { return ByteBuffer().getBytes(); } }, 
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
    { "minecraft:empty", [](const SlotDisplayData&) { return ByteBuffer().getBytes(); } },
    { "minecraft:any_fuel", [](const SlotDisplayData&) { return ByteBuffer().getBytes(); } },
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
    { "minecraft:empty", [](ByteBuffer&) { return SlotDisplayData(); } },
    { "minecraft:any_fuel", [](ByteBuffer&) { return SlotDisplayData(); } },
    { "minecraft:item", [](ByteBuffer& buffer) {
        SlotDisplayData data;
        data.m_itemType = buffer.readVarNumeric<int>();
        return data;
    } }, 
    { "minecraft:item_stack", [](ByteBuffer&) { return SlotDisplayData(); } }, 
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