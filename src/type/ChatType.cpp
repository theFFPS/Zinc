#include <type/ChatType.h>

namespace zinc {

std::string& ChatTypeDecoration::getTranslationKey() {
    return m_translationKey;
}
std::string ChatTypeDecoration::getTranslationKey() const {
    return m_translationKey;
}
std::vector<ChatTypeDecoration::Parameter>& ChatTypeDecoration::getParameters() {
    return m_parameters;
}
std::vector<ChatTypeDecoration::Parameter> ChatTypeDecoration::getParameters() const {
    return m_parameters;
}
NBTElement& ChatTypeDecoration::getStyle() {
    return m_style;
}
NBTElement ChatTypeDecoration::getStyle() const {
    return m_style;
}
void ChatTypeDecoration::setTranslationKey(const std::string& translationKey) {
    m_translationKey = translationKey;
}
void ChatTypeDecoration::setParameters(const std::vector<Parameter>& parameters) {
    m_parameters = parameters;
}
void ChatTypeDecoration::setStyle(const NBTElement& style) {
    m_style = style;
}
bool ChatTypeDecoration::operator==(const ChatTypeDecoration& chatDecoration) const {
    return m_translationKey == chatDecoration.getTranslationKey() && m_parameters == chatDecoration.getParameters() && m_style == chatDecoration.getStyle();
}
bool ChatTypeDecoration::operator!=(const ChatTypeDecoration& chatDecoration) const {
    return !operator==(chatDecoration);
}

ChatTypeDecoration& ChatType::getChat() {
    return m_chat;
}
ChatTypeDecoration ChatType::getChat() const {
    return m_chat;
}
ChatTypeDecoration& ChatType::getNarration() {
    return m_narration;
}
ChatTypeDecoration ChatType::getNarration() const {
    return m_narration;
}
void ChatType::setChat(const ChatTypeDecoration& chat) {
    m_chat = chat;
}
void ChatType::setNarration(const ChatTypeDecoration& narration) {
    m_narration = narration;
}
bool ChatType::operator==(const ChatType& chat) const {
    return m_narration == chat.getNarration() && m_chat == chat.getChat();
}
bool ChatType::operator!=(const ChatType& chat) const {
    return !operator==(chat);
}

}