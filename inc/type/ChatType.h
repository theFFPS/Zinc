#pragma once

#include "nbt/NBTElement.h"

namespace zinc {

struct ChatTypeDecoration {
public:
    enum class Parameter : int { Sender, Target, Content };
private:
    std::string m_translationKey;
    std::vector<Parameter> m_parameters;
    NBTElement m_style;
public:
    ChatTypeDecoration() {}
    ChatTypeDecoration(const std::string& translationKey, const std::vector<Parameter>& parameters, const NBTElement& style)
        : m_translationKey(translationKey), m_parameters(parameters), m_style(style) {}
    
    std::string& getTranslationKey();
    std::string getTranslationKey() const;
    std::vector<Parameter>& getParameters();
    std::vector<Parameter> getParameters() const;
    NBTElement& getStyle();
    NBTElement getStyle() const;

    void setTranslationKey(const std::string& translationKey);
    void setParameters(const std::vector<Parameter>& parameters);
    void setStyle(const NBTElement& style);

    bool operator==(const ChatTypeDecoration& chatDecoration) const;
    bool operator!=(const ChatTypeDecoration& chatDecoration) const;
};
struct ChatType {
private:
    ChatTypeDecoration m_chat;
    ChatTypeDecoration m_narration;
public:
    ChatType() {}
    ChatType(const ChatTypeDecoration& chat, const ChatTypeDecoration& narration) : m_chat(chat), m_narration(narration) {}

    ChatTypeDecoration& getChat();
    ChatTypeDecoration getChat() const;
    ChatTypeDecoration& getNarration();
    ChatTypeDecoration getNarration() const;

    void setChat(const ChatTypeDecoration& chat);
    void setNarration(const ChatTypeDecoration& narration);

    bool operator==(const ChatType& chat) const;
    bool operator!=(const ChatType& chat) const;
};

}