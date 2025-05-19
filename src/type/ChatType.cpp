#include <type/ChatType.h>

namespace zinc {

bool ChatTypeDecoration::operator==(const ChatTypeDecoration& chatDecoration) const {
    return m_translationKey == chatDecoration.m_translationKey && m_parameters == chatDecoration.m_parameters && m_style == chatDecoration.m_style;
}
bool ChatTypeDecoration::operator!=(const ChatTypeDecoration& chatDecoration) const {
    return !operator==(chatDecoration);
}

bool ChatType::operator==(const ChatType& chat) const {
    return m_narration == chat.m_narration && m_chat == chat.m_chat;
}
bool ChatType::operator!=(const ChatType& chat) const {
    return !operator==(chat);
}

}