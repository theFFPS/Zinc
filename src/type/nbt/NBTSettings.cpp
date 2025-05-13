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
void NBTSettings::setType(const NBTElementType& type) {
    m_type = type;
}
bool NBTSettings::operator==(const NBTSettings& settings) const {
    return m_isInArray == settings.getIsInArray() && m_isNetwork == settings.getIsNetwork() && m_type == settings.getType();
}
bool NBTSettings::operator!=(const NBTSettings& settings) const {
    return !operator==(settings);
}

}