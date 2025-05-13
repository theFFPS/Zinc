#pragma once

#include <vector>

namespace zinc {

struct RandomUtil {
    static std::vector<unsigned char> randomBytes(const size_t& length);
};

}