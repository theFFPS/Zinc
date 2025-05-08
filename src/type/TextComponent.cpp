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
std::string& TextComponent::Format::getFont() {
    return m_font;
}
std::string TextComponent::Format::getFont() const {
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



}