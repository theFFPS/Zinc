#include "type/BitSet.h"
#include <cstring>

namespace zinc {

BitSet::BitSet(std::vector<long> const& words) : m_words(words.begin(), words.end()) {};

void BitSet::set(size_t bitIndex) {
    size_t wordIndex = bitIndex / 64;
    if (wordIndex >= m_words.size()) {
        m_words.resize(wordIndex + 1, 0);
    }
    m_words[wordIndex] |= (1ULL << (bitIndex % 64));
}

bool BitSet::get(size_t bitIndex) const {
    size_t wordIndex = bitIndex / 64;
    if (wordIndex >= m_words.size()) {
        return false;
    }
    return (m_words[wordIndex] & (1ULL << (bitIndex % 64))) != 0;
}

std::vector<int64_t> BitSet::toLongArray() const {
    std::vector<int64_t> longArray(m_words.size());
    for (size_t i = 0; i < m_words.size(); ++i) {
        longArray[i] = static_cast<int64_t>(m_words[i]);
    }
    return longArray;
}

} // namespace zinc
