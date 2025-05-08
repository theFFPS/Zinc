#pragma once

#include <string>

namespace zinc {

struct Identifier {
private:
    std::string m_namespace;
    std::string m_object;
public:
    Identifier() : m_namespace("minecraft"), m_object("placeholder") {}
    Identifier(const std::string& namespaceV, const std::string& object) : m_namespace(namespaceV), m_object(object) {}
    Identifier(const std::string& value);

    std::string toString() const;

    bool operator==(const Identifier& identifier) const;
    bool operator==(const std::string& identifier) const;
    bool operator!=(const std::string& identifier) const;
    bool operator!=(const Identifier& identifier) const;
};

}