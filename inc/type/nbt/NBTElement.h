#pragma once

#include <map>
#include "../ByteBuffer.h"
#include "NBTSettings.h"
#include "NBTElementType.h"

namespace zinc {

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
    NBTElement() : m_type(NBTElementType::End), m_byteValue(0), m_shortValue(0), m_intValue(0), m_longValue(0), m_floatValue(0), m_doubleValue(0), m_settings(NBTSettings()) {}
    NBTElement(const std::vector<char>& bytes) 
        : m_type(NBTElementType::End), m_byteValue(0), m_shortValue(0), m_intValue(0), m_longValue(0), m_floatValue(0), m_doubleValue(0), m_settings(NBTSettings()) {
        ByteBuffer buffer (bytes);
        decode(buffer);
    }
    NBTElement(ByteBuffer& byteBuffer) 
        : m_type(NBTElementType::End), m_byteValue(0), m_shortValue(0), m_intValue(0), m_longValue(0), m_floatValue(0), m_doubleValue(0), m_settings(NBTSettings()) {
        decode(byteBuffer);
    }
    NBTElement(const NBTSettings& settings) 
        : m_type(NBTElementType::End), m_byteValue(0), m_shortValue(0), m_intValue(0), m_longValue(0), m_floatValue(0), m_doubleValue(0), m_settings(settings) {}
    NBTElement(const std::vector<char>& bytes, const NBTSettings& settings) 
        : m_type(NBTElementType::End), m_byteValue(0), m_shortValue(0), m_intValue(0), m_longValue(0), m_floatValue(0), m_doubleValue(0), m_settings(settings) {
        ByteBuffer buffer (bytes);
        decode(buffer);
    }
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
};

}