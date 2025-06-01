#pragma once

#include <vector>

namespace zinc {

struct ZSTDUtil {
    static std::vector<char> compress(const std::vector<char>& buffer, const int& level = 3);
    static std::vector<char> uncompress(const std::vector<char>& buffer, const size_t& decompressedSize);
};

}