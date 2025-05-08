#pragma once

#include <string>
#include <vector>
#include "NBTSettings.h"

namespace zinc {

template<typename T> struct NBTTagType {
private:
    T m_value;
    int m_typeId;
    std::string m_typeName;

    std::vector<char>(*m_encodeFunction)(const std::string&, const T&, const NBTSettings&);
    T(*m_decodeFunction)(const std::vector<char>&, const NBTSettings&);

    static const std::vector<char>(*ENCODE_PLACEHOLDER)(const T&, const T&, const NBTSettings&) 
        = [](const std::string&, const T&, const NBTSettings&) {
            return {};
        };
    static const T(*DECODE_PLACEHOLDER)(const std::vector<char>&, const NBTSettings&) 
        = [](const std::vector<char>&, const NBTSettings&){
            return T{};
        };
public:
    NBTTagType() : m_typeId(0), m_typeName("end"), m_encodeFunction(ENCODE_PLACEHOLDER), m_decodeFunction(DECODE_PLACEHOLDER) {}
    NBTTagType(const T& value, const int& typeId, const std::string& typeName, 
                std::vector<char>(*encodeFunction)(const std::string&, const T&, const NBTSettings&),
                T(*decodeFunction)(const std::vector<char>&, const NBTSettings&))
        : m_value(value), m_typeId(typeId), m_typeName(typeName), m_encodeFunction(encodeFunction), m_decodeFunction(decodeFunction) {}

    T& getValue() {
        return m_value;
    }
    T getValue() const {
        return m_value;
    }
    int& getTypeId() {
        return m_typeId;
    }
    int getTypeId() const {
        return m_typeId;
    }
    std::string& getTypeName() {
        return m_typeName;
    }
    std::string getTypeName() const {
        return m_typeName;
    }

    void setValue(const T& value) {
        m_value = value;
    }
    void setType(const int& typeId, const std::string& typeName) {
        m_typeId = typeId;
        m_typeName = typeName;
    }

    std::vector<char> encode(const std::string& tagName, const T& value, const NBTSettings& settings) {
        return m_encodeFunction(tagName, value, settings);
    }
    T decode(const std::vector<char>& bytes, const NBTSettings& settings) {
        return m_decodeFunction(bytes, settings);
    }
};

}