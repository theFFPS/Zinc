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
const std::string LEGACY_COLOR_GRAY = "§7";
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

const NBTElement DEFAULT_SEPARATOR = NBTElement::Compound({
    NBTElement::String("type", "text"),
    NBTElement::String("color", "gray"),
    NBTElement::String("text", ", ")
});

NBTElement TextComponent::encode() const {
    NBTElement element = NBTElement::Compound({});
    Type type = Type::Text;
    if (m_type.has_value()) type = m_type.value();
    else {
        if (m_text.has_value()) type = Type::Text;
        else if (m_translatable.has_value()) type = Type::Translatable;
        else if (m_score.has_value()) type = Type::Score;
        else if (m_selector.has_value()) type = Type::Selector;
        else if (m_keybind.has_value()) type = Type::Keybind;
        else if (m_NBT.has_value()) type = Type::NBT;
    }
    switch (type) {
    case Type::Text: {
        element.m_childElements.push_back(NBTElement::String("type", "text"));
        element.m_childElements.push_back(NBTElement::String("text", m_text.value_or("")));
        break;
    }
    case Type::Translatable: {
        element.m_childElements.push_back(NBTElement::String("type", "translatable"));
        Translatable translatable = m_translatable.value_or(Translatable());
        element.m_childElements.push_back(NBTElement::String("translate", translatable.m_translate));
        element.m_childElements.push_back(NBTElement::String("fallback", translatable.m_fallback.value_or("nil")));
        std::vector<NBTElement> elements;
        if (translatable.m_with.has_value()) for (const TextComponent& text : translatable.m_with.value()) elements.push_back(text.encode());
        if (elements.size()) element.m_childElements.push_back(NBTElement::List("with", elements));
        break;
    }
    case Type::Score: {
        element.m_childElements.push_back(NBTElement::String("type", "score"));
        Score score = m_score.value_or(Score());
        element.m_childElements.push_back(NBTElement::Compound("score", {
            NBTElement::String("name", score.m_name),
            NBTElement::String("objective", score.m_objective)
        }));
        break;
    }
    case Type::Selector: {
        element.m_childElements.push_back(NBTElement::String("type", "selector"));
        Selector selector = m_selector.value_or(Selector());
        element.m_childElements.push_back(NBTElement::String("selector", selector.m_selector));
        element.m_childElements.push_back(NBTElement::Compound("separator", selector.m_separator.value_or(DEFAULT_SEPARATOR).m_childElements));
        break;
    }
    case Type::Keybind: {
        element.m_childElements.push_back(NBTElement::String("type", "keybind"));
        element.m_childElements.push_back(NBTElement::String("keybind", m_keybind.value_or("key.inventory")));
        break;
    }
    case Type::NBT: {
        element.m_childElements.push_back(NBTElement::String("type", "nbt"));
        NBT nbt = m_NBT.value_or(NBT());

        NBT::NBTType src = NBT::NBTType::Block;
        if (nbt.m_source.has_value()) src = nbt.m_source.value();
        else {
            if (nbt.m_block.has_value()) src = NBT::NBTType::Block;
            else if (nbt.m_entity.has_value()) src = NBT::NBTType::Entity;
            else if (nbt.m_storage.has_value()) src = NBT::NBTType::Storage;
        }

        switch (src) {
        case NBT::NBTType::Block: {
            element.m_childElements.push_back(NBTElement::String("source", "block"));
            element.m_childElements.push_back(NBTElement::String("block", nbt.m_block.value_or("")));
            break;
        }
        case NBT::NBTType::Entity: {
            element.m_childElements.push_back(NBTElement::String("source", "entity"));
            element.m_childElements.push_back(NBTElement::String("entity", nbt.m_entity.value_or("")));
            break;
        }
        case NBT::NBTType::Storage: {
            element.m_childElements.push_back(NBTElement::String("source", "storage"));
            element.m_childElements.push_back(NBTElement::String("storage", nbt.m_storage.value_or("")));
            break;
        }
        default: break;
        }
        element.m_childElements.push_back(NBTElement::String("nbt", nbt.m_nbt));
        if (nbt.m_interpret.has_value())
            element.m_childElements.push_back(NBTElement::Byte("interpret", nbt.m_interpret.value()));
        element.m_childElements.push_back(NBTElement::Compound("separator", nbt.m_separator.value_or(DEFAULT_SEPARATOR).m_childElements));

        break;
    }
    default: break;
    }
    std::vector<NBTElement> elements;
    for (const TextComponent& text : m_extra) elements.push_back(text.encode());
    if (elements.size()) element.m_childElements.push_back(NBTElement::List("extra", elements));
    if (m_color.has_value()) element.m_childElements.push_back(NBTElement::String("color", m_color.value()));
    if (m_font.has_value()) element.m_childElements.push_back(NBTElement::String("font", m_font.value()));
    if (m_bold.has_value()) element.m_childElements.push_back(NBTElement::Byte("bold", m_bold.value()));
    if (m_italic.has_value()) element.m_childElements.push_back(NBTElement::Byte("italic", m_italic.value()));
    if (m_underlined.has_value()) element.m_childElements.push_back(NBTElement::Byte("underlined", m_underlined.value()));
    if (m_strikethrough.has_value()) element.m_childElements.push_back(NBTElement::Byte("strikethrough", m_strikethrough.value()));
    if (m_obfuscated.has_value()) element.m_childElements.push_back(NBTElement::Byte("obfuscated", m_obfuscated.value()));
    if (m_insertion.has_value()) element.m_childElements.push_back(NBTElement::String("insertion", m_insertion.value()));
    if (m_clickEvent.has_value()) {
        NBTElement clickEventElement = NBTElement::Compound("click_event", {});
        ClickEvent::Action action = ClickEvent::Action::SuggestCommand;
        const ClickEvent& clickEvent = m_clickEvent.value();
        if (clickEvent.m_action.has_value()) action = clickEvent.m_action.value();
        else {
            if (clickEvent.m_url.has_value()) action = ClickEvent::Action::OpenURL;
            else if (clickEvent.m_path.has_value()) action = ClickEvent::Action::OpenFile;
            else if (clickEvent.m_page.has_value()) action = ClickEvent::Action::ChangePage;
            else if (clickEvent.m_value.has_value()) action = ClickEvent::Action::CopyToClipboard;
        }

        switch (action) {
        case ClickEvent::Action::OpenURL: {
            clickEventElement.m_childElements.push_back(NBTElement::String("action", "open_url"));
            clickEventElement.m_childElements.push_back(NBTElement::String("url", clickEvent.m_url.value_or("www.example.com")));
            break;
        }
        case ClickEvent::Action::OpenFile: {
            clickEventElement.m_childElements.push_back(NBTElement::String("action", "open_file"));
            clickEventElement.m_childElements.push_back(NBTElement::String("path", clickEvent.m_path.value_or("")));
            break;
        }
        case ClickEvent::Action::RunCommand: {
            clickEventElement.m_childElements.push_back(NBTElement::String("action", "run_command"));
            clickEventElement.m_childElements.push_back(NBTElement::String("command", clickEvent.m_command.value_or("/say hi")));
            break;
        }
        case ClickEvent::Action::SuggestCommand: {
            clickEventElement.m_childElements.push_back(NBTElement::String("action", "suggest_command"));
            clickEventElement.m_childElements.push_back(NBTElement::String("command", clickEvent.m_command.value_or("/say hi")));
            break;
        }
        case ClickEvent::Action::ChangePage: {
            clickEventElement.m_childElements.push_back(NBTElement::String("action", "change_page"));
            clickEventElement.m_childElements.push_back(NBTElement::Int("page", clickEvent.m_page.value_or(0)));
            break;
        }
        case ClickEvent::Action::CopyToClipboard: {
            clickEventElement.m_childElements.push_back(NBTElement::String("action", "copy_to_clipboard"));
            clickEventElement.m_childElements.push_back(NBTElement::String("value", clickEvent.m_value.value_or("")));
            break;
        }
        default: break;
        }
        element.m_childElements.push_back(clickEventElement);
    }
    if (m_hoverEvent.has_value()) {
        NBTElement hoverEventElement = NBTElement::Compound("hover_event", {});
        HoverEvent::Action action = HoverEvent::Action::ShowText;
        const HoverEvent& hoverEvent = m_hoverEvent.value();
        if (hoverEvent.m_action.has_value()) action = hoverEvent.m_action.value();
        else {
            if (hoverEvent.m_showItem.has_value()) action = HoverEvent::Action::ShowItem;
            else if (hoverEvent.m_showEntity.has_value()) action = HoverEvent::Action::ShowEntity;
        }

        switch (action) {
        case HoverEvent::Action::ShowText: {
            hoverEventElement.m_childElements.push_back(NBTElement::String("action", "show_text"));
            if (hoverEvent.m_value.has_value()) {
                if (hoverEvent.m_value.value().size()) {
                    NBTElement textNBT = hoverEvent.m_value.value()[0].encode();
                    textNBT.m_tag = "value";
                    hoverEventElement.m_childElements.push_back(textNBT);
                } else hoverEventElement.m_childElements.push_back(NBTElement::String("value", ""));
            } else hoverEventElement.m_childElements.push_back(NBTElement::String("value", ""));
            break;
        }
        case HoverEvent::Action::ShowItem: {
            const HoverEvent::ShowItem& item = hoverEvent.m_showItem.value_or(HoverEvent::ShowItem());
            hoverEventElement.m_childElements.push_back(NBTElement::String("action", "show_item"));
            hoverEventElement.m_childElements.push_back(NBTElement::String("id", item.m_id));
            if (item.m_count.has_value()) hoverEventElement.m_childElements.push_back(NBTElement::Int("count", item.m_count.value()));
            if (item.m_components.has_value()) {
                if (item.m_components.value().size())
                    hoverEventElement.m_childElements.push_back(NBTElement::List("components", item.m_components.value()));
            }
            break;
        }
        case HoverEvent::Action::ShowEntity: {
            const HoverEvent::ShowEntity& entity = hoverEvent.m_showEntity.value_or(HoverEvent::ShowEntity());
            hoverEventElement.m_childElements.push_back(NBTElement::String("action", "show_entity"));
            hoverEventElement.m_childElements.push_back(NBTElement::String("id", entity.m_id));
            if (entity.m_name.has_value()) {
                if (entity.m_name.value().size()) {
                    NBTElement textNBT = entity.m_name.value()[0].encode();
                    textNBT.m_tag = "name";
                    hoverEventElement.m_childElements.push_back(textNBT);
                }
            }
            if (entity.m_UUID.has_value()) 
                hoverEventElement.m_childElements.push_back(NBTElement::String("uuid", uuids::to_string(entity.m_UUID.value())));
            break;
        }
        default: break;
        }
        element.m_childElements.push_back(hoverEventElement);
    }
    return element;
}
std::string TextComponent::encodeJSON() const {
    std::string result = "[";
    TextComponent baseText = *this;
    baseText.m_extra.clear();
    result += baseText.encode().toJSON();
    for (const TextComponent& text : m_extra) {
        std::string temporaryJSONArray = text.encodeJSON();
        if (temporaryJSONArray.size() > 2) result += "," + std::string(temporaryJSONArray.begin() + 1, temporaryJSONArray.end() - 1);
    }
    return result + "]";
}
void TextComponent::encodeJSON(ByteBuffer& buffer) const {
    buffer.writeString(encodeJSON());
}
void TextComponent::encode(ByteBuffer& buffer) const {
    buffer.writeNBTElement(encode());
}
void TextComponent::decode(ByteBuffer& buffer) {
    decode(buffer.readNBTElement());
}
void TextComponent::decode(const NBTElement& element) {
    std::string typeString;
    for (const NBTElement& subelement : element.m_childElements) {
        if (subelement.m_tag == "extra") {
            for (const NBTElement& text : subelement.m_childElements) {
                TextComponent textComponent;
                ByteBuffer buf;
                buf.writeNBTElement(text);
                textComponent.decode(buf);
                m_extra.push_back(textComponent);
            }
        }
        if (subelement.m_tag == "type") typeString = subelement.m_stringValue;
        if (subelement.m_tag == "color") m_color = subelement.m_stringValue;
        if (subelement.m_tag == "font") m_font = subelement.m_stringValue;
        if (subelement.m_tag == "bold") m_bold = subelement.m_byteValue;
        if (subelement.m_tag == "italic") m_italic = subelement.m_byteValue;
        if (subelement.m_tag == "underlined") m_underlined = subelement.m_byteValue;
        if (subelement.m_tag == "strikethrough") m_strikethrough = subelement.m_byteValue;
        if (subelement.m_tag == "obfuscated") m_obfuscated = subelement.m_byteValue;
        if (subelement.m_tag == "insertion") m_insertion = subelement.m_stringValue;
        if (subelement.m_tag == "click_event") {
            std::string actionString;
            ClickEvent event;
            for (const NBTElement& eventSubelement : subelement.m_childElements) {
                if (eventSubelement.m_tag == "action") actionString = eventSubelement.m_stringValue;
            }
            if (actionString.empty()) for (const NBTElement& eventSubelement : subelement.m_childElements) {
                if (eventSubelement.m_tag == "url") {
                    actionString = "open_url";
                    event.m_url = eventSubelement.m_stringValue;
                }
                if (eventSubelement.m_tag == "path") {
                    actionString = "open_file";
                    event.m_path = eventSubelement.m_stringValue;
                }
                if (eventSubelement.m_tag == "command") {
                    actionString = "suggest_command";
                    event.m_command = eventSubelement.m_stringValue;
                }
                if (eventSubelement.m_tag == "page") {
                    actionString = "change_page";
                    event.m_page = eventSubelement.m_intValue;
                }
                if (eventSubelement.m_tag == "value") {
                    actionString = "copy_to_clipboard";
                    event.m_value = eventSubelement.m_stringValue;
                }
            }
            if (actionString == "open_url") event.m_action = ClickEvent::Action::OpenURL;
            if (actionString == "open_file") event.m_action = ClickEvent::Action::OpenFile;
            if (actionString == "run_command") event.m_action = ClickEvent::Action::RunCommand;
            if (actionString == "suggest_command") event.m_action = ClickEvent::Action::SuggestCommand;
            if (actionString == "change_page") event.m_action = ClickEvent::Action::ChangePage;
            if (actionString == "copy_to_clipboard") event.m_action = ClickEvent::Action::CopyToClipboard;
            m_clickEvent = event;
        }
        if (subelement.m_tag == "hover_event") {
            std::string actionString;
            HoverEvent event;
            HoverEvent::ShowItem showItem;
            HoverEvent::ShowEntity showEntity;
            std::string id;
            for (const NBTElement& eventSubelement : subelement.m_childElements) {
                if (eventSubelement.m_tag == "action") actionString = eventSubelement.m_stringValue;
            }
            if (actionString.empty()) for (const NBTElement& eventSubelement : subelement.m_childElements) {
                if (eventSubelement.m_tag == "value") {
                    actionString = "show_text";
                    TextComponent textComponent;
                    ByteBuffer buf;
                    buf.writeNBTElement(eventSubelement);
                    textComponent.decode(buf);
                    event.m_value = { textComponent };
                }
                if (eventSubelement.m_tag == "components") {
                    actionString = "show_item";
                    showItem.m_components = eventSubelement.m_childElements;
                }
                if (eventSubelement.m_tag == "count") {
                    actionString = "show_item";
                    showItem.m_count = eventSubelement.m_intValue;
                }
                if (eventSubelement.m_tag == "id") {
                    id = eventSubelement.m_stringValue;
                }
                if (eventSubelement.m_tag == "uuid") {
                    actionString = "show_entity";
                    showEntity.m_UUID = uuids::uuid::from_string(eventSubelement.m_stringValue);
                }
                if (eventSubelement.m_tag == "name") {
                    actionString = "show_entity";
                    TextComponent textComponent;
                    ByteBuffer buf;
                    buf.writeNBTElement(eventSubelement);
                    textComponent.decode(buf);
                    showEntity.m_name = { textComponent };
                }
            }
            if (actionString == "show_text") event.m_action = HoverEvent::Action::ShowText;
            if (actionString == "show_item") {
                event.m_action = HoverEvent::Action::ShowItem;
                showItem.m_id = id;
                event.m_showItem = showItem;
            }
            if (actionString == "show_entity") {
                event.m_action = HoverEvent::Action::ShowEntity;
                showEntity.m_id = id;
                event.m_showEntity = showEntity;
            }
            m_hoverEvent = event;
        }
    }
    Translatable translatable;
    std::vector<TextComponent> with;
    NBTElement separator;
    Score score;
    Selector selector;
    NBT nbt;
    if (typeString.empty()) for (const NBTElement& subelement : element.m_childElements) {
        if (subelement.m_tag == "text") {
            typeString = "text";
            m_text = subelement.m_stringValue;
        }
        if (subelement.m_tag == "source") {
            typeString = "nbt";
            std::string sourceString = subelement.m_stringValue;
            if (sourceString == "block") nbt.m_source = NBT::NBTType::Block;
            if (sourceString == "entity") nbt.m_source = NBT::NBTType::Entity;
            if (sourceString == "storage") nbt.m_source = NBT::NBTType::Storage;
        }
        if (subelement.m_tag == "nbt") {
            typeString = "nbt";
            nbt.m_nbt = subelement.m_stringValue;
        }
        if (subelement.m_tag == "block") {
            typeString = "nbt";
            nbt.m_block = subelement.m_stringValue;
        }
        if (subelement.m_tag == "entity") {
            typeString = "nbt";
            nbt.m_entity = subelement.m_stringValue;
        }
        if (subelement.m_tag == "storage") {
            typeString = "nbt";
            nbt.m_storage = subelement.m_stringValue;
        }
        if (subelement.m_tag == "interpret") {
            typeString = "nbt";
            nbt.m_interpret = subelement.m_byteValue;
        }
        if (subelement.m_tag == "score") {
            typeString = "score";
            for (const NBTElement& scoreElement : subelement.m_childElements) {
                if (scoreElement.m_tag == "name") score.m_name = scoreElement.m_stringValue;
                if (scoreElement.m_tag == "objective") score.m_objective = scoreElement.m_stringValue;
            }
        }
        if (subelement.m_tag == "separator") {
            separator = subelement;
        }
        if (subelement.m_tag == "selector") {
            typeString = "selector";
            selector.m_selector = subelement.m_stringValue;
        }
        if (subelement.m_tag == "keybind") {
            typeString = "keybind";
            m_keybind = subelement.m_stringValue;
        }
        if (subelement.m_tag == "translate") {
            typeString = "translatable";
            translatable.m_translate = subelement.m_stringValue;
        }
        if (subelement.m_tag == "fallback") {
            typeString = "translatable";
            translatable.m_fallback = subelement.m_stringValue;
        }
        if (subelement.m_tag == "with") {
            typeString = "translatable";
            for (const NBTElement& text : subelement.m_childElements) {
                TextComponent textComponent;
                ByteBuffer buf;
                buf.writeNBTElement(text);
                textComponent.decode(buf);
                with.push_back(textComponent);
            }
        }
    }
    if (with.size()) translatable.m_with = with;

    if (typeString == "text") m_type = Type::Text;
    if (typeString == "translatable") {
        m_type = Type::Text;
        m_translatable = translatable;
    }
    if (typeString == "keybind") m_type = Type::Text;
    if (typeString == "selector") {
        m_type = Type::Text;
        selector.m_separator = separator;
        m_selector = selector;
    }
    if (typeString == "score") {
        m_type = Type::Text;
        m_score = score;
    }
    if (typeString == "nbt") {
        m_type = Type::Text;
        nbt.m_separator = separator;
        m_NBT = nbt;
    }
}
bool TextComponent::operator==(const TextComponent& text) const {
    return encodeJSON() == text.encodeJSON();
}
bool TextComponent::operator!=(const TextComponent& text) const {
    return !operator==(text);
}

TextComponent::Translatable& TextComponentBuilder::TranslatableBuilder::build() {
    return m_translatable;
}
TextComponent::Translatable TextComponentBuilder::TranslatableBuilder::build() const {
    return m_translatable;
}
TextComponentBuilder::TranslatableBuilder& TextComponentBuilder::TranslatableBuilder::translate(const std::string& translate) {
    m_translatable.m_translate = translate;
    return *this;
}
TextComponentBuilder::TranslatableBuilder& TextComponentBuilder::TranslatableBuilder::fallback(const std::string& fallback) {
    m_translatable.m_fallback = fallback;
    return *this;
}
TextComponentBuilder::TranslatableBuilder& TextComponentBuilder::TranslatableBuilder::with(const std::vector<TextComponent>& with) {
    m_translatable.m_with = with;
    return *this;
}

TextComponent::Score& TextComponentBuilder::ScoreBuilder::build() {
    return m_score;
}
TextComponent::Score TextComponentBuilder::ScoreBuilder::build() const {
    return m_score;
}
TextComponentBuilder::ScoreBuilder& TextComponentBuilder::ScoreBuilder::name(const std::string& name) {
    m_score.m_name = name;
    return *this;
}
TextComponentBuilder::ScoreBuilder& TextComponentBuilder::ScoreBuilder::objective(const std::string& objective) {
    m_score.m_objective = objective;
    return *this;
}

TextComponent::Selector& TextComponentBuilder::SelectorBuilder::build() {
    return m_selector;
}
TextComponent::Selector TextComponentBuilder::SelectorBuilder::build() const {
    return m_selector;
}
TextComponentBuilder::SelectorBuilder& TextComponentBuilder::SelectorBuilder::selector(const std::string& selector) {
    m_selector.m_selector = selector;
    return *this;
}
TextComponentBuilder::SelectorBuilder& TextComponentBuilder::SelectorBuilder::separator(const TextComponent& separator) {
    m_selector.m_separator = separator.encode();
    return *this;
}

TextComponent::NBT& TextComponentBuilder::NBTBuilder::build() {
    return m_NBT;
}
TextComponent::NBT TextComponentBuilder::NBTBuilder::build() const {
    return m_NBT;
}
TextComponentBuilder::NBTBuilder& TextComponentBuilder::NBTBuilder::nbt(const std::string& nbt) {
    m_NBT.m_nbt = nbt;
    return *this;
}
TextComponentBuilder::NBTBuilder& TextComponentBuilder::NBTBuilder::block(const std::string& block) {
    m_NBT.m_block = block;
    m_NBT.m_source = TextComponent::NBT::NBTType::Block;
    return *this;
}
TextComponentBuilder::NBTBuilder& TextComponentBuilder::NBTBuilder::entity(const std::string& entity) {
    m_NBT.m_entity = entity;
    m_NBT.m_source = TextComponent::NBT::NBTType::Entity;
    return *this;
}
TextComponentBuilder::NBTBuilder& TextComponentBuilder::NBTBuilder::storage(const std::string& storage) {
    m_NBT.m_storage = storage;
    m_NBT.m_source = TextComponent::NBT::NBTType::Storage;
    return *this;
}
TextComponentBuilder::NBTBuilder& TextComponentBuilder::NBTBuilder::separator(const TextComponent& separator) {
    m_NBT.m_separator = separator.encode();
    return *this;
}
TextComponentBuilder::NBTBuilder& TextComponentBuilder::NBTBuilder::interpret(const bool& state) {
    m_NBT.m_interpret = state;
    return *this;
}

TextComponent::ClickEvent& TextComponentBuilder::ClickEventBuilder::build() {
    return m_clickEvent;
}
TextComponent::ClickEvent TextComponentBuilder::ClickEventBuilder::build() const {
    return m_clickEvent;
}
TextComponentBuilder::ClickEventBuilder& TextComponentBuilder::ClickEventBuilder::openUrl(const std::string& url) {
    m_clickEvent.m_action = TextComponent::ClickEvent::Action::OpenURL;
    m_clickEvent.m_url = url;
    return *this;
}
TextComponentBuilder::ClickEventBuilder& TextComponentBuilder::ClickEventBuilder::openFile(const std::string& path) {
    m_clickEvent.m_action = TextComponent::ClickEvent::Action::OpenFile;
    m_clickEvent.m_path = path;
    return *this;
}
TextComponentBuilder::ClickEventBuilder& TextComponentBuilder::ClickEventBuilder::runCommand(const std::string& command) {
    m_clickEvent.m_action = TextComponent::ClickEvent::Action::RunCommand;
    m_clickEvent.m_command = command;
    return *this;
}
TextComponentBuilder::ClickEventBuilder& TextComponentBuilder::ClickEventBuilder::suggestCommand(const std::string& command) {
    m_clickEvent.m_action = TextComponent::ClickEvent::Action::SuggestCommand;
    m_clickEvent.m_command = command;
    return *this;
}
TextComponentBuilder::ClickEventBuilder& TextComponentBuilder::ClickEventBuilder::changePage(const int& page) {
    m_clickEvent.m_action = TextComponent::ClickEvent::Action::ChangePage;
    m_clickEvent.m_page = page;
    return *this;
}
TextComponentBuilder::ClickEventBuilder& TextComponentBuilder::ClickEventBuilder::copyToClipboard(const std::string& value) {
    m_clickEvent.m_action = TextComponent::ClickEvent::Action::CopyToClipboard;
    m_clickEvent.m_value = value;
    return *this;
}

TextComponent::HoverEvent::ShowItem& TextComponentBuilder::HoverEventBuilder::ShowItemBuilder::build() {
    return m_showItem;
}
TextComponent::HoverEvent::ShowItem TextComponentBuilder::HoverEventBuilder::ShowItemBuilder::build() const {
    return m_showItem;
}
TextComponentBuilder::HoverEventBuilder::ShowItemBuilder& TextComponentBuilder::HoverEventBuilder::ShowItemBuilder::id(const std::string& id) {
    m_showItem.m_id = id;
    return *this;
}
TextComponentBuilder::HoverEventBuilder::ShowItemBuilder& TextComponentBuilder::HoverEventBuilder::ShowItemBuilder::count(const int& count) {
    m_showItem.m_count = count;
    return *this;
}
TextComponentBuilder::HoverEventBuilder::ShowItemBuilder& 
    TextComponentBuilder::HoverEventBuilder::ShowItemBuilder::components(const std::vector<NBTElement>& components) {
    m_showItem.m_components = components;
    return *this;
}

TextComponent::HoverEvent::ShowEntity& TextComponentBuilder::HoverEventBuilder::ShowEntityBuilder::build() {
    return m_showEntity;
}
TextComponent::HoverEvent::ShowEntity TextComponentBuilder::HoverEventBuilder::ShowEntityBuilder::build() const {
    return m_showEntity;
}
TextComponentBuilder::HoverEventBuilder::ShowEntityBuilder& TextComponentBuilder::HoverEventBuilder::ShowEntityBuilder::name(const TextComponent& name) {
    m_showEntity.m_name = {name};
    return *this;
}
TextComponentBuilder::HoverEventBuilder::ShowEntityBuilder& TextComponentBuilder::HoverEventBuilder::ShowEntityBuilder::id(const std::string& id) {
    m_showEntity.m_id = id;
    return *this;
}
TextComponentBuilder::HoverEventBuilder::ShowEntityBuilder& TextComponentBuilder::HoverEventBuilder::ShowEntityBuilder::uuid(const uuids::uuid& uuid) {
    m_showEntity.m_UUID = uuid;
    return *this;
}

TextComponent::HoverEvent& TextComponentBuilder::HoverEventBuilder::build() {
    return m_hoverEvent;
}
TextComponent::HoverEvent TextComponentBuilder::HoverEventBuilder::build() const {
    return m_hoverEvent;
}
TextComponentBuilder::HoverEventBuilder& TextComponentBuilder::HoverEventBuilder::showText(const TextComponent& text) {
    m_hoverEvent.m_action = TextComponent::HoverEvent::Action::ShowText;
    m_hoverEvent.m_value = {text};
    return *this;
}
TextComponentBuilder::HoverEventBuilder& TextComponentBuilder::HoverEventBuilder::showItem(const TextComponent::HoverEvent::ShowItem& item) {
    m_hoverEvent.m_action = TextComponent::HoverEvent::Action::ShowItem;
    m_hoverEvent.m_showItem = item;
    return *this;
}
TextComponentBuilder::HoverEventBuilder& TextComponentBuilder::HoverEventBuilder::showEntity(const TextComponent::HoverEvent::ShowEntity& entity) {
    m_hoverEvent.m_action = TextComponent::HoverEvent::Action::ShowEntity;
    m_hoverEvent.m_showEntity = entity;
    return *this;
}

TextComponent& TextComponentBuilder::build() {
    return m_text;
}
TextComponent TextComponentBuilder::build() const {
    return m_text;
}
TextComponentBuilder& TextComponentBuilder::text(const std::string& text) {
    m_text.m_text = text;
    m_text.m_type = TextComponent::Type::Text;
    return *this;
}
TextComponentBuilder& TextComponentBuilder::translatable(const TextComponent::Translatable& translatable) {
    m_text.m_translatable = translatable;
    m_text.m_type = TextComponent::Type::Translatable;
    return *this;
}
TextComponentBuilder& TextComponentBuilder::score(const TextComponent::Score& score) {
    m_text.m_score = score;
    m_text.m_type = TextComponent::Type::Score;
    return *this;
}
TextComponentBuilder& TextComponentBuilder::selector(const TextComponent::Selector& selector) {
    m_text.m_selector = selector;
    m_text.m_type = TextComponent::Type::Selector;
    return *this;
}
TextComponentBuilder& TextComponentBuilder::keybind(const std::string& keybind) {
    m_text.m_keybind = keybind;
    m_text.m_type = TextComponent::Type::Keybind;
    return *this;
}
TextComponentBuilder& TextComponentBuilder::nbt(const TextComponent::NBT& nbt) {
    m_text.m_NBT = nbt;
    m_text.m_type = TextComponent::Type::NBT;
    return *this;
}
TextComponentBuilder& TextComponentBuilder::bold(const bool& state) {
    m_text.m_bold = state;
    return *this;
}
TextComponentBuilder& TextComponentBuilder::italic(const bool& state) {
    m_text.m_italic = state;
    return *this;
}
TextComponentBuilder& TextComponentBuilder::underlined(const bool& state) {
    m_text.m_underlined = state;
    return *this;
}
TextComponentBuilder& TextComponentBuilder::strikethrough(const bool& state) {
    m_text.m_strikethrough = state;
    return *this;
}
TextComponentBuilder& TextComponentBuilder::obfuscated(const bool& state) {
    m_text.m_obfuscated = state;
    return *this;
}
TextComponentBuilder& TextComponentBuilder::font(const Identifier& font) {
    m_text.m_font = font.toString();
    return *this;
}
TextComponentBuilder& TextComponentBuilder::color(const std::string& color) {
    m_text.m_color = color;
    return *this;
}
TextComponentBuilder& TextComponentBuilder::append(const TextComponent& text) {
    m_text.m_extra.push_back(text);
    return *this;
}
TextComponentBuilder& TextComponentBuilder::insertion(const std::string& insertion) {
    m_text.m_insertion = insertion;
    return *this;
}
TextComponentBuilder& TextComponentBuilder::clickEvent(const TextComponent::ClickEvent& clickEvent) {
    m_text.m_clickEvent = clickEvent;
    return *this;
}
TextComponentBuilder& TextComponentBuilder::hoverEvent(const TextComponent::HoverEvent& hoverEvent) {
    m_text.m_hoverEvent = hoverEvent;
    return *this;
}

}