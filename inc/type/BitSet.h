#pragma once

#include <vector>
#include <cstdint>

namespace zinc {

struct BitSet {
private:
    std::vector<uint64_t> m_words;
    
public:
    BitSet() = default;
    BitSet(std::vector<long> const& words);
    void set(size_t bitIndex);
    bool get(size_t bitIndex) const;
    std::vector<int64_t> toLongArray() const;
};

}
