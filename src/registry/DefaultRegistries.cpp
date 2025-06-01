#include "type/Identifier.h"
#include "type/nbt/NBTElement.h"
#include "util/Memory.h"
#include <registry/DefaultRegistries.h>
#include <network/minecraft/channels/BrandChannel.h>
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

std::unordered_map<std::string, NBTElement> g_trimMaterial = {
    { "minecraft:amethyst", NBTElement::Compound({
        NBTElement::String("asset_name", "minecraft:amethyst"),
        NBTElement::Compound("description", {
            NBTElement::String("type", "translatable"),
            NBTElement::String("translate", "trim_material.minecraft.amethyst"), 
            NBTElement::String("color", "#9A5CC6")
        })
    }) },
    { "minecraft:copper", NBTElement::Compound({
        NBTElement::String("asset_name", "minecraft:copper"),
        NBTElement::Compound("description", {
            NBTElement::String("type", "translatable"),
            NBTElement::String("translate", "trim_material.minecraft.copper"),
            NBTElement::String("color", "#B4684D")
        })
    }) },
    { "minecraft:diamond", NBTElement::Compound({
        NBTElement::String("asset_name", "minecraft:diamond"),
        NBTElement::Compound("override_armor_assets", {
            NBTElement::String("minecraft:diamond", "diamond_darker")
        }),
        NBTElement::Compound("description", {
            NBTElement::String("type", "translatable"),
            NBTElement::String("translate", "trim_material.minecraft.diamond"),
            NBTElement::String("color", "#6EECD2")
        })
    }) },
    { "minecraft:iron", NBTElement::Compound({
        NBTElement::String("asset_name", "minecraft:iron"),
        NBTElement::Compound("override_armor_assets", {
            NBTElement::String("minecraft:iron", "iron_darker")
        }),
        NBTElement::Compound("description", {
            NBTElement::String("type", "translatable"),
            NBTElement::String("translate", "trim_material.minecraft.iron"),
            NBTElement::String("color", "#ECECEC")
        })
    }) },
    { "minecraft:gold", NBTElement::Compound({
        NBTElement::String("asset_name", "minecraft:gold"),
        NBTElement::Compound("override_armor_assets", {
            NBTElement::String("minecraft:gold", "gold_darker")
        }),
        NBTElement::Compound("description", {
            NBTElement::String("type", "translatable"),
            NBTElement::String("translate", "trim_material.minecraft.gold"),
            NBTElement::String("color", "#DEB12D")
        })
    }) },
    { "minecraft:netherite", NBTElement::Compound({
        NBTElement::String("asset_name", "minecraft:netherite"),
        NBTElement::Compound("override_armor_assets", {
            NBTElement::String("minecraft:netherite", "netherite_darker")
        }),
        NBTElement::Compound("description", {
            NBTElement::String("type", "translatable"),
            NBTElement::String("translate", "trim_material.minecraft.netherite"),
            NBTElement::String("color", "#625859")
        })
    }) },
    { "minecraft:lapis", NBTElement::Compound({
        NBTElement::String("asset_name", "minecraft:lapis"),
        NBTElement::Compound("description", {
            NBTElement::String("type", "translatable"),
            NBTElement::String("translate", "trim_material.minecraft.lapis"),
            NBTElement::String("color", "#416E97")
        })
    }) },
    { "minecraft:quartz", NBTElement::Compound({
        NBTElement::String("asset_name", "minecraft:quartz"),
        NBTElement::Compound("description", {
            NBTElement::String("type", "translatable"),
            NBTElement::String("translate", "trim_material.minecraft.quartz"),
            NBTElement::String("color", "#E3D4C4")
        })
    }) },
    { "minecraft:redstone", NBTElement::Compound({
        NBTElement::String("asset_name", "minecraft:redstone"),
        NBTElement::Compound("description", {
            NBTElement::String("type", "translatable"),
            NBTElement::String("translate", "trim_material.minecraft.redstone"),
            NBTElement::String("color", "#971607")
        })
    }) },
    { "minecraft:resin", NBTElement::Compound({
        NBTElement::String("asset_name", "minecraft:resin"),
        NBTElement::Compound("description", {
            NBTElement::String("type", "translatable"),
            NBTElement::String("translate", "trim_material.minecraft.resin"),
            NBTElement::String("color", "#FC7812")
        })
    }) },
    { "minecraft:emerald", NBTElement::Compound({
        NBTElement::String("asset_name", "minecraft:emerald"),
        NBTElement::Compound("description", {
            NBTElement::String("type", "translatable"),
            NBTElement::String("translate", "trim_material.minecraft.emerald"),
            NBTElement::String("color", "#11A036")
        })
    }) }
};
std::unordered_map<std::string, NBTElement> g_trimPattern = {
    { "minecraft:bolt", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:bolt"),
        NBTElement::Compound("description", { 
            NBTElement::String("type", "translatable"), NBTElement::String("translate", "trim_pattern.minecraft.bolt") 
        }),
        NBTElement::Byte("decal", false)
    }) },
    { "minecraft:coast", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:coast"),
        NBTElement::Compound("description", { 
            NBTElement::String("type", "translatable"), NBTElement::String("translate", "trim_pattern.minecraft.coast") 
        }),
        NBTElement::Byte("decal", false)
    }) },
    { "minecraft:dune", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:dune"),
        NBTElement::Compound("description", { 
            NBTElement::String("type", "translatable"), NBTElement::String("translate", "trim_pattern.minecraft.dune") 
        }),
        NBTElement::Byte("decal", false)
    }) },
    { "minecraft:eye", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:eye"),
        NBTElement::Compound("description", { 
            NBTElement::String("type", "translatable"), NBTElement::String("translate", "trim_pattern.minecraft.eye") 
        }),
        NBTElement::Byte("decal", false)
    }) },
    { "minecraft:flow", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:flow"),
        NBTElement::Compound("description", { 
            NBTElement::String("type", "translatable"), NBTElement::String("translate", "trim_pattern.minecraft.flow") 
        }),
        NBTElement::Byte("decal", false)
    }) },
    { "minecraft:host", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:host"),
        NBTElement::Compound("description", { 
            NBTElement::String("type", "translatable"), NBTElement::String("translate", "trim_pattern.minecraft.host") 
        }),
        NBTElement::Byte("decal", false)
    }) },
    { "minecraft:raiser", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:raiser"),
        NBTElement::Compound("description", { 
            NBTElement::String("type", "translatable"), NBTElement::String("translate", "trim_pattern.minecraft.raiser") 
        }),
        NBTElement::Byte("decal", false)
    }) },
    { "minecraft:rib", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:rib"),
        NBTElement::Compound("description", { 
            NBTElement::String("type", "translatable"), NBTElement::String("translate", "trim_pattern.minecraft.rib") 
        }),
        NBTElement::Byte("decal", false)
    }) },
    { "minecraft:sentry", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:sentry"),
        NBTElement::Compound("description", { 
            NBTElement::String("type", "translatable"), NBTElement::String("translate", "trim_pattern.minecraft.sentry") 
        }),
        NBTElement::Byte("decal", false)
    }) },
    { "minecraft:shaper", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:shaper"),
        NBTElement::Compound("description", { 
            NBTElement::String("type", "translatable"), NBTElement::String("translate", "trim_pattern.minecraft.shaper") 
        }),
        NBTElement::Byte("decal", false)
    }) },
    { "minecraft:silence", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:silence"),
        NBTElement::Compound("description", { 
            NBTElement::String("type", "translatable"), NBTElement::String("translate", "trim_pattern.minecraft.silence") 
        }),
        NBTElement::Byte("decal", false)
    }) },
    { "minecraft:snout", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:snout"),
        NBTElement::Compound("description", { 
            NBTElement::String("type", "translatable"), NBTElement::String("translate", "trim_pattern.minecraft.snout") 
        }),
        NBTElement::Byte("decal", false)
    }) },
    { "minecraft:spire", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:spire"),
        NBTElement::Compound("description", { 
            NBTElement::String("type", "translatable"), NBTElement::String("translate", "trim_pattern.minecraft.spire") 
        }),
        NBTElement::Byte("decal", false)
    }) },
    { "minecraft:tide", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:tide"),
        NBTElement::Compound("description", { 
            NBTElement::String("type", "translatable"), NBTElement::String("translate", "trim_pattern.minecraft.tide") 
        }),
        NBTElement::Byte("decal", false)
    }) },
    { "minecraft:vex", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:vex"),
        NBTElement::Compound("description", { 
            NBTElement::String("type", "translatable"), NBTElement::String("translate", "trim_pattern.minecraft.vex") 
        }),
        NBTElement::Byte("decal", false)
    }) },
    { "minecraft:ward", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:ward"),
        NBTElement::Compound("description", { 
            NBTElement::String("type", "translatable"), NBTElement::String("translate", "trim_pattern.minecraft.ward") 
        }),
        NBTElement::Byte("decal", false)
    }) },
    { "minecraft:wayfinder", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:wayfinder"),
        NBTElement::Compound("description", { 
            NBTElement::String("type", "translatable"), NBTElement::String("translate", "trim_pattern.minecraft.wayfinder") 
        }),
        NBTElement::Byte("decal", false)
    }) },
    { "minecraft:wild", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:wild"),
        NBTElement::Compound("description", { 
            NBTElement::String("type", "translatable"), NBTElement::String("translate", "trim_pattern.minecraft.wild") 
        }),
        NBTElement::Byte("decal", false)
    }) }
};
std::unordered_map<std::string, NBTElement> g_bannerPattern = {
    { "minecraft:base", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:base"),
        NBTElement::String("translation_key", "block.minecraft.banner.base")
    }) },
    { "minecraft:border", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:border"),
        NBTElement::String("translation_key", "block.minecraft.banner.border")
    }) },
    { "minecraft:bricks", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:bricks"),
        NBTElement::String("translation_key", "block.minecraft.banner.bricks")
    }) },
    { "minecraft:circle", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:circle"),
        NBTElement::String("translation_key", "block.minecraft.banner.circle")
    }) },
    { "minecraft:creeper", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:creeper"),
        NBTElement::String("translation_key", "block.minecraft.banner.creeper")
    }) },
    { "minecraft:cross", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:cross"),
        NBTElement::String("translation_key", "block.minecraft.banner.cross")
    }) },
    { "minecraft:curly_border", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:curly_border"),
        NBTElement::String("translation_key", "block.minecraft.banner.curly_border")
    }) },
    { "minecraft:diagonal_left", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:diagonal_left"),
        NBTElement::String("translation_key", "block.minecraft.banner.diagonal_left")
    }) },
    { "minecraft:diagonal_right", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:diagonal_right"),
        NBTElement::String("translation_key", "block.minecraft.banner.diagonal_right")
    }) },
    { "minecraft:diagonal_left_up", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:diagonal_left_up"),
        NBTElement::String("translation_key", "block.minecraft.banner.diagonal_left_up")
    }) },
    { "minecraft:diagonal_right_up", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:diagonal_right_up"),
        NBTElement::String("translation_key", "block.minecraft.banner.diagonal_right_up")
    }) },
    { "minecraft:flow", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:flow"),
        NBTElement::String("translation_key", "block.minecraft.banner.flow")
    }) },
    { "minecraft:flower", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:flower"),
        NBTElement::String("translation_key", "block.minecraft.banner.flower")
    }) },
    { "minecraft:globe", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:globe"),
        NBTElement::String("translation_key", "block.minecraft.banner.globe")
    }) },
    { "minecraft:gradient", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:gradient"),
        NBTElement::String("translation_key", "block.minecraft.banner.gradient")
    }) },
    { "minecraft:gradient_up", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:gradient_up"),
        NBTElement::String("translation_key", "block.minecraft.banner.gradient_up")
    }) },
    { "minecraft:guster", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:guster"),
        NBTElement::String("translation_key", "block.minecraft.banner.guster")
    }) },
    { "minecraft:half_horizontal", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:half_horizontal"),
        NBTElement::String("translation_key", "block.minecraft.banner.half_horizontal")
    }) },
    { "minecraft:half_horizontal_bottom", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:half_horizontal_bottom"),
        NBTElement::String("translation_key", "block.minecraft.banner.half_horizontal_bottom")
    }) },
    { "minecraft:half_vertical", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:half_vertical"),
        NBTElement::String("translation_key", "block.minecraft.banner.half_vertical")
    }) },
    { "minecraft:half_vertical_right", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:half_vertical_right"),
        NBTElement::String("translation_key", "block.minecraft.banner.half_vertical_right")
    }) },
    { "minecraft:mojang", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:mojang"),
        NBTElement::String("translation_key", "block.minecraft.banner.mojang")
    }) },
    { "minecraft:piglin", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:piglin"),
        NBTElement::String("translation_key", "block.minecraft.banner.piglin")
    }) },
    { "minecraft:rhombus", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:rhombus"),
        NBTElement::String("translation_key", "block.minecraft.banner.rhombus")
    }) },
    { "minecraft:skull", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:skull"),
        NBTElement::String("translation_key", "block.minecraft.banner.skull")
    }) },
    { "minecraft:small_stripes", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:small_stripes"),
        NBTElement::String("translation_key", "block.minecraft.banner.small_stripes")
    }) },
    { "minecraft:square_bottom_left", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:square_bottom_left"),
        NBTElement::String("translation_key", "block.minecraft.banner.square_bottom_left")
    }) },
    { "minecraft:square_bottom_right", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:square_bottom_right"),
        NBTElement::String("translation_key", "block.minecraft.banner.square_bottom_right")
    }) },
    { "minecraft:square_top_left", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:square_top_left"),
        NBTElement::String("translation_key", "block.minecraft.banner.square_top_left")
    }) },
    { "minecraft:square_top_right", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:square_top_right"),
        NBTElement::String("translation_key", "block.minecraft.banner.square_top_right")
    }) },
    { "minecraft:straight_cross", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:straight_cross"),
        NBTElement::String("translation_key", "block.minecraft.banner.straight_cross")
    }) },
    { "minecraft:stripe_bottom", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:stripe_bottom"),
        NBTElement::String("translation_key", "block.minecraft.banner.stripe_bottom")
    }) },
    { "minecraft:stripe_center", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:stripe_center"),
        NBTElement::String("translation_key", "block.minecraft.banner.stripe_center")
    }) },
    { "minecraft:stripe_downleft", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:stripe_downleft"),
        NBTElement::String("translation_key", "block.minecraft.banner.stripe_downleft")
    }) },
    { "minecraft:stripe_downright", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:stripe_downright"),
        NBTElement::String("translation_key", "block.minecraft.banner.stripe_downright")
    }) },
    { "minecraft:stripe_left", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:stripe_left"),
        NBTElement::String("translation_key", "block.minecraft.banner.stripe_left")
    }) },
    { "minecraft:stripe_middle", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:stripe_middle"),
        NBTElement::String("translation_key", "block.minecraft.banner.stripe_middle")
    }) },
    { "minecraft:stripe_right", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:stripe_right"),
        NBTElement::String("translation_key", "block.minecraft.banner.stripe_right")
    }) },
    { "minecraft:stripe_top", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:stripe_top"),
        NBTElement::String("translation_key", "block.minecraft.banner.stripe_top")
    }) },
    { "minecraft:triangle_bottom", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:triangle_bottom"),
        NBTElement::String("translation_key", "block.minecraft.banner.triangle_bottom")
    }) },
    { "minecraft:triangle_top", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:triangle_top"),
        NBTElement::String("translation_key", "block.minecraft.banner.triangle_top")
    }) },
    { "minecraft:triangles_bottom", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:triangles_bottom"),
        NBTElement::String("translation_key", "block.minecraft.banner.triangles_bottom")
    }) },
    { "minecraft:triangles_top", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:triangles_top"),
        NBTElement::String("translation_key", "block.minecraft.banner.triangles_top")
    }) }
};
std::unordered_map<std::string, NBTElement> g_chatType = {
    { "minecraft:chat", NBTElement::Compound({
        NBTElement::Compound("chat", {
            NBTElement::String("translation_key", "chat.type.text"),
            NBTElement::List("parameters", { NBTElement::String("sender"), NBTElement::String("content") })
        }),
        NBTElement::Compound("narration", {
            NBTElement::String("translation_key", "chat.type.text.narrate"),
            NBTElement::List("parameters", { NBTElement::String("sender"), NBTElement::String("content") })
        })
    }) },
    { "minecraft:emote_command", NBTElement::Compound({
        NBTElement::Compound("chat", {
            NBTElement::String("translation_key", "chat.type.emote"),
            NBTElement::List("parameters", { NBTElement::String("sender"), NBTElement::String("content") })
        }),
        NBTElement::Compound("narration", {
            NBTElement::String("translation_key", "chat.type.emote"),
            NBTElement::List("parameters", { NBTElement::String("sender"), NBTElement::String("content") })
        })
    }) },
    { "minecraft:msg_command_incoming", NBTElement::Compound({
        NBTElement::Compound("chat", {
            NBTElement::String("translation_key", "commands.message.display.incoming"),
            NBTElement::List("parameters", { NBTElement::String("sender"), NBTElement::String("content") }),
            NBTElement::Compound("style", {
                NBTElement::Byte("italic", false),
                NBTElement::String("color", "gray")
            })
        }),
        NBTElement::Compound("narration", {
            NBTElement::String("translation_key", "chat.type.text.narrate"),
            NBTElement::List("parameters", { NBTElement::String("sender"), NBTElement::String("content") })
        })
    }) },
    { "minecraft:msg_command_outgoing", NBTElement::Compound({
        NBTElement::Compound("chat", {
            NBTElement::String("translation_key", "commands.message.display.outgoing"),
            NBTElement::List("parameters", { NBTElement::String("sender"), NBTElement::String("content") }),
            NBTElement::Compound("style", {
                NBTElement::Byte("italic", false),
                NBTElement::String("color", "gray")
            })
        }),
        NBTElement::Compound("narration", {
            NBTElement::String("translation_key", "chat.type.text.narrate"),
            NBTElement::List("parameters", { NBTElement::String("sender"), NBTElement::String("content") })
        })
    }) },
    { "minecraft:say_command", NBTElement::Compound({
        NBTElement::Compound("chat", {
            NBTElement::String("translation_key", "chat.type.announcement"),
            NBTElement::List("parameters", { NBTElement::String("sender"), NBTElement::String("content") })
        }),
        NBTElement::Compound("narration", {
            NBTElement::String("translation_key", "chat.type.text.narrate"),
            NBTElement::List("parameters", { NBTElement::String("sender"), NBTElement::String("content") })
        })
    }) },
    { "minecraft:team_msg_command_incoming", NBTElement::Compound({
        NBTElement::Compound("chat", {
            NBTElement::String("translation_key", "chat.type.team.text"),
            NBTElement::List("parameters", { NBTElement::String("target"), NBTElement::String("sender"), NBTElement::String("content") })
        }),
        NBTElement::Compound("narration", {
            NBTElement::String("translation_key", "chat.type.text.narrate"),
            NBTElement::List("parameters", { NBTElement::String("sender"), NBTElement::String("content") })
        })
    }) },
    { "minecraft:team_msg_command_outgoing", NBTElement::Compound({
        NBTElement::Compound("chat", {
            NBTElement::String("translation_key", "chat.type.team.sent"),
            NBTElement::List("parameters", { NBTElement::String("target"), NBTElement::String("sender"), NBTElement::String("content") })
        }),
        NBTElement::Compound("narration", {
            NBTElement::String("translation_key", "chat.type.text.narrate"),
            NBTElement::List("parameters", { NBTElement::String("sender"), NBTElement::String("content") })
        })
    }) }
};
std::unordered_map<std::string, NBTElement> g_biome = {
    { "minecraft:badlands", NBTElement::Compound({
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
    }) },
    { "minecraft:bamboo_jungle", NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", 0.95f),
        NBTElement::Float("downfall", 0.9f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.overworld.bamboo_jungle"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Int("sky_color", 7842047),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:basalt_deltas", NBTElement::Compound({
        NBTElement::Byte("has_precipitation", false),
        NBTElement::Float("temperature", 2.0f),
        NBTElement::Float("downfall", 0.0f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.basalt_deltas.mood"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.nether.basalt_deltas"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Compound("additions_sound", {
                NBTElement::String("sound", "minecraft:ambient.basalt_deltas.additions"),
                NBTElement::Double("tick_chance", 0.0111)
            }),
            NBTElement::Compound("particle", {
                NBTElement::Compound("options", {
                    NBTElement::String("type", "minecraft:white_ash")
                }),
                NBTElement::Float("probability", 0.118093334f)
            }),
            NBTElement::String("ambient_sound", "minecraft:ambient.basalt_deltas.loop"),
            NBTElement::Int("sky_color", 7254527),
            NBTElement::Int("fog_color", 6840176),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:beach", NBTElement::Compound({
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
    }) },
    { "minecraft:birch_forest", NBTElement::Compound({
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
    }) },
    { "minecraft:cherry_grove", NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", 0.5f),
        NBTElement::Float("downfall", 0.8f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.overworld.cherry_grove"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Int("sky_color", 8103167),
            NBTElement::Int("foliage_color", 11983713),
            NBTElement::Int("grass_color", 11983713),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 6141935),
            NBTElement::Int("water_fog_color", 6141935)
        })
    }) },
    { "minecraft:cold_ocean", NBTElement::Compound({
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
            NBTElement::Int("sky_color", 8103167),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4020182),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:crimson_forest", NBTElement::Compound({
        NBTElement::Byte("has_precipitation", false),
        NBTElement::Float("temperature", 2.0f),
        NBTElement::Float("downfall", 0.0f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.crimson_forest.mood"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.nether.crimson_forest"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Compound("additions_sound", {
                NBTElement::String("sound", "minecraft:ambient.crimson_forest.additions"),
                NBTElement::Double("tick_chance", 0.0111)
            }),
            NBTElement::Compound("particle", {
                NBTElement::Compound("options", {
                    NBTElement::String("type", "minecraft:crimson_spore")
                }),
                NBTElement::Float("probability", 0.025f)
            }),
            NBTElement::String("ambient_sound", "minecraft:ambient.crimson_forest.loop"),
            NBTElement::Int("sky_color", 7254527),
            NBTElement::Int("fog_color", 3343107),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:dark_forest", NBTElement::Compound({
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
                NBTElement::String("sound", "minecraft:music.overworld.dark_forest"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::String("grass_color_modifier", "dark_forest"),
            NBTElement::Int("sky_color", 7972607),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("dry_foliage_color", 8082228),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:deep_cold_ocean", NBTElement::Compound({
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
            NBTElement::Int("sky_color", 8103167),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4020182),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:deep_dark", NBTElement::Compound({
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
                NBTElement::String("sound", "minecraft:music.overworld.deep_dark"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Int("sky_color", 7907327),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:deep_frozen_ocean", NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", 0.5f),
        NBTElement::String("temperature_modifier", "frozen"),
        NBTElement::Float("downfall", 0.5f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Int("sky_color", 8103167),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 3750089),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:deep_lukewarm_ocean", NBTElement::Compound({
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
            NBTElement::Int("sky_color", 8103167),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4566514),
            NBTElement::Int("water_fog_color", 267827)
        })
    }) },
    { "minecraft:deep_ocean", NBTElement::Compound({
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
            NBTElement::Int("sky_color", 8103167),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:desert", NBTElement::Compound({
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
                NBTElement::String("sound", "minecraft:music.overworld.desert"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Int("sky_color", 7254527),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:dripstone_caves", NBTElement::Compound({
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
                NBTElement::String("sound", "minecraft:music.overworld.dripstone_caves"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Int("sky_color", 7907327),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:end_barrens", NBTElement::Compound({
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
    }) },
    { "minecraft:end_highlands", NBTElement::Compound({
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
    }) },
    { "minecraft:end_midlands", NBTElement::Compound({
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
    }) },
    { "minecraft:eroded_badlands", NBTElement::Compound({
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
                NBTElement::String("sound", "minecraft:music.overworld.eroded_badlands"),
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
    }) },
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
    { "minecraft:forest", NBTElement::Compound({
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
                NBTElement::String("sound", "minecraft:music.overworld.forest"),
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
    { "minecraft:frozen_ocean", NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", 0.5f),
        NBTElement::Float("downfall", 0.5f),
        NBTElement::String("temperature_modifier", "frozen"),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Int("sky_color", 8364543),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 3750089),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:frozen_peaks", NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", -0.7f),
        NBTElement::Float("downfall", 0.9f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.overworld.frozen_peaks"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Int("sky_color", 8756735),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:frozen_river", NBTElement::Compound({
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
            NBTElement::Int("water_color", 3750089),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:grove", NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", -0.2f),
        NBTElement::Float("downfall", 0.8f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.overworld.grove"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Int("sky_color", 8495359),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:ice_spikes", NBTElement::Compound({
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
    }) },
    { "minecraft:jagged_peaks", NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", -0.7f),
        NBTElement::Float("downfall", 0.9f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.overworld.jagged_peaks"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Int("sky_color", 8756735),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:jungle", NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", 0.95f),
        NBTElement::Float("downfall", 0.9f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.overworld.jungle"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Int("sky_color", 7842047),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:lukewarm_ocean", NBTElement::Compound({
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
            NBTElement::Int("sky_color", 8103167),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4566514),
            NBTElement::Int("water_fog_color", 267827)
        })
    }) },
    { "minecraft:lush_caves", NBTElement::Compound({
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
            NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.overworld.lush_caves"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Int("sky_color", 8103167),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:mangrove_swamp", NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", 0.8f),
        NBTElement::Float("downfall", 0.9f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.overworld.swamp"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::String("grass_color_modifier", "swamp"),
            NBTElement::Int("sky_color", 7907327),
            NBTElement::Int("dry_foliage_color", 8082228),
            NBTElement::Int("foliage_color", 9285927),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 3832426),
            NBTElement::Int("water_fog_color", 5077600)
        })
    }) },
    { "minecraft:meadow", NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", 0.5f),
        NBTElement::Float("downfall", 0.8f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.overworld.meadow"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Int("sky_color", 8103167),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 937679),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:mushroom_fields", NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", 0.9f),
        NBTElement::Float("downfall", 1.0f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Int("sky_color", 7842047),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:nether_wastes", NBTElement::Compound({
        NBTElement::Byte("has_precipitation", false),
        NBTElement::Float("temperature", 2.0f),
        NBTElement::Float("downfall", 0.0f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.nether_wastes.mood"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.nether.nether_wastes"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Compound("additions_sound", {
                NBTElement::String("sound", "minecraft:ambient.nether_wastes.additions"),
                NBTElement::Double("tick_chance", 0.0111)
            }),
            NBTElement::String("ambient_sound", "minecraft:ambient.nether_wastes.loop"),
            NBTElement::Int("sky_color", 7254527),
            NBTElement::Int("fog_color", 3344392),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:ocean", NBTElement::Compound({
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
            NBTElement::Int("sky_color", 8103167),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:old_growth_birch_forest", NBTElement::Compound({
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
    }) },
    { "minecraft:old_growth_pine_taiga", NBTElement::Compound({
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
                NBTElement::String("sound", "minecraft:music.overworld.old_growth_taiga"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Int("sky_color", 8168447),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:old_growth_spruce_taiga", NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", 0.25f),
        NBTElement::Float("downfall", 0.8f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.overworld.old_growth_taiga"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Int("sky_color", 8233983),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:pale_garden", NBTElement::Compound({
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
            NBTElement::Int("sky_color", 12171705),
            NBTElement::Int("dry_foliage_color", 10528412),
            NBTElement::Int("foliage_color", 8883574),
            NBTElement::Int("grass_color", 7832178),
            NBTElement::Int("fog_color", 8484720),
            NBTElement::Int("water_color", 7768221),
            NBTElement::Int("water_fog_color", 5597568)
        })
    }) },
    { "minecraft:plains", NBTElement::Compound({
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
    }) },
    { "minecraft:river", NBTElement::Compound({
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
            NBTElement::Int("sky_color", 8103167),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:savanna", NBTElement::Compound({
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
            NBTElement::Int("sky_color", 7254527),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:savanna_plateau", NBTElement::Compound({
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
            NBTElement::Int("sky_color", 7254527),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:small_end_islands", NBTElement::Compound({
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
    }) },
    { "minecraft:snowy_beach", NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", 0.05f),
        NBTElement::Float("downfall", 0.3f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Int("sky_color", 8364543),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4020182),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:snowy_plains", NBTElement::Compound({
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
    }) },
    { "minecraft:snowy_slopes", NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", -0.3f),
        NBTElement::Float("downfall", 0.9f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.overworld.snowy_slopes"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Int("sky_color", 8560639),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:snowy_taiga", NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", -0.5f),
        NBTElement::Float("downfall", 0.4f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Int("sky_color", 8625919),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4020182),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:soul_sand_valley", NBTElement::Compound({
        NBTElement::Byte("has_precipitation", false),
        NBTElement::Float("temperature", 2.0f),
        NBTElement::Float("downfall", 0.0f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.soul_sand_valley.mood"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.nether.soul_sand_valley"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Compound("additions_sound", {
                NBTElement::String("sound", "minecraft:ambient.soul_sand_valley.additions"),
                NBTElement::Double("tick_chance", 0.0111)
            }),
            NBTElement::Compound("particle", {
                NBTElement::Compound("options", {
                    NBTElement::String("type", "minecraft:ash")
                }),
                NBTElement::Float("probability", 0.00625f)
            }),
            NBTElement::String("ambient_sound", "minecraft:ambient.soul_sand_valley.loop"),
            NBTElement::Int("sky_color", 7254527),
            NBTElement::Int("fog_color", 1787717),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:sparse_jungle", NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", 0.95f),
        NBTElement::Float("downfall", 0.8f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.overworld.sparse_jungle"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Int("sky_color", 7842047),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:stony_peaks", NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", 1.0f),
        NBTElement::Float("downfall", 0.3f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.overworld.stony_peaks"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Int("sky_color", 7776511),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:stony_shore", NBTElement::Compound({
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
    }) },
    { "minecraft:sunflower_plains", NBTElement::Compound({
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
    }) },
    { "minecraft:swamp", NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", 0.8f),
        NBTElement::Float("downfall", 0.9f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.overworld.swamp"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::String("grass_color_modifier", "swamp"),
            NBTElement::Int("sky_color", 7907327),
            NBTElement::Int("dry_foliage_color", 8082228),
            NBTElement::Int("foliage_color", 6975545),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 6388580),
            NBTElement::Int("water_fog_color", 2302743)
        })
    }) },
    { "minecraft:taiga", NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", 0.25f),
        NBTElement::Float("downfall", 0.8f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.overworld.taiga"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Int("sky_color", 8233983),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:the_end", NBTElement::Compound({
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
    }) },
    { "minecraft:the_void", NBTElement::Compound({
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
            NBTElement::Int("sky_color", 8103167),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:warm_ocean", NBTElement::Compound({
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
            NBTElement::Int("sky_color", 8103167),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4445678),
            NBTElement::Int("water_fog_color", 270131)
        })
    }) },
    { "minecraft:warped_forest", NBTElement::Compound({
        NBTElement::Byte("has_precipitation", false),
        NBTElement::Float("temperature", 2.0f),
        NBTElement::Float("downfall", 0.0f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.warped_forest.mood"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Compound("music", {
                NBTElement::String("sound", "minecraft:music.nether.warped_forest"),
                NBTElement::Int("min_delay", 12000),
                NBTElement::Int("max_delay", 24000),
                NBTElement::Byte("replace_current_music", false)
            }),
            NBTElement::Compound("additions_sound", {
                NBTElement::String("sound", "minecraft:ambient.warped_forest.additions"),
                NBTElement::Double("tick_chance", 0.0111)
            }),
            NBTElement::Compound("particle", {
                NBTElement::Compound("options", {
                    NBTElement::String("type", "minecraft:warped_spore")
                }),
                NBTElement::Float("probability", 0.01428f)
            }),
            NBTElement::String("ambient_sound", "minecraft:ambient.warped_forest.loop"),
            NBTElement::Int("sky_color", 7254527),
            NBTElement::Int("fog_color", 1705242),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:windswept_forest", NBTElement::Compound({
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
    }) },
    { "minecraft:windswept_gravelly_hills", NBTElement::Compound({
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
    }) },
    { "minecraft:windswept_hills", NBTElement::Compound({
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
    }) },
    { "minecraft:windswept_savanna", NBTElement::Compound({
        NBTElement::Byte("has_precipitation", true),
        NBTElement::Float("temperature", 2.0f),
        NBTElement::Float("downfall", 0.0f),
        NBTElement::Compound("effects", {
            NBTElement::Compound("mood_sound", {
                NBTElement::String("sound", "minecraft:ambient.cave"),
                NBTElement::Int("tick_delay", 6000),
                NBTElement::Int("block_search_extent", 8),
                NBTElement::Double("offset", 2.0)
            }),
            NBTElement::Int("sky_color", 7254527),
            NBTElement::Int("fog_color", 12638463),
            NBTElement::Int("water_color", 4159204),
            NBTElement::Int("water_fog_color", 329011)
        })
    }) },
    { "minecraft:wooded_badlands", NBTElement::Compound({
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
    }) }
};
std::unordered_map<std::string, NBTElement> g_damageType = {
    { "minecrat:arrow", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "arrow"), 
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:bad_respawn_point", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "badRespawnPoint"), 
        NBTElement::String("death_message_type", "intentional_game_design"), 
        NBTElement::String("scaling", "always")
    }) },
    { "minecrat:cactus", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "cactus"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:campfire", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "inFire"), 
        NBTElement::String("effects", "burning"), 
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:cramming", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.0f),
        NBTElement::String("message_id", "cramming"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:dragon_breath", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.0f),
        NBTElement::String("message_id", "dragonBreath"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:drown", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.0f),
        NBTElement::String("message_id", "drown"),
        NBTElement::String("effects", "drowning"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:dry_out", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "dryout"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:ender_pearl", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.0f),
        NBTElement::String("message_id", "fall"),
        NBTElement::String("death_message_type", "fall_variants"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:explosion", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "explosion"),
        NBTElement::String("scaling", "always")
    }) },
    { "minecrat:fall", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.0f),
        NBTElement::String("message_id", "fall"),
        NBTElement::String("death_message_type", "fall_variants"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:falling_anvil", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "anvil"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:falling_block", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "fallingBlock"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:falling_stalactite", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "fallingStalactite"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:fireball", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "fireball"), 
        NBTElement::String("effects", "burning"), 
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:fireworks", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "fireworks"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:fly_into_wall", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.0f),
        NBTElement::String("message_id", "flyIntoWall"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:freeze", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.0f),
        NBTElement::String("message_id", "freeze"),
        NBTElement::String("effects", "freezing"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:generic", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.0f),
        NBTElement::String("message_id", "generic"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:generic_kill", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.0f),
        NBTElement::String("message_id", "genericKill"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:hot_floor", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "hotFloor"), 
        NBTElement::String("effects", "burning"), 
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:in_fire", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "inFire"), 
        NBTElement::String("effects", "burning"), 
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:in_wall", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.0f),
        NBTElement::String("message_id", "inWall"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:indirect_magic", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.0f),
        NBTElement::String("message_id", "indirectMagic"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:lava", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "lava"), 
        NBTElement::String("effects", "burning"), 
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:lightning_bolt", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "lightningBolt"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:mace_smash", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "mace_smash"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:magic", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.0f),
        NBTElement::String("message_id", "magic"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:mob_attack", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "mob"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:mob_attack_no_aggro", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "mob"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:mob_projectile", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "mob"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:on_fire", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.0f),
        NBTElement::String("message_id", "onFire"), 
        NBTElement::String("effects", "burning"), 
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:out_of_world", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.0f),
        NBTElement::String("message_id", "outOfWorld"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:outside_border", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.0f),
        NBTElement::String("message_id", "outsideBorder"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:player_attack", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.0f),
        NBTElement::String("message_id", "player"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:player_explosion", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "explosion.player"),
        NBTElement::String("scaling", "always")
    }) },
    { "minecrat:sonic_boom", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "sonic_boom"),
        NBTElement::String("scaling", "always")
    }) },
    { "minecrat:spit", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.0f),
        NBTElement::String("message_id", "mob"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:stalagmite", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "stalagmite"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:starve", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.0f),
        NBTElement::String("message_id", "starve"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:sting", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.0f),
        NBTElement::String("message_id", "sting"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:sweet_berry_bush", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "sweetBerryBush"), 
        NBTElement::String("effects", "poking"), 
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:thorns", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "thorns"), 
        NBTElement::String("effects", "thorns"), 
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:thrown", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "thrown"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:trident", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "trident"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:unattributed_fireball", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "onFire"), 
        NBTElement::String("effects", "burning"), 
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:wind_charge", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "mob"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:wither", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.0f),
        NBTElement::String("message_id", "wither"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) },
    { "minecrat:wither_skull", NBTElement::Compound({
        NBTElement::Float("exhaustion", 0.1f),
        NBTElement::String("message_id", "witherSkull"),
        NBTElement::String("scaling", "when_caused_by_living_non_player")
    }) }
};
std::unordered_map<std::string, NBTElement> g_dimensionType = {
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
};
std::unordered_map<std::string, NBTElement> g_wolfVariant = {
    { "minecraft:ashen", NBTElement::Compound({
        NBTElement::Compound("assets", {
            NBTElement::String("wild", "minecraft:entity/wolf/wolf_ashen"),
            NBTElement::String("tame", "minecraft:entity/wolf/wolf_ashen_tame"),
            NBTElement::String("angry", "minecraft:entity/wolf/wolf_ashen_angry")
        }),
        NBTElement::String("biomes", "minecraft:snowy_taiga")
    }) },
    { "minecraft:black", NBTElement::Compound({
        NBTElement::Compound("assets", {
            NBTElement::String("wild", "minecraft:entity/wolf/wolf_black"),
            NBTElement::String("tame", "minecraft:entity/wolf/wolf_black_tame"),
            NBTElement::String("angry", "minecraft:entity/wolf/wolf_black_angry")
        }),
        NBTElement::String("biomes", "minecraft:old_growth_pine_taiga")
    }) },
    { "minecraft:chestnut", NBTElement::Compound({
        NBTElement::Compound("assets", {
            NBTElement::String("wild", "minecraft:entity/wolf/wolf_chestnut"),
            NBTElement::String("tame", "minecraft:entity/wolf/wolf_chestnut_tame"),
            NBTElement::String("angry", "minecraft:entity/wolf/wolf_chestnut_angry")
        }),
        NBTElement::String("biomes", "minecraft:old_growth_spruce_taiga")
    }) },
    { "minecraft:pale", NBTElement::Compound({
        NBTElement::Compound("assets", {
            NBTElement::String("wild", "minecraft:entity/wolf/wolf"),
            NBTElement::String("tame", "minecraft:entity/wolf/wolf_tame"),
            NBTElement::String("angry", "minecraft:entity/wolf/wolf_angry")
        }),
        NBTElement::String("biomes", "minecraft:taiga")
    }) },
    { "minecraft:rusty", NBTElement::Compound({
        NBTElement::Compound("assets", {
            NBTElement::String("wild", "minecraft:entity/wolf/wolf_rusty"),
            NBTElement::String("tame", "minecraft:entity/wolf/wolf_rusty_tame"),
            NBTElement::String("angry", "minecraft:entity/wolf/wolf_rusty_angry")
        }),
        NBTElement::String("biomes", "minecraft:sparse_jungle")
    }) },
    { "minecraft:snowy", NBTElement::Compound({
        NBTElement::Compound("assets", {
            NBTElement::String("wild", "minecraft:entity/wolf/wolf_snowy"),
            NBTElement::String("tame", "minecraft:entity/wolf/wolf_snowy_tame"),
            NBTElement::String("angry", "minecraft:entity/wolf/wolf_snowy_angry")
        }),
        NBTElement::String("biomes", "minecraft:grove")
    }) },
    { "minecraft:spotted", NBTElement::Compound({
        NBTElement::Compound("assets", {
            NBTElement::String("wild", "minecraft:entity/wolf/wolf_spotted"),
            NBTElement::String("tame", "minecraft:entity/wolf/wolf_spotted_tame"),
            NBTElement::String("angry", "minecraft:entity/wolf/wolf_spotted_angry")
        }),
        NBTElement::String("biomes", "minecraft:savanna_plateau")
    }) },
    { "minecraft:striped", NBTElement::Compound({
        NBTElement::Compound("assets", {
            NBTElement::String("wild", "minecraft:entity/wolf/wolf_striped"),
            NBTElement::String("tame", "minecraft:entity/wolf/wolf_striped_tame"),
            NBTElement::String("angry", "minecraft:entity/wolf/wolf_striped_angry")
        }),
        NBTElement::String("biomes", "minecraft:wooded_badlands")
    }) },
    { "minecraft:woods", NBTElement::Compound({
        NBTElement::Compound("assets", {
            NBTElement::String("wild", "minecraft:entity/wolf/wolf_woods"),
            NBTElement::String("tame", "minecraft:entity/wolf/wolf_woods_tame"),
            NBTElement::String("angry", "minecraft:entity/wolf/wolf_woods_angry")
        }),
        NBTElement::String("biomes", "minecraft:forest")
    }) }
};
std::unordered_map<std::string, NBTElement> g_wolfSoundVariant = {
    { "minecraft:angry", NBTElement::Compound({
        NBTElement::String("hurt_sound", "minecraft:entity.wolf_angry.hurt"),
        NBTElement::String("pant_sound", "minecraft:entity.wolf_angry.pant"),
        NBTElement::String("whine_sound", "minecraft:entity.wolf_angry.whine"),
        NBTElement::String("ambient_sound", "minecraft:entity.wolf_angry.ambient"),
        NBTElement::String("death_sound", "minecraft:entity.wolf_angry.death"),
        NBTElement::String("growl_sound", "minecraft:entity.wolf_angry.growl")
    }) },
    { "minecraft:big", NBTElement::Compound({
        NBTElement::String("hurt_sound", "minecraft:entity.wolf_big.hurt"),
        NBTElement::String("pant_sound", "minecraft:entity.wolf_big.pant"),
        NBTElement::String("whine_sound", "minecraft:entity.wolf_big.whine"),
        NBTElement::String("ambient_sound", "minecraft:entity.wolf_big.ambient"),
        NBTElement::String("death_sound", "minecraft:entity.wolf_big.death"),
        NBTElement::String("growl_sound", "minecraft:entity.wolf_big.growl")
    }) },
    { "minecraft:classic", NBTElement::Compound({
        NBTElement::String("hurt_sound", "minecraft:entity.wolf.hurt"),
        NBTElement::String("pant_sound", "minecraft:entity.wolf.pant"),
        NBTElement::String("whine_sound", "minecraft:entity.wolf.whine"),
        NBTElement::String("ambient_sound", "minecraft:entity.wolf.ambient"),
        NBTElement::String("death_sound", "minecraft:entity.wolf.death"),
        NBTElement::String("growl_sound", "minecraft:entity.wolf.growl")
    }) },
    { "minecraft:cute", NBTElement::Compound({
        NBTElement::String("hurt_sound", "minecraft:entity.wolf_cute.hurt"),
        NBTElement::String("pant_sound", "minecraft:entity.wolf_cute.pant"),
        NBTElement::String("whine_sound", "minecraft:entity.wolf_cute.whine"),
        NBTElement::String("ambient_sound", "minecraft:entity.wolf_cute.ambient"),
        NBTElement::String("death_sound", "minecraft:entity.wolf_cute.death"),
        NBTElement::String("growl_sound", "minecraft:entity.wolf_cute.growl")
    }) },
    { "minecraft:grumpy", NBTElement::Compound({
        NBTElement::String("hurt_sound", "minecraft:entity.wolf_grumpy.hurt"),
        NBTElement::String("pant_sound", "minecraft:entity.wolf_grumpy.pant"),
        NBTElement::String("whine_sound", "minecraft:entity.wolf_grumpy.whine"),
        NBTElement::String("ambient_sound", "minecraft:entity.wolf_grumpy.ambient"),
        NBTElement::String("death_sound", "minecraft:entity.wolf_grumpy.death"),
        NBTElement::String("growl_sound", "minecraft:entity.wolf_grumpy.growl")
    }) },
    { "minecraft:puglin", NBTElement::Compound({
        NBTElement::String("hurt_sound", "minecraft:entity.wolf_puglin.hurt"),
        NBTElement::String("pant_sound", "minecraft:entity.wolf_puglin.pant"),
        NBTElement::String("whine_sound", "minecraft:entity.wolf_puglin.whine"),
        NBTElement::String("ambient_sound", "minecraft:entity.wolf_puglin.ambient"),
        NBTElement::String("death_sound", "minecraft:entity.wolf_puglin.death"),
        NBTElement::String("growl_sound", "minecraft:entity.wolf_puglin.growl")
    }) },
    { "minecraft:sad", NBTElement::Compound({
        NBTElement::String("hurt_sound", "minecraft:entity.wolf_sad.hurt"),
        NBTElement::String("pant_sound", "minecraft:entity.wolf_sad.pant"),
        NBTElement::String("whine_sound", "minecraft:entity.wolf_sad.whine"),
        NBTElement::String("ambient_sound", "minecraft:entity.wolf_sad.ambient"),
        NBTElement::String("death_sound", "minecraft:entity.wolf_sad.death"),
        NBTElement::String("growl_sound", "minecraft:entity.wolf_sad.growl")
    }) }
};
std::unordered_map<std::string, NBTElement> g_pigVariant = {
    { "cold", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:entity/pig/cold_pig")
    }) },
    { "temperate", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:entity/pig/temperate_pig")
    }) },
    { "warm", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:entity/pig/warm_pig")
    }) }
};
std::unordered_map<std::string, NBTElement> g_frogVariant = {
    { "cold", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:entity/frog/cold_frog")
    }) },
    { "temperate", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:entity/frog/temperate_frog")
    }) },
    { "warm", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:entity/frog/warm_frog")
    }) }
};
std::unordered_map<std::string, NBTElement> g_cowVariant = {
    { "cold", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:entity/cow/cold_cow")
    }) },
    { "temperate", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:entity/cow/temperate_cow")
    }) },
    { "warm", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:entity/cow/warm_cow")
    }) }
};
std::unordered_map<std::string, NBTElement> g_chickenVariant = {
    { "cold", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:entity/chicken/cold_chicken")
    }) },
    { "temperate", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:entity/chicken/temperate_chicken")
    }) },
    { "warm", NBTElement::Compound({
        NBTElement::String("asset_id", "minecraft:entity/chicken/warm_chicken")
    }) }
};

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