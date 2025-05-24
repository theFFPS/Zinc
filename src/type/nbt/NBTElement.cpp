#include <type/nbt/NBTElement.h>
#include <type/ByteBuffer.h>
#include <util/Logger.h>
#include <iostream>

namespace zinc {

std::vector<char> NBTElement::encode() const {
    ByteBuffer buffer;
    encode(buffer);
    return buffer.getBytes();
}
void NBTElement::encode(ByteBuffer& byteBuffer) const {
    NBTElementType type = m_type;
    if (m_settings.m_type != NBTElementType::End) type = m_settings.m_type;
    if (!m_settings.m_isInArray) byteBuffer.writeByte((char) type);
    if (type != NBTElementType::End && !m_settings.m_isInArray) {
        if (!byteBuffer.m_isBigEndian) {
            if (!m_settings.m_isNetwork) {
                byteBuffer.writeNumeric<unsigned short>(m_tag.size());
                byteBuffer.writeBytes(std::vector<char>(m_tag.begin(), m_tag.end()));
            } else byteBuffer.writeString(m_tag);
        } else {
            if (!(m_tag.empty() && m_settings.m_isNetwork)) {
                byteBuffer.writeNumeric<unsigned short>(m_tag.size());
                byteBuffer.writeBytes(std::vector<char>(m_tag.begin(), m_tag.end()));
            }
        }
    }
    switch (type) {
    case NBTElementType::Byte: {
        byteBuffer.writeByte(m_byteValue);
        break;
    }
    case NBTElementType::Short: {
        byteBuffer.writeNumeric<short>(m_shortValue);
        break;
    }
    case NBTElementType::Int: {
        if (byteBuffer.m_isBigEndian) byteBuffer.writeNumeric<int>(m_intValue);
        else {
            if (m_settings.m_isNetwork) byteBuffer.writeZigZagVarNumeric<int>(m_intValue);
            else byteBuffer.writeNumeric<int>(m_intValue);
        }
        break;
    }
    case NBTElementType::Long: {
        if (byteBuffer.m_isBigEndian) byteBuffer.writeNumeric<long>(m_longValue);
        else {
            if (m_settings.m_isNetwork) byteBuffer.writeZigZagVarNumeric<long>(m_longValue);
            else byteBuffer.writeNumeric<long>(m_longValue);
        }
        break;
    }
    case NBTElementType::Float: {
        byteBuffer.writeNumeric<float>(m_floatValue);
        break;
    }
    case NBTElementType::Double: {
        byteBuffer.writeNumeric<double>(m_doubleValue);
        break;
    }
    case NBTElementType::ByteArray: {
        if (byteBuffer.m_isBigEndian) byteBuffer.writeNumeric<unsigned int>(m_byteArrayValue.size());
        else {
            if (m_settings.m_isNetwork) byteBuffer.writeZigZagVarNumeric<int>(m_byteArrayValue.size());
            else byteBuffer.writeNumeric<unsigned int>(m_byteArrayValue.size());
        }
        byteBuffer.writeByteArray(m_byteArrayValue);
        break;
    }
    case NBTElementType::IntArray: {
        if (byteBuffer.m_isBigEndian) byteBuffer.writeNumeric<unsigned int>(m_intArrayValue.size());
        else {
            if (m_settings.m_isNetwork) byteBuffer.writeZigZagVarNumeric<int>(m_intArrayValue.size());
            else byteBuffer.writeNumeric<unsigned int>(m_intArrayValue.size());
        }
        byteBuffer.writeArray<int>(m_intArrayValue, &ByteBuffer::writeNumeric<int>);
        break;
    }
    case NBTElementType::LongArray: {
        if (byteBuffer.m_isBigEndian) byteBuffer.writeNumeric<unsigned int>(m_longArrayValue.size());
        else {
            if (m_settings.m_isNetwork) byteBuffer.writeZigZagVarNumeric<int>(m_longArrayValue.size());
            else byteBuffer.writeNumeric<unsigned int>(m_longArrayValue.size());
        }
        byteBuffer.writeArray<long>(m_longArrayValue, &ByteBuffer::writeNumeric<long>);
        break;
    }
    case NBTElementType::List: {
        if (m_childElements.empty()) {
            byteBuffer.writeByte(0);
            if (byteBuffer.m_isBigEndian) byteBuffer.writeNumeric<unsigned int>(0);
            else {
                if (m_settings.m_isNetwork) byteBuffer.writeZigZagVarNumeric<int>(0);
                else byteBuffer.writeNumeric<unsigned int>(0);
            }
        } else {
            NBTElementType elementType = m_childElements[0].m_type;
            for (const NBTElement& element : m_childElements) if (elementType != element.m_type) {
                Logger("NBTEncode").error("All NBTList elements must have the same type");
                byteBuffer.writeByte(0);
                if (byteBuffer.m_isBigEndian) byteBuffer.writeNumeric<unsigned int>(0);
                else {
                    if (m_settings.m_isNetwork) byteBuffer.writeZigZagVarNumeric<int>(0);
                    else byteBuffer.writeNumeric<unsigned int>(0);
                }
                return;
            }
            NBTSettings settings = m_settings;
            settings.m_type = elementType;
            settings.m_isInArray = true;
            settings.m_isNetwork = false;
            byteBuffer.writeByte((char) elementType);
            if (byteBuffer.m_isBigEndian) byteBuffer.writeNumeric<unsigned int>(m_childElements.size());
            else {
                if (m_settings.m_isNetwork) byteBuffer.writeZigZagVarNumeric<int>(m_childElements.size());
                else byteBuffer.writeNumeric<unsigned int>(m_childElements.size());
            }
            for (const NBTElement& element : m_childElements) {
                NBTElement arrayElement = element;
                arrayElement.m_settings = settings;
                arrayElement.encode(byteBuffer);
            }
        }
        break;
    }
    case NBTElementType::String: {
        if (byteBuffer.m_isBigEndian) {
            byteBuffer.writeNumeric<unsigned short>(m_stringValue.size());
            byteBuffer.writeBytes(std::vector<char>(m_stringValue.begin(), m_stringValue.end()));
        } else {
            if (!m_settings.m_isNetwork) {
                byteBuffer.writeNumeric<unsigned short>(m_stringValue.size());
                byteBuffer.writeBytes(std::vector<char>(m_stringValue.begin(), m_stringValue.end()));
            } else byteBuffer.writeString(m_stringValue);
        }
        break;
    }
    case NBTElementType::Compound: {
        NBTSettings settings;
        settings.m_type = NBTElementType::End;
        settings.m_isInArray = false;
        settings.m_isNetwork = false;
        for (const NBTElement& element : m_childElements) {
            NBTElement childElement = element;
            childElement.m_settings = settings;
            childElement.encode(byteBuffer);
        }
        byteBuffer.writeByte(0);
        break;
    }
    default: break;
    }
}
void NBTElement::decode(ByteBuffer& byteBuffer) {
    if (m_settings.m_type != NBTElementType::End) m_type = m_settings.m_type;
    if (!m_settings.m_isInArray) m_type = (NBTElementType) byteBuffer.readByte();
    if (m_type != NBTElementType::End && !m_settings.m_isInArray) {
        if (!byteBuffer.m_isBigEndian) {
            if (!m_settings.m_isNetwork) {
                std::vector<char> bytes = byteBuffer.readBytes(byteBuffer.readNumeric<unsigned short>());
                m_tag = std::string(bytes.begin(), bytes.end());
            } else m_tag = byteBuffer.readString();
        } else {
            if (!m_settings.m_isNetwork) {
                std::vector<char> bytes = byteBuffer.readBytes(byteBuffer.readNumeric<unsigned short>());
                m_tag = std::string(bytes.begin(), bytes.end());
            }
        }
    }
    switch (m_type) {
    case NBTElementType::Byte: {
        m_byteValue = byteBuffer.readByte();
        break;
    }
    case NBTElementType::Short: {
        m_shortValue = byteBuffer.readNumeric<short>();
        break;
    }
    case NBTElementType::Int: {
        if (byteBuffer.m_isBigEndian) m_intValue = byteBuffer.readNumeric<int>();
        else {
            if (m_settings.m_isNetwork) m_intValue = byteBuffer.readZigZagVarNumeric<int>();
            else m_intValue = byteBuffer.readNumeric<int>();
        }
        break;
    }
    case NBTElementType::Long: {
        if (byteBuffer.m_isBigEndian) m_longValue = byteBuffer.readNumeric<long>();
        else {
            if (m_settings.m_isNetwork) m_longValue = byteBuffer.readZigZagVarNumeric<long>();
            else m_longValue = byteBuffer.readNumeric<long>();
        }
        break;
    }
    case NBTElementType::Float: {
        m_floatValue = byteBuffer.readNumeric<float>();
        break;
    }
    case NBTElementType::Double: {
        m_doubleValue = byteBuffer.readNumeric<double>();
        break;
    }
    case NBTElementType::ByteArray: {
        unsigned length = 0;
        if (byteBuffer.m_isBigEndian) length = byteBuffer.readNumeric<unsigned int>();
        else {
            if (m_settings.m_isNetwork) length = byteBuffer.readZigZagVarNumeric<int>();
            else length = byteBuffer.readNumeric<unsigned int>();
        }
        m_byteArrayValue = byteBuffer.readByteArray(length);
        break;
    }
    case NBTElementType::IntArray: {
        unsigned length = 0;
        if (byteBuffer.m_isBigEndian) length = byteBuffer.readNumeric<unsigned int>();
        else {
            if (m_settings.m_isNetwork) length = byteBuffer.readZigZagVarNumeric<int>();
            else length = byteBuffer.readNumeric<unsigned int>();
        }
        m_intArrayValue = byteBuffer.readArray<int>(&ByteBuffer::readNumeric<int>, length);
        break;
    }
    case NBTElementType::LongArray: {
        unsigned length = 0;
        if (byteBuffer.m_isBigEndian) length = byteBuffer.readNumeric<unsigned int>();
        else {
            if (m_settings.m_isNetwork) length = byteBuffer.readZigZagVarNumeric<int>();
            else length = byteBuffer.readNumeric<unsigned int>();
        }
        m_longArrayValue = byteBuffer.readArray<long>(&ByteBuffer::readNumeric<long>, length);
        break;
    }
    case NBTElementType::String: {
        unsigned short length = 0;
        if (byteBuffer.m_isBigEndian) {
            length = byteBuffer.readNumeric<unsigned short>();
            std::vector<char> bytes = byteBuffer.readBytes(length);
            m_stringValue = std::string(bytes.begin(), bytes.end());
        } else {
            if (!m_settings.m_isNetwork) {
                length = byteBuffer.readNumeric<unsigned short>();
                std::vector<char> bytes = byteBuffer.readBytes(length);
                m_stringValue = std::string(bytes.begin(), bytes.end());
            } else m_stringValue = byteBuffer.readString();
        }
        break;        
    }
    case NBTElementType::List: {
        NBTElementType type = (NBTElementType) byteBuffer.readByte();
        NBTSettings settings = m_settings;
        unsigned length = 0;
        if (byteBuffer.m_isBigEndian) length = byteBuffer.readNumeric<unsigned int>();
        else {
            if (m_settings.m_isNetwork) length = byteBuffer.readZigZagVarNumeric<int>();
            else length = byteBuffer.readNumeric<unsigned int>();
        }
        if (type == NBTElementType::End) length = 0;
        settings.m_type = type;
        settings.m_isInArray = true;
        settings.m_isNetwork = !byteBuffer.m_isBigEndian && settings.m_isNetwork;
        for (unsigned i = 0; i < length; i++) {
            NBTElement arrayElement;
            arrayElement.m_settings = settings;
            arrayElement.decode(byteBuffer);
            m_childElements.push_back(arrayElement);
        }
        break;        
    }
    case NBTElementType::Compound: {
        NBTSettings settings = m_settings;
        settings.m_isInArray = false;
        settings.m_isNetwork = !byteBuffer.m_isBigEndian && settings.m_isNetwork;
        settings.m_type = NBTElementType::End;
        while (byteBuffer.getReaderPointer() < byteBuffer.size()) {
            if (!byteBuffer.getBytes()[byteBuffer.getReaderPointer()]) {
                byteBuffer.readByte();
                break;
            }
            NBTElement childElement;
            childElement.m_settings = settings;
            childElement.decode(byteBuffer);
            m_childElements.push_back(childElement);
        }
        break;
    }
    default: break;
    }
}
bool NBTElement::operator==(const NBTElement& element) const {
    if (element.m_type != m_type) return false;
    switch (m_type) {
    case NBTElementType::Byte: return m_byteValue == element.m_byteValue;
    case NBTElementType::Short: return m_shortValue == element.m_shortValue;
    case NBTElementType::Int: return m_intValue == element.m_intValue;
    case NBTElementType::Long: return m_longValue == element.m_longValue;
    case NBTElementType::Float: return m_floatValue == element.m_floatValue;
    case NBTElementType::Double: return m_doubleValue == element.m_doubleValue;
    case NBTElementType::ByteArray: return m_byteArrayValue == element.m_byteArrayValue;
    case NBTElementType::String: return m_stringValue == element.m_stringValue;
    case NBTElementType::List: 
    case NBTElementType::Compound: return m_childElements == element.m_childElements;
    case NBTElementType::IntArray: return m_intArrayValue == element.m_intArrayValue;
    case NBTElementType::LongArray: return m_longArrayValue == element.m_longArrayValue;
    default: return true;
    }
}
bool NBTElement::operator!=(const NBTElement& element) const {
    return !operator==(element);
}

std::string NBTElement::toJSON() const {
    switch (m_type) {
    case NBTElementType::End: return "";
    case NBTElementType::Byte: {
        if (m_byteValue == 1) return "true";
        else if (!m_byteValue) return "false";
        return std::to_string(m_byteValue);
    }
    case NBTElementType::Short: return std::to_string(m_shortValue);
    case NBTElementType::Int: return std::to_string(m_intValue);
    case NBTElementType::Long: return std::to_string(m_longValue);
    case NBTElementType::Float: return std::to_string(m_floatValue);
    case NBTElementType::Double: return std::to_string(m_doubleValue);
    case NBTElementType::ByteArray: {
        std::string result = "[";
        for (const char& byteValue : m_byteArrayValue) result += std::to_string(byteValue) + ",";
        if (m_byteArrayValue.size()) result.pop_back();
        return result + "]";
    }
    case NBTElementType::String: return "\"" + m_stringValue + "\"";
    case NBTElementType::List: {
        std::string result = "[";
        for (const NBTElement& element : m_childElements) result += element.toJSON() + ",";
        if (m_childElements.size()) result.pop_back();
        return result + "]";
    }
    case NBTElementType::Compound: {
        std::string result = "{";
        for (const NBTElement& element : m_childElements) result += "\"" + element.m_tag + "\":" + element.toJSON() + ",";
        if (m_childElements.size()) result.pop_back();
        return result + "}";
    }
    case NBTElementType::IntArray: {
        std::string result = "[";
        for (const char& intValue : m_intArrayValue) result += std::to_string(intValue) + ",";
        if (m_intArrayValue.size()) result.pop_back();
        return result + "]";
    }
    case NBTElementType::LongArray: {
        std::string result = "[";
        for (const long& longValue : m_longArrayValue) result += std::to_string(longValue) + ",";
        if (m_longArrayValue.size()) result.pop_back();
        return result + "]";
    }
    default: return "{}";
    }
}

NBTElement NBTElement::Byte(const char& byte, const NBTSettings& settings) {
    NBTElement element;
    element.m_type = NBTElementType::Byte;
    element.m_byteValue = byte;
    element.m_settings = settings;
    return element;
}
NBTElement NBTElement::Short(const short& shortValue, const NBTSettings& settings) {
    NBTElement element;
    element.m_type = NBTElementType::Short;
    element.m_shortValue = shortValue;
    element.m_settings = settings;
    return element;
}
NBTElement NBTElement::Int(const int& intValue, const NBTSettings& settings) {
    NBTElement element;
    element.m_type = NBTElementType::Int;
    element.m_intValue = intValue;
    element.m_settings = settings;
    return element;
}
NBTElement NBTElement::Long(const long& longValue, const NBTSettings& settings) {
    NBTElement element;
    element.m_type = NBTElementType::Long;
    element.m_longValue = longValue;
    element.m_settings = settings;
    return element;
}
NBTElement NBTElement::Float(const float& floatValue, const NBTSettings& settings) {
    NBTElement element;
    element.m_type = NBTElementType::Float;
    element.m_floatValue = floatValue;
    element.m_settings = settings;
    return element;
}
NBTElement NBTElement::Double(const double& doubleValue, const NBTSettings& settings) {
    NBTElement element;
    element.m_type = NBTElementType::Double;
    element.m_doubleValue = doubleValue;
    element.m_settings = settings;
    return element;
}
NBTElement NBTElement::ByteArray(const std::vector<char>& byteArray, const NBTSettings& settings) {
    NBTElement element;
    element.m_type = NBTElementType::ByteArray;
    element.m_byteArrayValue = byteArray;
    element.m_settings = settings;
    return element;
}
NBTElement NBTElement::String(const std::string& string, const NBTSettings& settings) {
    NBTElement element;
    element.m_type = NBTElementType::String;
    element.m_stringValue = string;
    element.m_settings = settings;
    return element;
}
NBTElement NBTElement::IntArray(const std::vector<int>& intArray, const NBTSettings& settings) {
    NBTElement element;
    element.m_type = NBTElementType::IntArray;
    element.m_intArrayValue = intArray;
    element.m_settings = settings;
    return element;
}
NBTElement NBTElement::LongArray(const std::vector<long>& longArray, const NBTSettings& settings) {
    NBTElement element;
    element.m_type = NBTElementType::LongArray;
    element.m_longArrayValue = longArray;
    element.m_settings = settings;
    return element;
}
NBTElement NBTElement::List(const std::vector<NBTElement>& children, const NBTSettings& settings) {
    NBTElement element;
    element.m_type = NBTElementType::List;
    element.m_childElements = children;
    element.m_settings = settings;
    return element;
}
NBTElement NBTElement::Compound(const std::vector<NBTElement>& children, const NBTSettings& settings) {
    NBTElement element;
    element.m_type = NBTElementType::Compound;
    element.m_childElements = children;
    element.m_settings = settings;
    return element;
}
NBTElement NBTElement::Byte(const std::string& tag, const char& byte, const NBTSettings& settings) {
    NBTElement element = NBTElement::Byte(byte, settings);
    element.m_tag = tag;
    return element;
}
NBTElement NBTElement::Short(const std::string& tag, const short& shortValue, const NBTSettings& settings) {
    NBTElement element = NBTElement::Short(shortValue, settings);
    element.m_tag = tag;
    return element;
}
NBTElement NBTElement::Int(const std::string& tag, const int& intValue, const NBTSettings& settings) {
    NBTElement element = NBTElement::Int(intValue, settings);
    element.m_tag = tag;
    return element;
}
NBTElement NBTElement::Long(const std::string& tag, const long& longValue, const NBTSettings& settings) {
    NBTElement element = NBTElement::Long(longValue, settings);
    element.m_tag = tag;
    return element;
}
NBTElement NBTElement::Float(const std::string& tag, const float& floatValue, const NBTSettings& settings) {
    NBTElement element = NBTElement::Float(floatValue, settings);
    element.m_tag = tag;
    return element;
}
NBTElement NBTElement::Double(const std::string& tag, const double& doubleValue, const NBTSettings& settings) {
    NBTElement element = NBTElement::Double(doubleValue, settings);
    element.m_tag = tag;
    return element;
}
NBTElement NBTElement::ByteArray(const std::string& tag, const std::vector<char>& byteArray, const NBTSettings& settings) {
    NBTElement element = NBTElement::ByteArray(byteArray, settings);
    element.m_tag = tag;
    return element;
}
NBTElement NBTElement::String(const std::string& tag, const std::string& string, const NBTSettings& settings) {
    NBTElement element = NBTElement::String(string, settings);
    element.m_tag = tag;
    return element;
}
NBTElement NBTElement::IntArray(const std::string& tag, const std::vector<int>& intArray, const NBTSettings& settings) {
    NBTElement element = NBTElement::IntArray(intArray, settings);
    element.m_tag = tag;
    return element;
}
NBTElement NBTElement::LongArray(const std::string& tag, const std::vector<long>& longArray, const NBTSettings& settings) {
    NBTElement element = NBTElement::LongArray(longArray, settings);
    element.m_tag = tag;
    return element;
}
NBTElement NBTElement::List(const std::string& tag, const std::vector<NBTElement>& children, const NBTSettings& settings) {
    NBTElement element = NBTElement::List(children, settings);
    element.m_tag = tag;
    return element;
}
NBTElement NBTElement::Compound(const std::string& tag, const std::vector<NBTElement>& children, const NBTSettings& settings) {
    NBTElement element = NBTElement::Compound(children, settings);
    element.m_tag = tag;
    return element;
}

}
