#pragma once

#include <optional>
#include <vector>
#include <string>

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

struct IDSet {
    int m_type;
    std::string m_tagName;
    std::vector<int> m_ids;

    IDSet();
    IDSet(int type, std::string const& tagName);
    IDSet(int type, std::string const& tagName, std::vector<int> const& ids);

    bool operator==(IDSet const& b);
    bool operator!=(IDSet const& b);
};

}