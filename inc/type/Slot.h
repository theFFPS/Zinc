#pragma once

#include <vector>

namespace zinc {

struct ComponentWrapper {
    int m_type;
    std::vector<char> m_dataAndType;

    ComponentWrapper() : m_type(0) {}
    ComponentWrapper(const int& type, const std::vector<char>& dataAndType) : m_type(type), m_dataAndType(dataAndType) {}

    bool operator==(const ComponentWrapper& component) const;
    bool operator!=(const ComponentWrapper& component) const;
};
struct Slot {
    int m_itemId;
    int m_itemCount;
    std::vector<ComponentWrapper> m_componentsToAdd;
    std::vector<int> m_componentsToRemove;

    Slot() : m_itemId(-1), m_itemCount(0) {}
    Slot(const int& itemId, const int& itemCount, const std::vector<ComponentWrapper>& componentsToAdd, const std::vector<int>& componentsToRemove)
        : m_itemId(itemId), m_itemCount(itemCount), m_componentsToAdd(componentsToAdd), m_componentsToRemove(componentsToRemove) {}
    
    std::vector<char> toBytes() const;
    std::vector<char> toBytesHashed() const;

    bool operator==(const Slot& slot) const;
    bool operator!=(const Slot& slot) const;
};

}