#pragma once

#include <optional>

namespace zinc {

template<typename T> struct IDorX {
private:
    int m_id;
    std::optional<T> m_value;
public:
    IDorX() : m_id(-1) {}
    IDorX(const int& id) : m_id(id) {}
    IDorX(const T& value) : m_id(0), m_value(value) {}

    void setId(const int& id) {
        m_id = id;
        if (id) m_value.reset();
    }
    void setValue(const T& value) {
        m_id = 0;
        m_value = value;
    }

    T& getValue() {
        return m_value.value();
    }
    T getValue() const {
        return m_value.value_or(T{});
    }
    int& getId() {
        return m_id;
    }
    int getId() const {
        return m_id;
    }

    bool operator==(const IDorX<T>& idOrX) const {
        return idOrX.getId() == getId() && idOrX.getValue() == getValue();
    }
    bool operator!=(const IDorX<T>& idOrX) const {
        return !operator==(idOrX);
    }
};

}