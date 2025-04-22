#pragma once

#include <optional>

namespace zinc {

template<typename T> struct IDorX {
    int m_id;
    std::optional<T> m_value;

    IDorX() {}
    IDorX(int id) : m_id(id) {}
    IDorX(std::optional<T> const& value) : m_id(0), m_value(value) {}

    bool operator==(IDorX const& b) {
        return m_id == b.m_id && m_value == b.m_value;
    }
    bool operator!=(IDorX const& b) {
        return !operator==(b);
    }
};

}