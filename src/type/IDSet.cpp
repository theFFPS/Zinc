#include <type/IDSet.h>
#include <util/Memory.h>

namespace zinc {

void IDSet::setIdentifier(const Identifier& identifier) {
    m_type = 0;
    m_identifier = identifier;
}
void IDSet::setIDs(const std::vector<int>& ids) {
    m_type = zinc_safe_cast<size_t, int>(ids.size() + 1);
    m_ids = ids;
}
void IDSet::setType(const int& type) {
    m_type = type;
    if (!type) m_ids.clear();
}
std::vector<int>& IDSet::getIDs() {
    return m_ids;
}
std::vector<int> IDSet::getIDs() const {
    return m_ids;
}
Identifier& IDSet::getIdentifier() {
    return m_identifier;
}
Identifier IDSet::getIdentifier() const {
    return m_identifier;
}
int& IDSet::getType() {
    return m_type;
}
int IDSet::getType() const {
    return m_type;
}
bool IDSet::operator==(const IDSet& idSet) {
    return idSet.getType() == getType() && idSet.getIdentifier() == getIdentifier() && idSet.getIDs() == getIDs();
}
bool IDSet::operator!=(const IDSet& idSet) {
    return !operator==(idSet);
}

}