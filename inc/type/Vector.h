#pragma once

#include <vector>

namespace zinc {

template<typename T> struct Vector2 {
private:
    T m_x, m_y;
public:
    Vector2() : m_x(0), m_y(0) {}
    Vector2(const T& x, const T& y) : m_x(x), m_y(y) {}

    T& getX() {
        return m_x;
    }
    const T getX() const {
        return m_x;
    }
    T& getY() {
        return m_y;
    }
    const T getY() const {
        return m_y;
    }

    T& operator[](size_t i) {
        if (i == 0) return m_x;
        else return m_y;
    }
    const T operator[](size_t i) const {
        if (i == 0) return m_x;
        else return m_y;
    }

    std::vector<T> getValues() const {
        return { m_x, m_y };
    }

    bool operator==(const Vector2<T>& vector) const {
        return vector.getValues() == getValues();
    }
    bool operator==(const std::vector<T>& vector) const {
        return vector == getValues();
    }
    bool operator!=(const std::vector<T>& vector) const {
        return vector != getValues();
    }
    bool operator!=(const Vector2<T>& vector) const {
        return vector.getValues() != getValues();
    }
};
template<typename T> struct Vector3 {
private:
    T m_x, m_y, m_z;
public:
    Vector3() : m_x(0), m_y(0), m_z(0) {}
    Vector3(const T& x, const T& y, const T& z) : m_x(x), m_y(y), m_z(z) {}

    T& getX() {
        return m_x;
    }
    const T getX() const {
        return m_x;
    }
    T& getY() {
        return m_y;
    }
    const T getY() const {
        return m_y;
    }
    T& getZ() {
        return m_z;
    }
    const T getZ() const {
        return m_z;
    }

    T& operator[](size_t i) {
        if (i == 0) return m_x;
        if (i == 1) return m_y;
        else return m_z;
    }
    const T operator[](size_t i) const {
        if (i == 0) return m_x;
        if (i == 1) return m_y;
        else return m_z;
    }

    std::vector<T> getValues() const {
        return { m_x, m_y, m_z };
    }

    bool operator==(const Vector3<T>& vector) const {
        return vector.getValues() == getValues();
    }
    bool operator==(const std::vector<T>& vector) const {
        return vector == getValues();
    }
    bool operator!=(const std::vector<T>& vector) const {
        return vector != getValues();
    }
    bool operator!=(const Vector3<T>& vector) const {
        return vector.getValues() != getValues();
    }
};
typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;
typedef Vector3<int> Vector3i;
typedef Vector3<float> Vector3f;

}