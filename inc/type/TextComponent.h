#pragma once

#include <string>
#include <vector>
#include "Vector.h"
#include "Identifier.h"
#include "nbt/NBTElement.h"
#include "nbt/NBTSettings.h"
#include <external/UUID.h>

namespace zinc {

extern const std::string LEGACY_COLOR_BLACK;
extern const std::string LEGACY_COLOR_DARK_BLUE;
extern const std::string LEGACY_COLOR_DARK_GREEN;
extern const std::string LEGACY_COLOR_DARK_AQUA;
extern const std::string LEGACY_COLOR_DARK_RED;
extern const std::string LEGACY_COLOR_DARK_PURPLE;
extern const std::string LEGACY_COLOR_GOLD;
extern const std::string LEGACY_COLOR_GRAY;
extern const std::string LEGACY_COLOR_DARK_GRAY;
extern const std::string LEGACY_COLOR_BLUE;
extern const std::string LEGACY_COLOR_GREEN;
extern const std::string LEGACY_COLOR_AQUA;
extern const std::string LEGACY_COLOR_RED;
extern const std::string LEGACY_COLOR_LIGHT_PURPLE;
extern const std::string LEGACY_COLOR_YELLOW;
extern const std::string LEGACY_COLOR_WHITE;

extern const std::string LEGACY_FORMAT_OBFUSCATED;
extern const std::string LEGACY_FORMAT_BOLD;
extern const std::string LEGACY_FORMAT_STRIKETHROUGH;
extern const std::string LEGACY_FORMAT_UNDERLINED;
extern const std::string LEGACY_FORMAT_ITALIC;
extern const std::string LEGACY_FORMAT_RESET;

extern const NBTElement DEFAULT_SEPARATOR;

struct TextComponent {
    enum class Type : int { Text, Translatable, Score, Selector, Keybind, NBT };
    struct Translatable {
        std::string m_translate;
        std::optional<std::string> m_fallback;
        std::optional<std::vector<TextComponent>> m_with;
    };
    struct Score {
        std::string m_name;
        std::string m_objective;
    };
    struct Selector {
        std::string m_selector;
        std::optional<NBTElement> m_separator = DEFAULT_SEPARATOR;
    };
    struct NBT {
        enum class NBTType : int { Block, Entity, Storage };
        std::optional<NBTType> m_source;
        std::string m_nbt;
        std::optional<bool> m_interpret = false;
        std::optional<NBTElement> m_separator = DEFAULT_SEPARATOR;
        std::optional<std::string> m_block;
        std::optional<std::string> m_entity;
        std::optional<std::string> m_storage;
    };
    struct ClickEvent {
        enum class Action : int { OpenURL, OpenFile, RunCommand, SuggestCommand, ChangePage, CopyToClipboard };
        std::optional<Action> m_action;
        std::optional<std::string> m_url;
        std::optional<std::string> m_path;
        std::optional<std::string> m_command;
        std::optional<int> m_page;
        std::optional<std::string> m_value;
    };
    struct HoverEvent {
        enum class Action : int { ShowText, ShowItem, ShowEntity };
        struct ShowItem {
            std::string m_id;
            std::optional<int> m_count;
            std::optional<std::vector<NBTElement>> m_components;
        };
        struct ShowEntity {
            std::optional<std::vector<TextComponent>> m_name;
            std::string m_id;
            std::optional<uuids::uuid> m_UUID;
        };
        std::optional<Action> m_action;
        std::optional<std::vector<TextComponent>> m_value;
        std::optional<ShowItem> m_showItem;
        std::optional<ShowEntity> m_showEntity;
    };

    std::optional<Type> m_type;

    std::optional<std::string> m_text;
    std::optional<Translatable> m_translatable;
    std::optional<Score> m_score;
    std::optional<Selector> m_selector;
    std::optional<std::string> m_keybind;
    std::optional<NBT> m_NBT;

    std::optional<std::string> m_color;
    std::optional<std::string> m_font;
    std::optional<bool> m_bold;
    std::optional<bool> m_italic;
    std::optional<bool> m_underlined;
    std::optional<bool> m_strikethrough;
    std::optional<bool> m_obfuscated;

    std::vector<TextComponent> m_extra;

    std::optional<std::string> m_insertion;
    std::optional<ClickEvent> m_clickEvent;
    std::optional<HoverEvent> m_hoverEvent;

    void encode(ByteBuffer& buffer) const;
    void encodeJSON(ByteBuffer& buffer) const;
    std::string encodeJSON() const;
    void decode(ByteBuffer& buffer);
    void decodeJSON(ByteBuffer& buffer);
    void decodeJSON(const std::string& JSON);

    bool operator==(const TextComponent& text) const;
    bool operator!=(const TextComponent& text) const;
};

}