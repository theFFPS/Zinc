#pragma once

#include <string>
#include <vector>

namespace zinc {

struct Base64 {
    static std::string encode(const std::vector<unsigned char>& data);
    static std::string encode(const std::vector<char>& data);
    static std::string encode(const std::string& data);
    static std::vector<unsigned char> decode(const std::string& encodedData);
};

}
