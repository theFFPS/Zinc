#pragma once

#include <vector>
#include <string>

namespace zinc {

enum class NBTTagType : char {
    End,
    Byte, Short, Int, Long, Float, Double,
    ByteArray, String, List, Compound, IntArray, LongArray
};
struct NBTTag {
    NBTTagType m_type = NBTTagType::End;
    char m_byteValue;
    short m_shortValue;
    int m_intValue;
    long m_longValue;
    float m_floatValue;
    double m_doubleValue;
    std::vector<char> m_byteArrayValue;
    std::string m_stringValue;
    std::vector<NBTTag> m_childrenTagsValue;
    std::vector<int> m_intArrayValue;
    std::vector<long> m_longArrayValue;
    std::string m_tagName;

    NBTTag();
    NBTTag(struct ByteBuffer *buffer, NBTTagType expectedType = NBTTagType::Compound, bool expectTagName = true, 
                    bool isTypeKnown = false, bool isInList = false);

    std::vector<char> encode(bool ignoreNametag = false, bool isInList = false);

    bool operator==(NBTTag& b);
    bool operator!=(NBTTag& b);

    static NBTTag End();
    static NBTTag Byte(char value, std::string const& tagName);
    static NBTTag Short(short value, std::string const& tagName);
    static NBTTag Int(int value, std::string const& tagName);
    static NBTTag Long(long value, std::string const& tagName);
    static NBTTag Float(float value, std::string const& tagName);
    static NBTTag Double(double value, std::string const& tagName);
    static NBTTag ByteArray(std::vector<char> const& value, std::string const& tagName);
    static NBTTag String(std::string const& value, std::string const& tagName);
    static NBTTag IntArray(std::vector<int> const& value, std::string const& tagName);
    static NBTTag LongArray(std::vector<long> const& value, std::string const& tagName);
    static NBTTag List(std::vector<NBTTag> const& value, std::string const& tagName);
    static NBTTag Compound(std::vector<NBTTag> const& value, std::string const& tagName);
};

}