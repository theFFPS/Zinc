#pragma once

#include <vector>
#include <string>
#include <external/JSON.h>
#include "NBTSettings.h"
#include "NBTElementType.h"

namespace zinc {

struct ByteBuffer;

struct NBTElement {
    NBTSettings m_settings;
    std::string m_tag;
    NBTElementType m_type;

    char m_byteValue;
    short m_shortValue;
    int m_intValue;
    long m_longValue;
    float m_floatValue;
    double m_doubleValue;

    std::vector<char> m_byteArrayValue;
    std::string m_stringValue;
    std::vector<NBTElement> m_childElements;
    std::vector<int> m_intArrayValue;
    std::vector<long> m_longArrayValue;
    
    NBTElement() 
        : m_settings(NBTSettings()), m_type(NBTElementType::End), m_byteValue(0), m_shortValue(0), m_intValue(0), m_longValue(0), 
            m_floatValue(0), m_doubleValue(0) {}
    NBTElement(ByteBuffer& byteBuffer) 
        : m_settings(NBTSettings()), m_type(NBTElementType::End), m_byteValue(0), m_shortValue(0), m_intValue(0), m_longValue(0), 
            m_floatValue(0), m_doubleValue(0) {
        decode(byteBuffer);
    }
    NBTElement(const NBTSettings& settings) 
        : m_settings(settings), m_type(NBTElementType::End), m_byteValue(0), m_shortValue(0), m_intValue(0), m_longValue(0), 
            m_floatValue(0), m_doubleValue(0) {}
    NBTElement(ByteBuffer& byteBuffer, const NBTSettings& settings) 
        : m_settings(settings), m_type(NBTElementType::End), m_byteValue(0), m_shortValue(0), m_intValue(0), m_longValue(0), 
            m_floatValue(0), m_doubleValue(0) {
        decode(byteBuffer);
    }

    std::vector<char> encode() const;
    void encode(ByteBuffer& byteBuffer) const;
    void decode(ByteBuffer& byteBuffer);

    std::string toJSON() const;

    bool contains(const std::string& tag) const;
    bool contains(size_t pos) const;
    NBTElement& at(const std::string& tag);
    const NBTElement& at(const std::string& tag) const;
    NBTElement& at(size_t pos);
    const NBTElement& at(size_t pos) const;

    NBTElement& operator[](const std::string& tag);
    const NBTElement& operator[](const std::string& tag) const;
    NBTElement& operator[](size_t pos);
    const NBTElement& operator[](size_t pos) const;

    bool operator==(const NBTElement& element) const;
    bool operator!=(const NBTElement& element) const;

    static NBTElement Byte(const char& byte, const NBTSettings& settings = NBTSettings());
    static NBTElement Short(const short& shortValue, const NBTSettings& settings = NBTSettings());
    static NBTElement Int(const int& intValue, const NBTSettings& settings = NBTSettings());
    static NBTElement Long(const long& longValue, const NBTSettings& settings = NBTSettings());
    static NBTElement Float(const float& floatValue, const NBTSettings& settings = NBTSettings());
    static NBTElement Double(const double& doubleValue, const NBTSettings& settings = NBTSettings());
    static NBTElement ByteArray(const std::vector<char>& byteArray, const NBTSettings& settings = NBTSettings());
    static NBTElement String(const std::string& string, const NBTSettings& settings = NBTSettings());
    static NBTElement IntArray(const std::vector<int>& intArray, const NBTSettings& settings = NBTSettings());
    static NBTElement LongArray(const std::vector<long>& longArray, const NBTSettings& settings = NBTSettings());
    static NBTElement List(const std::vector<NBTElement>& children, const NBTSettings& settings = NBTSettings());
    static NBTElement Compound(const std::vector<NBTElement>& children, const NBTSettings& settings = NBTSettings());

    static NBTElement Byte(const std::string& tag, const char& byte, const NBTSettings& settings = NBTSettings());
    static NBTElement Short(const std::string& tag, const short& shortValue, const NBTSettings& settings = NBTSettings());
    static NBTElement Int(const std::string& tag, const int& intValue, const NBTSettings& settings = NBTSettings());
    static NBTElement Long(const std::string& tag, const long& longValue, const NBTSettings& settings = NBTSettings());
    static NBTElement Float(const std::string& tag, const float& floatValue, const NBTSettings& settings = NBTSettings());
    static NBTElement Double(const std::string& tag, const double& doubleValue, const NBTSettings& settings = NBTSettings());
    static NBTElement ByteArray(const std::string& tag, const std::vector<char>& byteArray, const NBTSettings& settings = NBTSettings());
    static NBTElement String(const std::string& tag, const std::string& string, const NBTSettings& settings = NBTSettings());
    static NBTElement IntArray(const std::string& tag, const std::vector<int>& intArray, const NBTSettings& settings = NBTSettings());
    static NBTElement LongArray(const std::string& tag, const std::vector<long>& longArray, const NBTSettings& settings = NBTSettings());
    static NBTElement List(const std::string& tag, const std::vector<NBTElement>& children, const NBTSettings& settings = NBTSettings());
    static NBTElement Compound(const std::string& tag, const std::vector<NBTElement>& children, const NBTSettings& settings = NBTSettings());
};

}
