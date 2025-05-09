#include <type/nbt/NBTElement.h>
#include <type/ByteBuffer.h>
#include <util/Logger.h>

namespace zinc {

std::vector<char> NBTElement::encode() const {
    ByteBuffer buffer;
    encode(buffer);
    return buffer.getBytes();
}
void NBTElement::encode(ByteBuffer& byteBuffer) const {
    NBTElementType type = m_type;
    if (m_settings.getType() != NBTElementType::End) type = m_settings.getType();
    if (!m_settings.getIsInArray()) byteBuffer.writeByte((char) type);
    if (type != NBTElementType::End && !m_settings.getIsInArray()) {
        if (type == NBTElementType::Compound) {
            if (!(m_tag.empty() && m_settings.getIsNetwork() && m_settings.getProtocol() >= 764)) {
                if (byteBuffer.getIsBigEndian()) {
                    byteBuffer.writeNumeric<unsigned short>(m_tag.size());
                    byteBuffer.writeBytes(std::vector<char>(m_tag.begin(), m_tag.end()));
                } else byteBuffer.writeString(m_tag);
            }
        } else {
            if (byteBuffer.getIsBigEndian()) {
                byteBuffer.writeNumeric<unsigned short>(m_tag.size());
                byteBuffer.writeBytes(std::vector<char>(m_tag.begin(), m_tag.end()));
            } else {
                if (!m_settings.getIsNetwork()) {
                    byteBuffer.writeNumeric<unsigned short>(m_tag.size());
                    byteBuffer.writeBytes(std::vector<char>(m_tag.begin(), m_tag.end()));
                } else byteBuffer.writeString(m_tag);
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
        if (byteBuffer.getIsBigEndian()) byteBuffer.writeNumeric<int>(m_intValue);
        else {
            if (m_settings.getIsNetwork()) byteBuffer.writeZigZagVarInt(m_intValue);
            else byteBuffer.writeNumeric<int>(m_intValue);
        }
        break;
    }
    case NBTElementType::Long: {
        if (byteBuffer.getIsBigEndian()) byteBuffer.writeNumeric<long>(m_longValue);
        else {
            if (m_settings.getIsNetwork()) byteBuffer.writeZigZagVarLong(m_longValue);
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
        if (byteBuffer.getIsBigEndian()) byteBuffer.writeNumeric<unsigned int>(m_byteArrayValue.size());
        else {
            if (m_settings.getIsNetwork()) byteBuffer.writeZigZagVarInt(m_byteArrayValue.size());
            else byteBuffer.writeNumeric<unsigned int>(m_byteArrayValue.size());
        }
        byteBuffer.writeByteArray(m_byteArrayValue);
        break;
    }
    case NBTElementType::IntArray: {
        if (byteBuffer.getIsBigEndian()) byteBuffer.writeNumeric<unsigned int>(m_intArrayValue.size());
        else {
            if (m_settings.getIsNetwork()) byteBuffer.writeZigZagVarInt(m_intArrayValue.size());
            else byteBuffer.writeNumeric<unsigned int>(m_intArrayValue.size());
        }
        byteBuffer.writeArray<int>(m_intArrayValue, &ByteBuffer::writeNumeric<int>);
        break;
    }
    case NBTElementType::LongArray: {
        if (byteBuffer.getIsBigEndian()) byteBuffer.writeNumeric<unsigned int>(m_longArrayValue.size());
        else {
            if (m_settings.getIsNetwork()) byteBuffer.writeZigZagVarInt(m_longArrayValue.size());
            else byteBuffer.writeNumeric<unsigned int>(m_longArrayValue.size());
        }
        byteBuffer.writeArray<long>(m_longArrayValue, &ByteBuffer::writeNumeric<long>);
        break;
    }
    case NBTElementType::List: {
        if (m_childElements.empty()) {
            byteBuffer.writeByte(0);
            if (byteBuffer.getIsBigEndian()) byteBuffer.writeNumeric<unsigned int>(0);
            else {
                if (m_settings.getIsNetwork()) byteBuffer.writeZigZagVarInt(0);
                else byteBuffer.writeNumeric<unsigned int>(0);
            }
        } else {
            NBTElementType elementType = m_childElements[0].getType();
            for (const NBTElement& element : m_childElements) if (elementType != element.getType()) {
                Logger("NBTEncode").error("All NBTList elements must have the same type");
                byteBuffer.writeByte(0);
                if (byteBuffer.getIsBigEndian()) byteBuffer.writeNumeric<unsigned int>(0);
                else {
                    if (m_settings.getIsNetwork()) byteBuffer.writeZigZagVarInt(0);
                    else byteBuffer.writeNumeric<unsigned int>(0);
                }
                break;
            }
            NBTSettings settings = m_settings;
            settings.setType(elementType);
            settings.setIsInArray(true);
            byteBuffer.writeByte((char) elementType);
            if (byteBuffer.getIsBigEndian()) byteBuffer.writeNumeric<unsigned int>(m_childElements.size());
            else {
                if (m_settings.getIsNetwork()) byteBuffer.writeZigZagVarInt(m_childElements.size());
                else byteBuffer.writeNumeric<unsigned int>(m_childElements.size());
            }
            for (const NBTElement& element : m_childElements) {
                NBTElement arrayElement = element;
                arrayElement.setSettings(settings);
                arrayElement.encode(byteBuffer);
            }
        }
        break;
    }
    case NBTElementType::String: {
        if (byteBuffer.getIsBigEndian()) {
            byteBuffer.writeNumeric<unsigned short>(m_stringValue.size());
            byteBuffer.writeBytes(std::vector<char>(m_stringValue.begin(), m_stringValue.end()));
        } else {
            if (!m_settings.getIsNetwork()) {
                byteBuffer.writeNumeric<unsigned short>(m_stringValue.size());
                byteBuffer.writeBytes(std::vector<char>(m_stringValue.begin(), m_stringValue.end()));
            } else byteBuffer.writeString(m_stringValue);
        }
        break;
    }
    case NBTElementType::Compound: {
        NBTSettings settings = m_settings;
        settings.setIsInArray(false);
        settings.setType(NBTElementType::End);
        for (const NBTElement& element : m_childElements) {
            NBTElement childElement = element;
            childElement.setSettings(settings);
            childElement.encode(byteBuffer);
        }
        byteBuffer.writeByte(0);
        break;
    }
    default: break;
    }
}
void NBTElement::decode(ByteBuffer& byteBuffer) {
    if (m_settings.getType() != NBTElementType::End) m_type = m_settings.getType();
    if (!m_settings.getIsInArray()) m_type = (NBTElementType) byteBuffer.readByte();
    if (m_type != NBTElementType::End && !m_settings.getIsInArray()) {
        if (m_type == NBTElementType::Compound) {
            if (!(m_tag.empty() && m_settings.getIsNetwork() && m_settings.getProtocol() >= 764)) {
                if (byteBuffer.getIsBigEndian()) {
                    std::vector<char> bytes = byteBuffer.readBytes(byteBuffer.readNumeric<unsigned short>());
                    m_tag = std::string(bytes.begin(), bytes.end());
                } else m_tag = byteBuffer.readString();
            }
        } else {
            if (byteBuffer.getIsBigEndian()) {
                std::vector<char> bytes = byteBuffer.readBytes(byteBuffer.readNumeric<unsigned short>());
                m_tag = std::string(bytes.begin(), bytes.end());
            } else {
                if (!m_settings.getIsNetwork()) {
                    std::vector<char> bytes = byteBuffer.readBytes(byteBuffer.readNumeric<unsigned short>());
                    m_tag = std::string(bytes.begin(), bytes.end());
                } else m_tag = byteBuffer.readString();
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
        if (byteBuffer.getIsBigEndian()) m_intValue = byteBuffer.readNumeric<int>();
        else {
            if (m_settings.getIsNetwork()) m_intValue = byteBuffer.readZigZagVarInt();
            else m_intValue = byteBuffer.readNumeric<int>();
        }
        break;
    }
    case NBTElementType::Long: {
        if (byteBuffer.getIsBigEndian()) m_longValue = byteBuffer.readNumeric<long>();
        else {
            if (m_settings.getIsNetwork()) m_longValue = byteBuffer.readZigZagVarLong();
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
        if (byteBuffer.getIsBigEndian()) length = byteBuffer.readNumeric<unsigned int>();
        else {
            if (m_settings.getIsNetwork()) length = byteBuffer.readZigZagVarInt();
            else length = byteBuffer.readNumeric<unsigned int>();
        }
        m_byteArrayValue = byteBuffer.readByteArray(length);
        break;
    }
    case NBTElementType::IntArray: {
        unsigned length = 0;
        if (byteBuffer.getIsBigEndian()) length = byteBuffer.readNumeric<unsigned int>();
        else {
            if (m_settings.getIsNetwork()) length = byteBuffer.readZigZagVarInt();
            else length = byteBuffer.readNumeric<unsigned int>();
        }
        m_intArrayValue = byteBuffer.readArray<int>(&ByteBuffer::readNumeric<int>, length);
        break;
    }
    case NBTElementType::LongArray: {
        unsigned length = 0;
        if (byteBuffer.getIsBigEndian()) length = byteBuffer.readNumeric<unsigned int>();
        else {
            if (m_settings.getIsNetwork()) length = byteBuffer.readZigZagVarInt();
            else length = byteBuffer.readNumeric<unsigned int>();
        }
        m_longArrayValue = byteBuffer.readArray<long>(&ByteBuffer::readNumeric<long>, length);
        break;
    }
    case NBTElementType::String: {
        unsigned short length = 0;
        if (byteBuffer.getIsBigEndian()) {
            length = byteBuffer.readNumeric<unsigned short>();
            std::vector<char> bytes = byteBuffer.readBytes(length);
            m_stringValue = std::string(bytes.begin(), bytes.end());
        } else {
            if (!m_settings.getIsNetwork()) {
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
        if (byteBuffer.getIsBigEndian()) length = byteBuffer.readNumeric<unsigned int>();
        else {
            if (m_settings.getIsNetwork()) length = byteBuffer.readZigZagVarInt();
            else length = byteBuffer.readNumeric<unsigned int>();
        }
        if (type == NBTElementType::End) length = 0;
        settings.setType(type);
        settings.setIsInArray(true);
        for (unsigned i = 0; i < length; i++) {
            NBTElement arrayElement;
            arrayElement.setSettings(settings);
            arrayElement.decode(byteBuffer);
            m_childElements.push_back(arrayElement);
        }
        break;        
    }
    case NBTElementType::Compound: {
        NBTSettings settings = m_settings;
        settings.setIsInArray(false);
        settings.setType(NBTElementType::End);
        while (true) {
            if (byteBuffer.getReadPointer() < byteBuffer.getBytes().size()) {
                if (!byteBuffer.getBytes()[byteBuffer.getReadPointer()]) {
                    byteBuffer.readByte();
                    break;
                }
            } else break;
            NBTElement childElement;
            childElement.setSettings(settings);
            childElement.decode(byteBuffer);
            m_childElements.push_back(childElement);
        }
        break;
    }
    default: break;
    }
}
NBTSettings& NBTElement::getSettings() {
    return m_settings;
}
NBTSettings NBTElement::getSettings() const {
    return m_settings;
}
NBTElementType& NBTElement::getType() {
    return m_type;
}
NBTElementType NBTElement::getType() const {
    return m_type;
}
std::string& NBTElement::getTag() {
    return m_tag;
}
std::string NBTElement::getTag() const {
    return m_tag;
}
void NBTElement::setSettings(const NBTSettings& settings) {
    m_settings = settings;
}
void NBTElement::setType(const NBTElementType& type) {
    m_type = type;
}
void NBTElement::setTag(const std::string& tag) {
    m_tag = tag;
}
char& NBTElement::getByte() {
    return m_byteValue;
}
char NBTElement::getByte() const {
    return m_byteValue;
}
short& NBTElement::getShort() {
    return m_shortValue;
}
short NBTElement::getShort() const {
    return m_shortValue;
}
int& NBTElement::getInt() {
    return m_intValue;
}
int NBTElement::getInt() const {
    return m_intValue;
}
long& NBTElement::getLong() {
    return m_longValue;
}
long NBTElement::getLong() const {
    return m_longValue;
}
float& NBTElement::getFloat() {
    return m_floatValue;
}
float NBTElement::getFloat() const {
    return m_floatValue;
}
double& NBTElement::getDouble() {
    return m_doubleValue;
}
double NBTElement::getDouble() const {
    return m_doubleValue;
}
std::vector<char>& NBTElement::getByteArray() {
    return m_byteArrayValue;
}
std::vector<char> NBTElement::getByteArray() const {
    return m_byteArrayValue;
}
std::string& NBTElement::getString() {
    return m_stringValue;
}
std::string NBTElement::getString() const {
    return m_stringValue;
}
std::vector<NBTElement>& NBTElement::getChildElements() {
    return m_childElements;
}
std::vector<NBTElement> NBTElement::getChildElements() const {
    return m_childElements;
}
std::vector<int>& NBTElement::getIntArray() {
    return m_intArrayValue;
}
std::vector<int> NBTElement::getIntArray() const {
    return m_intArrayValue;
}
std::vector<long>& NBTElement::getLongArray() {
    return m_longArrayValue;
}
std::vector<long> NBTElement::getLongArray() const {
    return m_longArrayValue;
}
void NBTElement::setByte(const char& byteValue) {
    m_byteValue = byteValue;
    m_type = NBTElementType::Byte;
}
void NBTElement::setShort(const short& shortValue) {
    m_shortValue = shortValue;
    m_type = NBTElementType::Short;
}
void NBTElement::setInt(const int& intValue) {
    m_intValue = intValue;
    m_type = NBTElementType::Int;
}
void NBTElement::setLong(const long& longValue) {
    m_longValue = longValue;
    m_type = NBTElementType::Long;
}
void NBTElement::setFloat(const float& floatValue) {
    m_floatValue = floatValue;
    m_type = NBTElementType::Float;
}
void NBTElement::setDouble(const double& doubleValue) {
    m_doubleValue = doubleValue;
    m_type = NBTElementType::Double;
}
void NBTElement::setByteArray(const std::vector<char>& byteArrayValue) {
    m_byteArrayValue = byteArrayValue;
    m_type = NBTElementType::ByteArray;
}
void NBTElement::setString(const std::string& stringValue) {
    m_stringValue = stringValue;
    m_type = NBTElementType::String;
}
void NBTElement::setChildElements(const std::vector<NBTElement>& childElements) {
    m_childElements = childElements;
}
void NBTElement::setIntArray(const std::vector<int>& intArrayValue) {
    m_intArrayValue = intArrayValue;
    m_type = NBTElementType::IntArray;
}
void NBTElement::setLongArray(const std::vector<long>& longArrayValue) {
    m_longArrayValue = longArrayValue;
    m_type = NBTElementType::LongArray;
}
bool NBTElement::operator==(const NBTElement& element) const {
    if (element.getType() != m_type) return false;
    if (element.getSettings() != m_settings) return false;
    switch (m_type) {
    case NBTElementType::Byte: return m_byteValue == element.getByte();
    case NBTElementType::Short: return m_shortValue == element.getShort();
    case NBTElementType::Int: return m_intValue == element.getInt();
    case NBTElementType::Long: return m_longValue == element.getLong();
    case NBTElementType::Float: return m_floatValue == element.getFloat();
    case NBTElementType::Double: return m_doubleValue == element.getDouble();
    case NBTElementType::ByteArray: return m_byteArrayValue == element.getByteArray();
    case NBTElementType::String: return m_stringValue == element.getString();
    case NBTElementType::List:
    case NBTElementType::Compound: return m_childElements == element.getChildElements();
    case NBTElementType::IntArray: return m_intArrayValue == element.getIntArray();
    case NBTElementType::LongArray: return m_longArrayValue == element.getLongArray();
    default: return true;
    }
}
bool NBTElement::operator!=(const NBTElement& element) const {
    return !operator==(element);
}

}