#include <type/registry/IDSet.h>

namespace zinc {

IDSet::IDSet() {}
IDSet::IDSet(int type, std::string const& tagName) : m_type(type), m_tagName(tagName) {}
IDSet::IDSet(int type, std::string const& tagName, std::vector<int> const& ids) : m_type(type), m_tagName(tagName), m_ids(ids) {}
bool IDSet::operator==(IDSet const& b) {
    return m_type == b.m_type && m_tagName == b.m_tagName && m_ids == b.m_ids;
}
bool IDSet::operator!=(IDSet const& b) {
    return !operator==(b);
}

}