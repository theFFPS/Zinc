#pragma once

#include "nbt/NBTElement.h"

namespace zinc {

struct ChatTypeDecoration {
    enum class Parameter : int { Sender, Target, Content };
    std::string m_translationKey;
    std::vector<Parameter> m_parameters;
    NBTElement m_style;

    ChatTypeDecoration() {}
    ChatTypeDecoration(const std::string& translationKey, const std::vector<Parameter>& parameters, const NBTElement& style)
        : m_translationKey(translationKey), m_parameters(parameters), m_style(style) {}

    bool operator==(const ChatTypeDecoration& chatDecoration) const;
    bool operator!=(const ChatTypeDecoration& chatDecoration) const;
};
struct ChatType {
    ChatTypeDecoration m_chat;
    ChatTypeDecoration m_narration;
    
    ChatType() {}
    ChatType(const ChatTypeDecoration& chat, const ChatTypeDecoration& narration) : m_chat(chat), m_narration(narration) {}

    bool operator==(const ChatType& chat) const;
    bool operator!=(const ChatType& chat) const;
};

}