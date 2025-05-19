#include <type/nbt/NBTSettings.h>

namespace zinc {

bool NBTSettings::operator==(const NBTSettings& settings) const {
    return m_isInArray == settings.m_isInArray && m_isNetwork == settings.m_isNetwork && m_type == settings.m_type;
}
bool NBTSettings::operator!=(const NBTSettings& settings) const {
    return !operator==(settings);
}

}