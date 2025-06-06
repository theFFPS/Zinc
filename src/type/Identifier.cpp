#include <type/Identifier.h>
#include <util/Memory.h>

namespace zinc {

Identifier::Identifier(const std::string& value) {
    std::size_t pos = value.find(':');
    m_namespace.resize(pos);
    m_object.resize(value.size() - (1 + pos));
    std::copy(value.begin(), value.begin() + zinc_safe_cast<size_t, long>(pos), m_namespace.begin());
    std::copy(value.begin() + zinc_safe_cast<size_t, long>(pos) + 1, value.end(), m_object.begin());
}
std::string Identifier::toString() const {
    return m_namespace + ":" + m_object;
}
bool Identifier::operator==(const Identifier& identifier) const {
    return toString() == identifier.toString();
}
bool Identifier::operator==(const std::string& identifier) const {
    return toString() == identifier;
}
bool Identifier::operator!=(const std::string& identifier) const {
    return toString() != identifier;
}
bool Identifier::operator!=(const Identifier& identifier) const {
    return toString() != identifier.toString();
}

}