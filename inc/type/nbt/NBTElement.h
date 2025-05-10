#pragma once

#include <map>
#include <vector>
#include <string>
#include "NBTSettings.h"
#include "NBTElementType.h"

namespace zinc {

struct ByteBuffer;

struct NBTElement {
private:
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
public:
    NBTElement() 
        : m_type(NBTElementType::End), m_byteValue(0), m_shortValue(0), m_intValue(0), m_longValue(0), m_floatValue(0), m_doubleValue(0), m_settings(NBTSettings()) {}
    NBTElement(ByteBuffer& byteBuffer) 
        : m_type(NBTElementType::End), m_byteValue(0), m_shortValue(0), m_intValue(0), m_longValue(0), m_floatValue(0), m_doubleValue(0), m_settings(NBTSettings()) {
        decode(byteBuffer);
    }
    NBTElement(const NBTSettings& settings) 
        : m_type(NBTElementType::End), m_byteValue(0), m_shortValue(0), m_intValue(0), m_longValue(0), m_floatValue(0), m_doubleValue(0), m_settings(settings) {}
    NBTElement(ByteBuffer& byteBuffer, const NBTSettings& settings) 
        : m_type(NBTElementType::End), m_byteValue(0), m_shortValue(0), m_intValue(0), m_longValue(0), m_floatValue(0), m_doubleValue(0), m_settings(settings) {
        decode(byteBuffer);
    }

    std::vector<char> encode() const;
    void encode(ByteBuffer& byteBuffer) const;
    void decode(ByteBuffer& byteBuffer);

    NBTSettings& getSettings();
    NBTSettings getSettings() const;
    NBTElementType& getType();
    NBTElementType getType() const;
    std::string& getTag();
    std::string getTag() const;

    void setSettings(const NBTSettings& settings);
    void setType(const NBTElementType& type);
    void setTag(const std::string& tag);

    char& getByte();
    char getByte() const;
    short& getShort();
    short getShort() const;
    int& getInt();
    int getInt() const;
    long& getLong();
    long getLong() const;
    float& getFloat();
    float getFloat() const;
    double& getDouble();
    double getDouble() const;
    std::vector<char>& getByteArray();
    std::vector<char> getByteArray() const;
    std::string& getString();
    std::string getString() const;
    std::vector<NBTElement>& getChildElements();
    std::vector<NBTElement> getChildElements() const;
    std::vector<int>& getIntArray();
    std::vector<int> getIntArray() const;
    std::vector<long>& getLongArray();
    std::vector<long> getLongArray() const;

    void setByte(const char& byteValue);
    void setShort(const short& shortValue);
    void setInt(const int& intValue);
    void setLong(const long& longValue);
    void setFloat(const float& floatValue);
    void setDouble(const double& doubleValue);
    void setByteArray(const std::vector<char>& byteArrayValue);
    void setString(const std::string& stringValue);
    void setChildElements(const std::vector<NBTElement>& childElements);
    void setIntArray(const std::vector<int>& intArrayValue);
    void setLongArray(const std::vector<long>& longArrayValue);

    bool operator==(const NBTElement& element) const;
    bool operator!=(const NBTElement& element) const;

    static NBTElement Byte(const char& byte);
    static NBTElement Short(const short& shortValue);
    static NBTElement Int(const int& intValue);
    static NBTElement Long(const long& longValue);
    static NBTElement Float(const float& floatValue);
    static NBTElement Double(const double& doubleValue);
    static NBTElement ByteArray(const std::vector<char>& byteArray);
    static NBTElement String(const std::string& string);
    static NBTElement IntArray(const std::vector<int>& intArray);
    static NBTElement LongArray(const std::vector<long>& longArray);
    static NBTElement List(const std::vector<NBTElement>& children);
    static NBTElement Compound(const std::vector<NBTElement>& children);

    static NBTElement Byte(const std::string& tag, const char& byte);
    static NBTElement Short(const std::string& tag, const short& shortValue);
    static NBTElement Int(const std::string& tag, const int& intValue);
    static NBTElement Long(const std::string& tag, const long& longValue);
    static NBTElement Float(const std::string& tag, const float& floatValue);
    static NBTElement Double(const std::string& tag, const double& doubleValue);
    static NBTElement ByteArray(const std::string& tag, const std::vector<char>& byteArray);
    static NBTElement String(const std::string& tag, const std::string& string);
    static NBTElement IntArray(const std::string& tag, const std::vector<int>& intArray);
    static NBTElement LongArray(const std::string& tag, const std::vector<long>& longArray);
    static NBTElement List(const std::string& tag, const std::vector<NBTElement>& children);
    static NBTElement Compound(const std::string& tag, const std::vector<NBTElement>& children);

    static NBTElement Byte(const char& byte, const NBTSettings& settings);
    static NBTElement Short(const short& shortValue, const NBTSettings& settings);
    static NBTElement Int(const int& intValue, const NBTSettings& settings);
    static NBTElement Long(const long& longValue, const NBTSettings& settings);
    static NBTElement Float(const float& floatValue, const NBTSettings& settings);
    static NBTElement Double(const double& doubleValue, const NBTSettings& settings);
    static NBTElement ByteArray(const std::vector<char>& byteArray, const NBTSettings& settings);
    static NBTElement String(const std::string& string, const NBTSettings& settings);
    static NBTElement IntArray(const std::vector<int>& intArray, const NBTSettings& settings);
    static NBTElement LongArray(const std::vector<long>& longArray, const NBTSettings& settings);
    static NBTElement List(const std::vector<NBTElement>& children, const NBTSettings& settings);
    static NBTElement Compound(const std::vector<NBTElement>& children, const NBTSettings& settings);

    static NBTElement Byte(const std::string& tag, const char& byte, const NBTSettings& settings);
    static NBTElement Short(const std::string& tag, const short& shortValue, const NBTSettings& settings);
    static NBTElement Int(const std::string& tag, const int& intValue, const NBTSettings& settings);
    static NBTElement Long(const std::string& tag, const long& longValue, const NBTSettings& settings);
    static NBTElement Float(const std::string& tag, const float& floatValue, const NBTSettings& settings);
    static NBTElement Double(const std::string& tag, const double& doubleValue, const NBTSettings& settings);
    static NBTElement ByteArray(const std::string& tag, const std::vector<char>& byteArray, const NBTSettings& settings);
    static NBTElement String(const std::string& tag, const std::string& string, const NBTSettings& settings);
    static NBTElement IntArray(const std::string& tag, const std::vector<int>& intArray, const NBTSettings& settings);
    static NBTElement LongArray(const std::string& tag, const std::vector<long>& longArray, const NBTSettings& settings);
    static NBTElement List(const std::string& tag, const std::vector<NBTElement>& children, const NBTSettings& settings);
    static NBTElement Compound(const std::string& tag, const std::vector<NBTElement>& children, const NBTSettings& settings);
};

}
