#pragma once

#include <string>

namespace zinc {

struct ZincArguments {
    bool m_debug = false;
    std::string m_workDir = ".";
    
    void parse(int argc, char **argv);
};
extern ZincArguments g_zincArguments;

}