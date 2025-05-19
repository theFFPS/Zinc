#pragma once

#include <string>
#include <vector>

namespace zinc {

struct Base64 {
    template<typename Container> static std::string encode(const Container& data);
    static std::vector<unsigned char> decode(const std::string& encoded);
};

}
