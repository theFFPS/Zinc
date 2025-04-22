#include <type/gametypes/LightData.h>

namespace zinc {

bool LightData::operator==(LightData const& b) {
    return m_skyLightArrays == b.m_skyLightArrays && m_blockLightArrays == b.m_blockLightArrays && m_blockLightMask == b.m_blockLightMask &&
            m_blockLightMaskEmpty == b.m_blockLightMaskEmpty && m_skyLightMask == b.m_skyLightMask && m_skyLightMaskEmpty == b.m_skyLightMaskEmpty;
}
bool LightData::operator!=(LightData const& b) {
    return !operator==(b);
}   

}