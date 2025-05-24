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

    NBTElement encode() const;
    std::string encodeJSON() const;
    void encodeJSON(ByteBuffer& buffer) const;
    void encode(ByteBuffer& buffer) const;
    void decode(const NBTElement& element);
    void decode(ByteBuffer& buffer);

    bool operator==(const TextComponent& text) const;
    bool operator!=(const TextComponent& text) const;
};
struct TextComponentBuilder {
private:
    TextComponent m_text;
public:
    struct TranslatableBuilder {
    private:
        TextComponent::Translatable m_translatable;
    public:
        TextComponent::Translatable& build();
        TextComponent::Translatable build() const;
        TranslatableBuilder& translate(const std::string& translate);
        TranslatableBuilder& fallback(const std::string& fallback);
        TranslatableBuilder& with(const std::vector<TextComponent>& with);
    };
    struct ScoreBuilder {
    private:
        TextComponent::Score m_score;
    public:
        TextComponent::Score& build();
        TextComponent::Score build() const;
        ScoreBuilder& name(const std::string& name);
        ScoreBuilder& objective(const std::string& objective);
    };
    struct SelectorBuilder {
    private:
        TextComponent::Selector m_selector;
    public:
        TextComponent::Selector& build();
        TextComponent::Selector build() const;
        SelectorBuilder& selector(const std::string& selector);
        SelectorBuilder& separator(const TextComponent& separator);
    };
    struct NBTBuilder {
    private:
        TextComponent::NBT m_NBT;
    public:
        TextComponent::NBT& build();
        TextComponent::NBT build() const;
        NBTBuilder& nbt(const std::string& nbt);
        NBTBuilder& block(const std::string& block);
        NBTBuilder& entity(const std::string& entity);
        NBTBuilder& storage(const std::string& storage);
        NBTBuilder& separator(const TextComponent& separator);
        NBTBuilder& interpret(const bool& state);
    };
    struct ClickEventBuilder {
    private:
        TextComponent::ClickEvent m_clickEvent;
    public:
        TextComponent::ClickEvent& build();
        TextComponent::ClickEvent build() const;
        ClickEventBuilder& openUrl(const std::string& url);
        ClickEventBuilder& openFile(const std::string& path);
        ClickEventBuilder& runCommand(const std::string& command);
        ClickEventBuilder& suggestCommand(const std::string& command);
        ClickEventBuilder& changePage(const int& page);
        ClickEventBuilder& copyToClipboard(const std::string& value);
    };
    struct HoverEventBuilder {
    private:
        TextComponent::HoverEvent m_hoverEvent;
    public:
        struct ShowItemBuilder {
        private:
            TextComponent::HoverEvent::ShowItem m_showItem;
        public:
            TextComponent::HoverEvent::ShowItem& build();
            TextComponent::HoverEvent::ShowItem build() const;
            ShowItemBuilder& id(const std::string& id);
            ShowItemBuilder& count(const int& count);
            ShowItemBuilder& components(const std::vector<NBTElement>& components);
        };
        struct ShowEntityBuilder {
        private:
            TextComponent::HoverEvent::ShowEntity m_showEntity;
        public:
            TextComponent::HoverEvent::ShowEntity& build();
            TextComponent::HoverEvent::ShowEntity build() const;
            ShowEntityBuilder& name(const TextComponent& name);
            ShowEntityBuilder& id(const std::string& id);
            ShowEntityBuilder& uuid(const uuids::uuid& uuid);
        };
        TextComponent::HoverEvent& build();
        TextComponent::HoverEvent build() const;
        HoverEventBuilder& showText(const TextComponent& text);
        HoverEventBuilder& showItem(const TextComponent::HoverEvent::ShowItem& item);
        HoverEventBuilder& showEntity(const TextComponent::HoverEvent::ShowEntity& entity);
    };
    TextComponent& build();
    TextComponent build() const;
    TextComponentBuilder& text(const std::string& text);
    TextComponentBuilder& translatable(const TextComponent::Translatable& translatable);
    TextComponentBuilder& score(const TextComponent::Score& score);
    TextComponentBuilder& selector(const TextComponent::Selector& selector);
    TextComponentBuilder& keybind(const std::string& keybind);
    TextComponentBuilder& nbt(const TextComponent::NBT& nbt);
    TextComponentBuilder& bold(const bool& state = true);
    TextComponentBuilder& italic(const bool& state = true);
    TextComponentBuilder& underlined(const bool& state = true);
    TextComponentBuilder& strikethrough(const bool& state = true);
    TextComponentBuilder& obfuscated(const bool& state = true);
    TextComponentBuilder& font(const Identifier& font = Identifier("minecraft:default"));
    TextComponentBuilder& color(const std::string& color = "gray");
    TextComponentBuilder& append(const TextComponent& text);
    TextComponentBuilder& insertion(const std::string& insertion);
    TextComponentBuilder& clickEvent(const TextComponent::ClickEvent& clickEvent);
    TextComponentBuilder& hoverEvent(const TextComponent::HoverEvent& hoverEvent);
};

}