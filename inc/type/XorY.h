#pragma once

#include <optional>

namespace zinc {

template<typename TX, typename TY> struct XorY {
    std::optional<TX> m_x;
    std::optional<TY> m_y;
    
    XorY() {}
    XorY(const TX& x) : m_x(x) {}
    XorY(const TY& y) : m_y(y) {}

    bool operator==(const XorY<TX, TY>& xOrY) const {
        if (m_x.has_value() != xOrY.m_x.has_value()) return false;
        if (m_y.has_value() != xOrY.m_y.has_value()) return false;
        if (m_x.has_value()) return m_x.value() == xOrY.m_x.value();
        if (m_y.has_value()) return m_y.value() == xOrY.m_y.value();
        return false;
    }
    bool operator!=(const XorY<TX, TY>& xOrY) const {
        return !operator==(xOrY);
    }
};

}