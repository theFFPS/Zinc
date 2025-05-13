#include <type/TextComponent.h>
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

}