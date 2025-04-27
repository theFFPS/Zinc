#include <iostream>
#include <stdexcept>

int main(int argc, char **argv) {
    try {
        
    } catch (const std::exception& e) {
        throw std::runtime_error("Error: " + std::string(e.what()));
    }
    return 0;
}
