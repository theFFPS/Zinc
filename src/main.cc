#include <iostream>
#include <stdexcept>
#include <ZincArguments.h>

int main(int argc, char **argv) {
    try {
        zinc::g_zincArguments.parse(argc, argv);
    } catch (const std::exception& e) {
        throw std::runtime_error("Error: " + std::string(e.what()));
    }
    return 0;
}
