#pragma once

#include <vector>
#include <string>

namespace zinc {

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