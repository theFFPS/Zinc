#pragma once

#include <vector>

namespace zinc {

struct ZLibUtil {
    static std::vector<char> compress(const std::vector<char>& buffer);
    static std::vector<char> uncompress(const std::vector<char>& buffer, const size_t& decompressedSize);
};

}