#include <type/nbt/NBTSettings.h>

namespace zinc {

bool& NBTSettings::getIsInArray() {
    return m_isInArray;
}
bool NBTSettings::getIsInArray() const {
    return m_isInArray;
}
bool& NBTSettings::getIsNetwork() {
    return m_isNetwork;
}
bool NBTSettings::getIsNetwork() const {
    return m_isNetwork;
}
int& NBTSettings::getProtocol() {
    return m_protocol;
}
int NBTSettings::getProtocol() const {
    return m_protocol;
}
NBTElementType& NBTSettings::getType() {
    return m_type;
}
NBTElementType NBTSettings::getType() const {
    return m_type;
}
void NBTSettings::setIsInArray(const bool& isInArray) {
    m_isInArray = isInArray;
}
void NBTSettings::setIsNetwork(const bool& isNetwork) {
    m_isNetwork = isNetwork;
}
void NBTSettings::setProtocol(const int& protocol) {
    m_protocol = protocol;
}
void NBTSettings::setType(const NBTElementType& type) {
    m_type = type;
}
bool NBTSettings::operator==(const NBTSettings& settings) const {
    return m_protocol == settings.getProtocol() && m_isInArray == settings.getIsInArray() && m_isNetwork == settings.getIsNetwork() && m_type == settings.getType();
}
bool NBTSettings::operator!=(const NBTSettings& settings) const {
    return !operator==(settings);
}

}