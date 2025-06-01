#pragma once

#include <type_traits>
#include <vector>
#include <mutex>
#include <cmath>
#include <limits>
#include <cstring>
#include <sys/mman.h>
#include <openssl/crypto.h>
#include "Logger.h"

namespace zinc {

template<typename Source, typename Target> Target zinc_safe_cast(const Source& source) {
    // using Common = std::common_type_t<Source, Target>;
    if constexpr (std::is_floating_point_v<Source>) {
        if (std::isnan(source)) {
            if constexpr (std::is_floating_point_v<Target>) {
                return std::numeric_limits<Target>::quiet_NaN();
            } else {
                return Target{0};
            }
        }
    }
    if constexpr (std::is_floating_point_v<Source>) {
        if (std::isinf(source)) {
            if constexpr (std::is_floating_point_v<Target>) {
                return (source > 0) ? std::numeric_limits<Target>::infinity() : -std::numeric_limits<Target>::infinity();
            } else {
                return (source > 0) ? std::numeric_limits<Target>::max() : std::numeric_limits<Target>::min();
            }
        }
    }
    // constexpr Common t_max = static_cast<Common>(std::numeric_limits<Target>::max());
    // constexpr Common t_min = static_cast<Common>(std::numeric_limits<Target>::min());
    // const Common c_value = static_cast<Common>(source);
    // if (c_value > t_max) {
    //     if constexpr (std::is_floating_point_v<Target>) {
    //         return std::numeric_limits<Target>::max();
    //     } else {
    //         return std::numeric_limits<Target>::max();
    //     }
    // }
    // if (c_value < t_min) {
    //     if constexpr (std::is_floating_point_v<Target>) {
    //         return std::numeric_limits<Target>::lowest();
    //     } else {
    //         return std::numeric_limits<Target>::min();
    //     }
    // }
    return static_cast<Target>(source);
}
template<typename T> struct SafeVector {
private:
    std::vector<T> m_data;
    mutable std::mutex m_mutex;
public:
    typedef T value_type;

    SafeVector() = default;
    explicit SafeVector(size_t size) : m_data(size) {}
    SafeVector(const T* data, size_t size) : m_data(data, data + size) {}
    SafeVector(const SafeVector&) = delete;
    SafeVector(SafeVector&& other) noexcept { *this = std::move(other); }
    ~SafeVector() {
        m_data.clear();
        m_data.shrink_to_fit();
    }

    T* data() noexcept { return m_data.data(); }
    const T* data() const noexcept { return m_data.data(); }
    size_t size() const noexcept { return m_data.size(); }
    bool empty() const noexcept { return m_data.empty(); }

    T& at(size_t pos) {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_data.at(pos);
    }
    const T& at(size_t pos) const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_data.at(pos);
    }
    void resize(size_t newSize) {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (newSize == m_data.size()) return;
        SafeVector<T> newVec(newSize);
        size_t copy_size = std::min(m_data.size(), newSize);
        if (copy_size > 0) {
            std::memcpy(newVec.data(), m_data.data(), copy_size * sizeof(T));
        }
        m_data = std::move(newVec.m_data);
    }
    void clear() {
        m_mutex.lock();
        m_data.clear();
        m_mutex.unlock();
    }

    SafeVector& operator=(const SafeVector&) = delete;
    SafeVector& operator=(SafeVector&& other) noexcept {
        if (this != &other) {
            m_mutex.lock();
            m_data = std::move(other.m_data);
            m_mutex.unlock();
        }
        return *this;
    }
    bool operator==(const SafeVector<T>& r_val) const {
        return r_val.m_data == m_data;
    }
    bool operator!=(const SafeVector<T>& r_val) const {
        return r_val.m_data != m_data;
    }
    T& operator[](size_t pos) {
        return at(pos);
    }
    const T& operator[](size_t pos) const {
        return at(pos);
    }
};
template<typename T> struct TypeContainer {
    static_assert(!std::is_pointer_v<T>, "TypeContainer cannot compress pointer targets");
private:
    mutable std::mutex m_mutex;
    T m_data;
public:
    typedef T value_type;

    TypeContainer() = default;
    TypeContainer(const T& data) : m_data(data) {}
    TypeContainer(const TypeContainer&) = delete;
    TypeContainer(TypeContainer&& other) noexcept { *this = std::move(other); }
    ~TypeContainer() {
        clear();
    }

    T* data() noexcept { return &m_data; }
    const T* data() const noexcept { return &m_data; }
    size_t size() const noexcept { return 1; }
    bool empty() const noexcept { return false; }

    void clear() {
        m_mutex.lock();
        OPENSSL_cleanse(&m_data, sizeof(T));
        m_mutex.unlock();
    }

    TypeContainer& operator=(const TypeContainer&) = delete;
    TypeContainer& operator=(TypeContainer&& other) noexcept {
        if (this != &other) {
            m_mutex.lock();
            m_data = std::move(other.m_data);
            m_mutex.unlock();
        }
        return *this;
    }
    bool operator==(const TypeContainer<T>& r_val) const {
        return r_val.m_data == m_data;
    }
    bool operator!=(const TypeContainer<T>& r_val) const {
        return r_val.m_data != m_data;
    }
};
template<typename T> struct Lockable {
    static_assert(!std::is_pointer_v<T>, "Lockable cannot lock pointer targets");
private:
    mutable std::mutex m_mutex;
    T m_data;
    bool m_isLocked = false;
    Logger m_logger = Logger("Lockable");

    void _lock() {
        if (m_data.empty()) return;
        m_isLocked = true;
        if (mlock(m_data.data(), m_data.size() * sizeof(T))) {
            m_logger.error("Memory locking failed: " + std::string(strerror(errno)));
            return;
        }
    }
    void _wipe() noexcept {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (!m_data.empty()) {
            OPENSSL_cleanse(m_data.data(), m_data.size() * sizeof(T));
        }
    }
    void _unlock() noexcept {
        m_isLocked = false;
        if (!m_data.empty() && m_isLocked) {
            _wipe();
            munlock(m_data.data(), m_data.size() * sizeof(T));
        }
    }
public:
    typedef T value_type;

    Lockable() = default;
    explicit Lockable(const T& value) : m_data(value) {
        _lock();
    }
    explicit Lockable(T&& value) : m_data(std::move(value)) {
        _lock();
    }
    Lockable(const Lockable&) = delete;
    Lockable(Lockable&& other) noexcept {
        std::unique_lock lock(other.m_mutex);
        m_data = std::move(other.m_data);
        m_isLocked = other.m_isLocked;
        other.m_isLocked = false;
    }
    ~Lockable() {
        _unlock();
    }

    Lockable& operator=(const Lockable&) = delete;
    Lockable& operator=(Lockable&& other) noexcept {
        if (this != &other) {
            std::unique_lock lock_this(m_mutex, std::defer_lock);
            std::unique_lock lock_other(other.m_mutex, std::defer_lock);
            std::lock(lock_this, lock_other);
            _unlock();
            m_data = std::move(other.m_data);
            m_isLocked = other.m_isLocked;
            other.m_isLocked = false;
        }
        return *this;
    }

    T& data() noexcept { return m_data; }
    const T& data() const noexcept { return m_data; }
    size_t size() const noexcept {
        return m_data.size();
    }
    bool empty() const noexcept {
        return !m_data.size();
    }

    bool operator==(const Lockable<T>& r_val) const {
        return r_val.m_data == m_data;
    }
    bool operator!=(const Lockable<T>& r_val) const {
        return r_val.m_data != m_data;
    }

    bool isLocked() const noexcept { return m_isLocked; }
};

}
