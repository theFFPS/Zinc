#pragma once

#include "NBTElementType.h"

namespace zinc {

struct NBTSettings {
private:
    bool m_isInArray;
    bool m_isNetwork;
    int m_protocol;
    NBTElementType m_type;
public:
    NBTSettings() : m_isInArray(false), m_isNetwork(false), m_protocol(-1), m_type(NBTElementType::End) {}
    NBTSettings(const bool& isInArray) : m_isInArray(isInArray), m_isNetwork(false), m_protocol(-1), m_type(NBTElementType::End) {}
    NBTSettings(const bool& isNetwork, const int& protocol) : m_isInArray(false), m_isNetwork(isNetwork), m_protocol(protocol), m_type(NBTElementType::End) {}
    NBTSettings(const bool& isInArray, const bool& isNetwork, const int& protocol) 
        : m_isInArray(isInArray), m_isNetwork(isNetwork), m_protocol(protocol), m_type(NBTElementType::End) {}
    NBTSettings(const NBTElementType& type) : m_isInArray(false), m_isNetwork(false), m_protocol(-1), m_type(type) {}
    NBTSettings(const bool& isInArray, const NBTElementType& type) : m_isInArray(isInArray), m_isNetwork(false), m_protocol(-1), m_type(type) {}
    NBTSettings(const bool& isNetwork, const int& protocol, const NBTElementType& type) 
        : m_isInArray(false), m_isNetwork(isNetwork), m_protocol(protocol), m_type(type) {}
    NBTSettings(const bool& isInArray, const bool& isNetwork, const int& protocol, const NBTElementType& type) 
        : m_isInArray(isInArray), m_isNetwork(isNetwork), m_protocol(protocol), m_type(type) {}

    bool& getIsInArray();
    bool getIsInArray() const;
    bool& getIsNetwork();
    bool getIsNetwork() const;
    int& getProtocol();
    int getProtocol() const;
    NBTElementType& getType();
    NBTElementType getType() const;

    void setIsInArray(const bool& isInArray);
    void setIsNetwork(const bool& isNetwork);
    void setProtocol(const int& protocol);
    void setType(const NBTElementType& type);

    bool operator==(const NBTSettings& settings) const;
    bool operator!=(const NBTSettings& settings) const;
};

}