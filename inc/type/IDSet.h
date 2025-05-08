#pragma once

#include <vector>
#include "Identifier.h"

namespace zinc {

struct IDSet {
private:
    int m_type;
    Identifier m_identifier;
    std::vector<int> m_ids;
public:
    IDSet() : m_type(-1) {}
    IDSet(const Identifier& identifier) : m_identifier(identifier), m_type(0) {}
    IDSet(const std::vector<int>& ids) : m_ids(ids), m_type(ids.size() + 1) {}

    void setIdentifier(const Identifier& identifier);
    void setIDs(const std::vector<int>& ids);
    void setType(const int& type);

    std::vector<int>& getIDs();
    std::vector<int> getIDs() const;
    Identifier& getIdentifier();
    Identifier getIdentifier() const;
    int& getType();
    int getType() const;

    bool operator==(const IDSet& idSet);
    bool operator!=(const IDSet& idSet);
};

}