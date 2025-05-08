#pragma once

#include <string>
#include <vector>
#include "Vector.h"
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

struct TextComponent {
public:
    enum class Type {
        Text, Translatable, Score, Selector, Keybind, NBT, Undefined
    };
    struct Content {

    };
    struct Format {
    private:
        std::string m_color;
        std::string m_font;
        bool m_isBold;
        bool m_isItalic;
        bool m_isUnderlined;
        bool m_isStrikethrough;
        bool m_isObfuscated;
        int m_shadowColor; // Red<<16 + Green<<8 + Blue
    public:
        Format() 
            : m_color("white"), m_font("minecraft:default"), m_isBold(false), m_isItalic(false), m_isUnderlined(false), m_isStrikethrough(false), 
              m_isObfuscated(false), m_shadowColor(0) {}
        
        void setColor(const std::string& color);
        void setFont(const std::string& font);
        void setBold(const bool& isBold);
        void setItalic(const bool& isItalic);
        void setUnderlined(const bool& isUnderlined);
        void setStrikethrough(const bool& isStrikethrough);
        void setObfuscated(const bool& isObfuscated);
        void setShadowColor(const int& shadowColor);
        void setShadowColor(const Vector3f& shadowColor);
        void setShadowColor(const Vector3i& shadowColor);

        std::string& getColor();
        std::string getColor() const;
        std::string& getFont();
        std::string getFont() const;
        bool& isBold();
        bool isBold() const;
        bool& isItalic();
        bool isItalic() const;
        bool& isUnderlined();
        bool isUnderlined() const;
        bool& isStrikethrough();
        bool isStrikethrough() const;
        bool& isObfuscated();
        bool isObfuscated() const;
        int& getShadowColor();
        int getShadowColor() const;
    };
    struct Interactivity {
    public:
        struct ClickEvent {
        public:
            enum class Action {
                OpenURL, OpenFile, RunCommand, SuggestCommand, ChangePage, CopyToClipboard, Undefined
            };
        private:
            Action m_action = Action::Undefined;
            std::string m_url; // OpenURL
            std::string m_path; // OpenFile
            std::string m_command; // RunCommand & SuggestCommand
            int m_page; // ChangePage
            std::string m_value; // CopyToClipboard
        public:
        };
        struct HoverEvent {
        public:
            enum class Action {
                ShowText, ShowItem, ShowEntity, Undefined
            };
        private:
            Action m_action = Action::Undefined;
            std::vector<TextComponent> m_value; // ShowText & ShowEntity
            std::string m_id; // ShowItem & ShowEntity
            int m_count; // ShowItem
            // TODO components // ShowItem
            uuids::uuid m_uuid;
        public:
        };
    private:
        std::string m_insertion;
        ClickEvent m_clickEvent;
        HoverEvent m_hoverEvent;
    public:
    };
private:
    Type m_type = Type::Undefined;
    Content m_content;
    std::vector<TextComponent> m_extra;
    Format m_format;
    Interactivity m_interactivity;
public:
};

}