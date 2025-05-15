#include <type/TextComponent.h>
#include <type/ByteBuffer.h>
#include <external/JSON.h>

namespace zinc {

const std::string LEGACY_COLOR_BLACK = "§0";
const std::string LEGACY_COLOR_DARK_BLUE = "§1";
const std::string LEGACY_COLOR_DARK_GREEN = "§2";
const std::string LEGACY_COLOR_DARK_AQUA = "§3";
const std::string LEGACY_COLOR_DARK_RED = "§4";
const std::string LEGACY_COLOR_DARK_PURPLE = "§5";
const std::string LEGACY_COLOR_GOLD = "§6";
const std::string LEGACY_COLOR_GRAY = "§c";
const std::string LEGACY_COLOR_DARK_GRAY = "§8";
const std::string LEGACY_COLOR_BLUE = "§9";
const std::string LEGACY_COLOR_GREEN = "§a";
const std::string LEGACY_COLOR_AQUA = "§b";
const std::string LEGACY_COLOR_RED = "§c";
const std::string LEGACY_COLOR_LIGHT_PURPLE = "§d";
const std::string LEGACY_COLOR_YELLOW = "§e";
const std::string LEGACY_COLOR_WHITE = "§f";

const std::string LEGACY_FORMAT_OBFUSCATED = "§k";
const std::string LEGACY_FORMAT_BOLD = "§l";
const std::string LEGACY_FORMAT_STRIKETHROUGH = "§m";
const std::string LEGACY_FORMAT_UNDERLINED = "§n";
const std::string LEGACY_FORMAT_ITALIC = "§o";
const std::string LEGACY_FORMAT_RESET = "§r";

void TextComponent::Format::setColor(const std::string& color) {
    m_color = color;
}
void TextComponent::Format::setFont(const std::string& font) {
    m_font = font;
}
void TextComponent::Format::setFont(const Identifier& font) {
    m_font = font;
}
void TextComponent::Format::setBold(const bool& isBold) {
    m_isBold = isBold;
}
void TextComponent::Format::setItalic(const bool& isItalic) {
    m_isItalic = isItalic;
}
void TextComponent::Format::setUnderlined(const bool& isUnderlined) {
    m_isUnderlined = isUnderlined;
}
void TextComponent::Format::setStrikethrough(const bool& isStrikethrough) {
    m_isStrikethrough = isStrikethrough;
}
void TextComponent::Format::setObfuscated(const bool& isObfuscated) {
    m_isObfuscated = isObfuscated;
}
void TextComponent::Format::setShadowColor(const int& shadowColor) {
    m_shadowColor = shadowColor;
}
void TextComponent::Format::setShadowColor(const Vector3f& shadowColor) {
    m_shadowColor = ( ((int)(shadowColor.getX() * 256) << 16) + ((int)(shadowColor.getY() * 256) << 8) + (int)(shadowColor.getZ() * 256) );
}
void TextComponent::Format::setShadowColor(const Vector3i& shadowColor) {
    m_shadowColor = ( (shadowColor.getX() << 16) + (shadowColor.getY() << 8) + shadowColor.getZ() );
}
std::string& TextComponent::Format::getColor() {
    return m_color;
}
std::string TextComponent::Format::getColor() const {
    return m_color;
}
Identifier& TextComponent::Format::getFont() {
    return m_font;
}
Identifier TextComponent::Format::getFont() const {
    return m_font;
}
bool& TextComponent::Format::isBold() {
    return m_isBold;
}
bool TextComponent::Format::isBold() const {
    return m_isBold;
}
bool& TextComponent::Format::isItalic() {
    return m_isItalic;
}
bool TextComponent::Format::isItalic() const {
    return m_isItalic;
}
bool& TextComponent::Format::isUnderlined() {
    return m_isUnderlined;
}
bool TextComponent::Format::isUnderlined() const {
    return m_isUnderlined;
}
bool& TextComponent::Format::isStrikethrough() {
    return m_isStrikethrough;
}
bool TextComponent::Format::isStrikethrough() const {
    return m_isStrikethrough;
}
bool& TextComponent::Format::isObfuscated() {
    return m_isObfuscated;
}
bool TextComponent::Format::isObfuscated() const {
    return m_isObfuscated;
}
int& TextComponent::Format::getShadowColor() {
    return m_shadowColor;
}
int TextComponent::Format::getShadowColor() const {
    return m_shadowColor;
}
bool TextComponent::Format::operator==(const Format& format) const {
    return m_color == format.getColor() && m_font == format.getFont() && m_isBold == format.isBold() && m_isItalic == format.isItalic() &&
           m_isUnderlined == format.isUnderlined() && m_isStrikethrough == format.isStrikethrough() && m_isObfuscated == format.isObfuscated() &&
           m_shadowColor == format.getShadowColor();
}
bool TextComponent::Format::operator!=(const Format& format) const {
    return !operator==(format);
}

void TextComponent::Interactivity::ClickEvent::setAction(const Action& action) {
    m_action = action;
}
void TextComponent::Interactivity::ClickEvent::setURL(const std::string& url) {
    m_url = url;
    m_action = Action::OpenURL;
}
void TextComponent::Interactivity::ClickEvent::setPath(const std::string& path) {
    m_path = path;
    m_action = Action::OpenFile;
}
void TextComponent::Interactivity::ClickEvent::setCommand(const std::string& command) {
    m_command = command;
}
void TextComponent::Interactivity::ClickEvent::setPage(const int& page) {
    m_page = page;
    m_action = Action::ChangePage;
}
void TextComponent::Interactivity::ClickEvent::setCopyValue(const std::string& value) {
    m_value = value;
    m_action = Action::CopyToClipboard;
}
TextComponent::Interactivity::ClickEvent::Action& TextComponent::Interactivity::ClickEvent::getAction() {
    return m_action;
}
TextComponent::Interactivity::ClickEvent::Action TextComponent::Interactivity::ClickEvent::getAction() const {
    return m_action;
}
std::string& TextComponent::Interactivity::ClickEvent::getURL() {
    return m_url;
}
std::string TextComponent::Interactivity::ClickEvent::getURL() const {
    return m_url;
}
std::string& TextComponent::Interactivity::ClickEvent::getPath() {
    return m_path;
}
std::string TextComponent::Interactivity::ClickEvent::getPath() const {
    return m_path;
}
std::string& TextComponent::Interactivity::ClickEvent::getCommand() {
    return m_command;
}
std::string TextComponent::Interactivity::ClickEvent::getCommand() const {
    return m_command;
}
int& TextComponent::Interactivity::ClickEvent::getPage() {
    return m_page;
}
int TextComponent::Interactivity::ClickEvent::getPage() const {
    return m_page;
}
std::string& TextComponent::Interactivity::ClickEvent::getCopyValue() {
    return m_value;
}
std::string TextComponent::Interactivity::ClickEvent::getCopyValue() const {
    return m_value;
}
bool TextComponent::Interactivity::ClickEvent::operator==(const ClickEvent& event) const {
    if (m_action != event.getAction()) return false;
    switch (m_action) {
    case Action::OpenURL: return m_url == event.getURL();
    case Action::OpenFile: return m_path == event.getPath();
    case Action::SuggestCommand: return m_command == event.getCommand();
    case Action::RunCommand: return m_command == event.getCommand();
    case Action::ChangePage: return m_page == event.getPage();
    case Action::CopyToClipboard: return m_value == event.getCopyValue();
    default: return true;
    }
}
bool TextComponent::Interactivity::ClickEvent::operator!=(const ClickEvent& event) const {
    return !operator==(event);
}
TextComponent::Interactivity::ClickEvent TextComponent::Interactivity::ClickEvent::OpenURL(const std::string& url) {
    ClickEvent event;
    event.setURL(url);
    return event;
}
TextComponent::Interactivity::ClickEvent TextComponent::Interactivity::ClickEvent::OpenFile(const std::string& path) {
    ClickEvent event;
    event.setPath(path);
    return event;
}
TextComponent::Interactivity::ClickEvent TextComponent::Interactivity::ClickEvent::RunCommand(const std::string& command) {
    ClickEvent event;
    event.setCommand(command);
    event.setAction(Action::RunCommand);
    return event;
}
TextComponent::Interactivity::ClickEvent TextComponent::Interactivity::ClickEvent::SuggestCommand(const std::string& command) {
    ClickEvent event;
    event.setCommand(command);
    event.setAction(Action::SuggestCommand);
    return event;
}
TextComponent::Interactivity::ClickEvent TextComponent::Interactivity::ClickEvent::ChangePage(const int& page) {
    ClickEvent event;
    event.setPage(page);
    return event;
}
TextComponent::Interactivity::ClickEvent TextComponent::Interactivity::ClickEvent::CopyToClipboard(const std::string& value) {
    ClickEvent event;
    event.setCopyValue(value);
    return event;
}

TextComponent::TextComponent(const std::vector<char>& bytes, const NBTSettings& settings) {
    ByteBuffer buffer (bytes);
    NBTElement NBT (settings);
    NBT.decode(buffer);
    decode(NBT);
}
TextComponent::TextComponent(const NBTElement& NBT) {
    decode(NBT);
}
void TextComponent::decode(const NBTElement& NBT) {
    m_tag = NBT.getTag();
    if (NBT.getType() == NBTElementType::String) {
        m_type = Type::Text;
        m_content.m_text = NBT.getString();
        return;
    }
    if (NBT.getType() != NBTElementType::Compound) return;
    bool hasTypeTag = false;
    size_t i = 0;
    for (const NBTElement& element : NBT.getChildElements()) {
        if (element.getTag() == "type") {
            hasTypeTag = true;
            break;
        }
        i++;
    }
    if (hasTypeTag) {
        std::string type = NBT.getChildElements()[i].getString();
        if (type == "text") m_type = Type::Text;
        if (type == "translatable") m_type = Type::Translatable;
        if (type == "score") m_type = Type::Score;
        if (type == "selector") m_type = Type::Selector;
        if (type == "keybind") m_type = Type::Keybind;
        if (type == "nbt") m_type = Type::NBT;
    } else {
        for (const NBTElement& element : NBT.getChildElements()) {
            if (element.getTag() == "text") m_type = Type::Text;
            if (element.getTag() == "translate") m_type = Type::Translatable;
            if (element.getTag() == "score") m_type = Type::Score;
            if (element.getTag() == "selector") m_type = Type::Selector;
            if (element.getTag() == "keybind") m_type = Type::Keybind;
            if (element.getTag() == "nbt") m_type = Type::NBT;
        }
    }
    for (const NBTElement& element : NBT.getChildElements()) {
        if (element.getTag() == "color") m_format.setColor(element.getString());
        if (element.getTag() == "font") m_format.setFont(element.getString());
        if (element.getTag() == "bold") m_format.setBold(element.getByte());
        if (element.getTag() == "italic") m_format.setItalic(element.getByte());
        if (element.getTag() == "obfuscated") m_format.setObfuscated(element.getByte());
        if (element.getTag() == "underlined") m_format.setUnderlined(element.getByte());
        if (element.getTag() == "strikethrough") m_format.setStrikethrough(element.getByte());
        if (element.getTag() == "shadow_color") {
            if (element.getType() == NBTElementType::Int) m_format.setShadowColor(element.getInt());
            if (element.getType() == NBTElementType::List && element.getChildElements().size() == 4) {
                if (element.getChildElements()[0].getType() == NBTElementType::Float) {
                    m_format.setShadowColor(Vector3f(
                        element.getChildElements()[0].getFloat(), 
                        element.getChildElements()[1].getFloat(), 
                        element.getChildElements()[2].getFloat()
                    ));
                }
            }
        }

        if (element.getTag() == "insertion") m_interactivity.m_insertion = element.getString();
        if (element.getTag() == "click_event") {
            hasTypeTag = false;
            i = 0;
            for (const NBTElement& subElement : element.getChildElements()) {
                if (subElement.getTag() == "action") {
                    hasTypeTag = true;
                    break;
                }
                i++;
            }
            if (hasTypeTag) {
                std::string type = element.getChildElements()[i].getString();
                if (type == "open_url") m_interactivity.m_clickEvent.setAction(Interactivity::ClickEvent::Action::OpenURL);
                if (type == "open_file") m_interactivity.m_clickEvent.setAction(Interactivity::ClickEvent::Action::OpenFile);
                if (type == "run_command") m_interactivity.m_clickEvent.setAction(Interactivity::ClickEvent::Action::RunCommand);
                if (type == "suggest_command") m_interactivity.m_clickEvent.setAction(Interactivity::ClickEvent::Action::SuggestCommand);
                if (type == "change_page") m_interactivity.m_clickEvent.setAction(Interactivity::ClickEvent::Action::ChangePage);
                if (type == "copy_to_clipboard") m_interactivity.m_clickEvent.setAction(Interactivity::ClickEvent::Action::CopyToClipboard);
            } else {
                for (const NBTElement& subElement : element.getChildElements()) {
                    if (subElement.getTag() == "url") m_interactivity.m_clickEvent.setAction(Interactivity::ClickEvent::Action::OpenURL);
                    if (subElement.getTag() == "path") m_interactivity.m_clickEvent.setAction(Interactivity::ClickEvent::Action::OpenFile);
                    if (subElement.getTag() == "command") m_interactivity.m_clickEvent.setAction(Interactivity::ClickEvent::Action::RunCommand);
                    if (subElement.getTag() == "page") m_interactivity.m_clickEvent.setAction(Interactivity::ClickEvent::Action::ChangePage);
                    if (subElement.getTag() == "value") m_interactivity.m_clickEvent.setAction(Interactivity::ClickEvent::Action::CopyToClipboard);
                }
            }
            for (const NBTElement& subElement : element.getChildElements()) {
                if (subElement.getTag() == "url") m_interactivity.m_clickEvent.setURL(subElement.getString());
                if (subElement.getTag() == "path") m_interactivity.m_clickEvent.setPath(subElement.getString());
                if (subElement.getTag() == "command") m_interactivity.m_clickEvent.setCommand(subElement.getString());
                if (subElement.getTag() == "page") m_interactivity.m_clickEvent.setPage(subElement.getInt());
                if (subElement.getTag() == "value") m_interactivity.m_clickEvent.setCopyValue(subElement.getString());
            } 
        }
        if (element.getTag() == "hover_event") {
            hasTypeTag = false;
            i = 0;
            for (const NBTElement& subElement : element.getChildElements()) {
                if (subElement.getTag() == "action") {
                    hasTypeTag = true;
                    break;
                }
                i++;
            }
            if (hasTypeTag) {
                std::string type = element.getChildElements()[i].getString();
                if (type == "show_text") m_interactivity.m_hoverEvent.m_action = Interactivity::HoverEvent::Action::ShowText;
                if (type == "show_item") m_interactivity.m_hoverEvent.m_action = Interactivity::HoverEvent::Action::ShowItem;
                if (type == "show_entity") m_interactivity.m_hoverEvent.m_action = Interactivity::HoverEvent::Action::ShowEntity;
            } else {
                for (const NBTElement& subElement : element.getChildElements()) {
                    if (subElement.getTag() == "value") m_interactivity.m_hoverEvent.m_action = Interactivity::HoverEvent::Action::ShowText;
                    if (subElement.getTag() == "components") m_interactivity.m_hoverEvent.m_action = Interactivity::HoverEvent::Action::ShowItem;
                    if (subElement.getTag() == "uuid") m_interactivity.m_hoverEvent.m_action = Interactivity::HoverEvent::Action::ShowEntity;
                }
            }
            for (const NBTElement& subElement : element.getChildElements()) {
                if (subElement.getTag() == "value") {
                    if (subElement.getType() == NBTElementType::List) {
                        for (const NBTElement& subSubElement : subElement.getChildElements()) m_interactivity.m_hoverEvent.m_value.push_back(subSubElement);
                    } else m_interactivity.m_hoverEvent.m_value = {subElement};
                }
                if (subElement.getTag() == "id") m_interactivity.m_hoverEvent.m_id = subElement.getString();
                if (subElement.getTag() == "count") m_interactivity.m_hoverEvent.m_count = subElement.getInt();
                if (subElement.getTag() == "name") m_interactivity.m_hoverEvent.m_value = {subElement};
                if (subElement.getTag() == "uuid") {
                    if (subElement.getType() == NBTElementType::List && subElement.getChildElements().size() == 4) {
                        if (subElement.getChildElements()[0].getType() == NBTElementType::Int) {
                            ByteBuffer buffer;
                            for (const NBTElement& subSubElement : subElement.getChildElements()) buffer.writeNumeric<int>(subSubElement.getInt());
                            m_interactivity.m_hoverEvent.m_uuid = buffer.readUUID();
                        }
                    } else if (subElement.getType() == NBTElementType::IntArray && subElement.getIntArray().size() == 4) {
                        ByteBuffer buffer;
                        buffer.writeArray<int>(subElement.getIntArray(), &ByteBuffer::writeNumeric<int>);
                        m_interactivity.m_hoverEvent.m_uuid = buffer.readUUID();
                    } else m_interactivity.m_hoverEvent.m_uuid = uuids::uuid::from_string(subElement.getString()).value();
                }
                if (subElement.getTag() == "components") m_interactivity.m_hoverEvent.m_components = subElement.getChildElements();
            }
        }

        switch (m_type) {
        case Type::Text: {
            if (element.getTag() == "text") m_content.m_text = element.getString();
            break;
        }
        case Type::Translatable: {
            if (element.getTag() == "translate") m_content.m_translate = element.getString();
            if (element.getTag() == "fallback") m_content.m_fallback = element.getString();
            if (element.getTag() == "with") {
                for (const NBTElement& subElement : element.getChildElements()) m_content.m_with.push_back(subElement);
            }
            break;
        }
        case Type::Score: {
            if (element.getTag() == "score") {
                for (const NBTElement& subElement : element.getChildElements()) {
                    if (subElement.getTag() == "name") m_content.m_name = element.getString();
                    if (subElement.getTag() == "objective") m_content.m_objective = element.getString();
                } 
            }
            break;
        }
        case Type::Selector: {
            if (element.getTag() == "selector") m_content.m_selector = element.getString();
            if (element.getTag() == "separator") m_content.m_separator = element;
            break;
        }
        case Type::Keybind: {
            if (element.getTag() == "keybind") m_content.m_keybind = element.getString();
            break;
        }
        case Type::NBT: {
            if (element.getTag() == "source") m_content.m_source = element.getString();
            if (element.getTag() == "nbt") m_content.m_nbt = element.getString();
            if (element.getTag() == "interpret") m_content.m_interpret = element.getByte();
            if (element.getTag() == "separator") m_content.m_separator = element;
            if (element.getTag() == "block") m_content.m_block = element.getString();
            if (element.getTag() == "entity") m_content.m_entity = element.getString();
            if (element.getTag() == "storage") m_content.m_storage = element.getString();
            break;
        }
        default: break;
        }

        if (element.getTag() == "extra") {
            for (const NBTElement& subElement : element.getChildElements()) m_extra.push_back(subElement);
        }
    }
}
NBTElement TextComponent::encode(const NBTSettings& settings) const {
    std::string typeString;
    switch (m_type) {
    case Type::Text: typeString = "text"; break;
    case Type::Translatable: typeString = "translatable"; break;
    case Type::Score: typeString = "score"; break;
    case Type::Selector: typeString = "selector"; break;
    case Type::Keybind: typeString = "keybind"; break;
    case Type::NBT: typeString = "nbt"; break;
    default: break;
    }
    NBTElement clickEvent = NBTElement::Compound("click_event", {});
    NBTElement hoverEvent = NBTElement::Compound("hover_event", {});
    NBTElement NBT = NBTElement::Compound(m_tag, {
        NBTElement::String("type", typeString),
        NBTElement::String("insertion", m_interactivity.m_insertion),
        NBTElement::String("color", m_format.getColor()),
        NBTElement::String("font", m_format.getFont().toString()),
        NBTElement::Byte("bold", m_format.isBold()),
        NBTElement::Byte("italic", m_format.isItalic()),
        NBTElement::Byte("strikethrough", m_format.isStrikethrough()),
        NBTElement::Byte("underlined", m_format.isUnderlined()),
        NBTElement::Byte("obfuscated", m_format.isObfuscated()),
        NBTElement::Int("shadow_color", m_format.getShadowColor())
    }, settings);
    NBTElement extra = NBTElement::List("extra", {});
    for (const TextComponent& text : m_extra) extra.getChildElements().push_back(text.encode(settings));
    NBT.getChildElements().push_back(extra);
    switch (m_type) {
    case Type::Text: NBT.getChildElements().push_back(NBTElement::String("text", m_content.m_text)); break;
    case Type::Translatable: {
        NBT.getChildElements().push_back(NBTElement::String("translate", m_content.m_translate));
        NBT.getChildElements().push_back(NBTElement::String("fallback", m_content.m_fallback));
        NBTElement with = NBTElement::List("with", {});
        for (const TextComponent& text : m_content.m_with) with.getChildElements().push_back(text.encode(settings));
        NBT.getChildElements().push_back(with);
        break;
    }
    case Type::Score: NBT.getChildElements().push_back(NBTElement::Compound("score", {
        NBTElement::String("name", m_content.m_name),
        NBTElement::String("objective", m_content.m_objective)
    })); break;
    case Type::Selector: {
        NBT.getChildElements().push_back(NBTElement::String("selector", m_content.m_selector)); break;
        NBTElement separator = m_content.m_separator;
        separator.setTag("separator");
        NBT.getChildElements().push_back(separator); break;
        break;
    }
    case Type::Keybind: NBT.getChildElements().push_back(NBTElement::String("keybind", m_content.m_keybind)); break;
    case Type::NBT: {
        NBT.getChildElements().push_back(NBTElement::String("source", m_content.m_source));
        NBT.getChildElements().push_back(NBTElement::String("nbt", m_content.m_nbt));
        NBT.getChildElements().push_back(NBTElement::Byte("interpret", m_content.m_interpret));
        NBTElement separator = m_content.m_separator;
        separator.setTag("separator");
        NBT.getChildElements().push_back(separator); break;
        NBT.getChildElements().push_back(NBTElement::String("block", m_content.m_block));
        NBT.getChildElements().push_back(NBTElement::String("entity", m_content.m_entity));
        NBT.getChildElements().push_back(NBTElement::String("storage", m_content.m_storage));
        break;
    }
    default: break;
    }
    std::string action;
    switch (m_interactivity.m_clickEvent.getAction()) {
    case Interactivity::ClickEvent::Action::OpenURL: action = "open_url"; break;
    case Interactivity::ClickEvent::Action::OpenFile: action = "open_path"; break;
    case Interactivity::ClickEvent::Action::RunCommand: action = "run_command"; break;
    case Interactivity::ClickEvent::Action::SuggestCommand: action = "suggest_command"; break;
    case Interactivity::ClickEvent::Action::ChangePage: action = "change_page"; break;
    case Interactivity::ClickEvent::Action::CopyToClipboard: action = "copy_to_clipboard"; break;
    default: break;
    }
    clickEvent.getChildElements().push_back(NBTElement::String("action", action));
    switch (m_interactivity.m_clickEvent.getAction()) {
    case Interactivity::ClickEvent::Action::OpenURL: {
        clickEvent.getChildElements().push_back(NBTElement::String("url", m_interactivity.m_clickEvent.getURL()));
        break;
    }
    case Interactivity::ClickEvent::Action::OpenFile: {
        clickEvent.getChildElements().push_back(NBTElement::String("path", m_interactivity.m_clickEvent.getPath()));
        break;
    }
    case Interactivity::ClickEvent::Action::RunCommand: {
        clickEvent.getChildElements().push_back(NBTElement::String("command", m_interactivity.m_clickEvent.getCommand()));
        break;
    }
    case Interactivity::ClickEvent::Action::SuggestCommand: {
        clickEvent.getChildElements().push_back(NBTElement::String("command", m_interactivity.m_clickEvent.getCommand()));
        break;
    }
    case Interactivity::ClickEvent::Action::ChangePage: {
        clickEvent.getChildElements().push_back(NBTElement::Int("page", m_interactivity.m_clickEvent.getPage()));
        break;
    }
    case Interactivity::ClickEvent::Action::CopyToClipboard: {
        clickEvent.getChildElements().push_back(NBTElement::String("value", m_interactivity.m_clickEvent.getCopyValue()));
        break;
    }
    default: break;
    }

    action;
    switch (m_interactivity.m_hoverEvent.m_action) {
    case Interactivity::HoverEvent::Action::ShowText: action = "show_text"; break;
    case Interactivity::HoverEvent::Action::ShowItem: action = "show_item"; break;
    case Interactivity::HoverEvent::Action::ShowEntity: action = "show_entity"; break;
    default: break;
    }
    hoverEvent.getChildElements().push_back(NBTElement::String("action", action));
    switch (m_interactivity.m_hoverEvent.m_action) {
    case Interactivity::HoverEvent::Action::ShowText: {
        NBTElement value;
        if (m_interactivity.m_hoverEvent.m_value.empty()) value = NBTElement::Compound("value", {});
        else value = m_interactivity.m_hoverEvent.m_value[0].encode(settings);
        hoverEvent.getChildElements().push_back(value);
        break;
    }
    case Interactivity::HoverEvent::Action::ShowItem: {
        hoverEvent.getChildElements().push_back(NBTElement::String("id", m_interactivity.m_hoverEvent.m_id));
        hoverEvent.getChildElements().push_back(NBTElement::Int("count", m_interactivity.m_hoverEvent.m_count));
        hoverEvent.getChildElements().push_back(NBTElement::Compound("compound", m_interactivity.m_hoverEvent.m_components));
        break;
    }
    case Interactivity::HoverEvent::Action::ShowEntity: {
        NBTElement value;
        if (m_interactivity.m_hoverEvent.m_value.empty()) value = NBTElement::Compound("value", {});
        else value = m_interactivity.m_hoverEvent.m_value[0].encode(settings);
        hoverEvent.getChildElements().push_back(value);
        hoverEvent.getChildElements().push_back(NBTElement::String("id", m_interactivity.m_hoverEvent.m_id));
        hoverEvent.getChildElements().push_back(NBTElement::String("uuid", uuids::to_string(m_interactivity.m_hoverEvent.m_uuid)));
        break;
    }
    default: break;
    }

    NBT.getChildElements().push_back(clickEvent);
    NBT.getChildElements().push_back(hoverEvent);
    return NBT;
}
bool TextComponent::operator==(const TextComponent& text) const {
    return encode(NBTSettings()) == text.encode(NBTSettings());
}
bool TextComponent::operator!=(const TextComponent& text) const {
    return encode(NBTSettings()) != text.encode(NBTSettings());
}

}