#include <type/BitSet.h>
#include <util/Memory.h>

namespace zinc {

unsigned long BitSet::findHighestSet() const {
    for (unsigned long i = m_data.size(); i-- > 0;) {
        if (m_data[i] != 0) return i * 64 + zinc_safe_cast<int, size_t>(63 - __builtin_clzll(m_data[i]));
    }
    return NPOS;
}

void BitSet::set(unsigned long pos) {
    if (((pos + 64) / 64) > m_data.size()) m_data.resize((pos + 64) / 64);
    if (!(m_data[pos / 64] & (1ULL << (pos % 64)))) {
        m_data[pos / 64] |= (1ULL << (pos % 64));
        if (m_highestSet == NPOS || pos > m_highestSet) m_highestSet = pos;
    }
}
void BitSet::clear(unsigned long pos) {
    if ((pos / 64) >= m_data.size()) return;
    if (m_data[pos / 64] & (1ULL << (pos % 64))) {
        m_data[pos / 64] &= ~(1ULL << (pos % 64));
        if (pos == m_highestSet) m_highestSet = findHighestSet();
    }
}
void BitSet::flip(unsigned long pos) {
    if (((pos + 64) / 64) > m_data.size()) m_data.resize((pos + 64) / 64);
    m_data[pos / 64] ^= (1ULL << (pos % 64));
    if (m_data[pos / 64] & (1ULL << (pos % 64))) {
        if (m_highestSet == NPOS || pos > m_highestSet) m_highestSet = pos;
    } else {
        if (pos == m_highestSet) m_highestSet = findHighestSet();
    }
}
bool BitSet::get(unsigned long pos) const {
    if ((pos / 64) >= m_data.size()) return false;
    return (m_data[pos / 64] & (1ULL << (pos % 64))) != 0;
}
unsigned long BitSet::size() const {
    return (m_highestSet == NPOS) ? 0 : m_highestSet + 1;
}
unsigned long BitSet::count() const {
    unsigned long total = 0;
    for (unsigned long block : m_data) {
        total += zinc_safe_cast<int, size_t>(__builtin_popcountll(block));
    }
    return total;
}
bool BitSet::any() const { return m_highestSet != NPOS; }
bool BitSet::none() const { return m_highestSet == NPOS; }
void BitSet::andNot(const BitSet& other) {
    for (unsigned long i = 0; i < std::min(m_data.size(), other.m_data.size()); ++i) m_data[i] &= ~other.m_data[i];
    m_highestSet = findHighestSet();
}
unsigned long BitSet::nextSetBit(unsigned long fromIndex) const {
    if (fromIndex > m_highestSet) return NPOS;
    unsigned long block_idx = fromIndex / 64;
    unsigned long bit_offset = fromIndex % 64;
    while (block_idx < m_data.size()) {
        if (m_data[block_idx]) {
            if (m_data[block_idx] >> bit_offset) return block_idx * 64 + bit_offset 
                + zinc_safe_cast<int, size_t>(__builtin_ctzll(m_data[block_idx] >> bit_offset));
        }
        ++block_idx;
        bit_offset = 0;
    }
    return NPOS;
}
unsigned long BitSet::nextClearBit(unsigned long fromIndex) const {
    unsigned long block_idx = fromIndex / 64;
    unsigned long bit_offset = fromIndex % 64;
    while (block_idx < m_data.size()) {
        if ((~m_data[block_idx]) & (~((1ULL << bit_offset) - 1))) 
            return block_idx * 64 + zinc_safe_cast<int, size_t>(__builtin_ctzll((~m_data[block_idx]) & (~((1ULL << bit_offset) - 1))));
        ++block_idx;
        bit_offset = 0;
    }
    return m_data.size() * 64;
}

void BitSet::operator&=(const BitSet& other) {
    for (unsigned long i = 0; i < std::min(m_data.size(), other.m_data.size()); ++i) {
        m_data[i] &= other.m_data[i];
    }
    for (unsigned long i = std::min(m_data.size(), other.m_data.size()); i < m_data.size(); ++i) {
        m_data[i] = 0;
    }
    m_highestSet = findHighestSet();
}
void BitSet::operator|=(const BitSet& other) {
    if (std::max(m_data.size(), other.m_data.size()) > m_data.size()) {
        m_data.resize(std::max(m_data.size(), other.m_data.size()));
    }
    for (unsigned long i = 0; i < other.m_data.size(); ++i) {
        m_data[i] |= other.m_data[i];
    }
    m_highestSet = findHighestSet();
}
void BitSet::operator^=(const BitSet& other) {
    if (std::max(m_data.size(), other.m_data.size()) > m_data.size()) {
        m_data.resize(std::max(m_data.size(), other.m_data.size()));
    }
    for (unsigned long i = 0; i < other.m_data.size(); ++i) {
        m_data[i] ^= other.m_data[i];
    }
    m_highestSet = findHighestSet();
}
bool BitSet::operator==(const BitSet& other) const {
    for (unsigned long i = 0; i < std::max(m_data.size(), other.m_data.size()); ++i) {
        if (((i < m_data.size()) ? m_data[i] : 0) != ((i < other.m_data.size()) ? other.m_data[i] : 0)) return false;
    }
    return true;
}
bool BitSet::operator!=(const BitSet& other) const {
    return !operator==(other);
}

std::vector<unsigned long> BitSet::toLongArray() const {
    if (m_highestSet == NPOS) return {};
    return std::vector<unsigned long>(m_data.begin(), m_data.begin() + zinc_safe_cast<size_t, long>(m_highestSet / 64) + 1);
}
std::vector<unsigned char> BitSet::toByteArray() const {
    if (m_highestSet == NPOS) return {};
    std::vector<unsigned char> bytes((m_highestSet + 7) / 8, 0);
    for (unsigned long i = 0; i < (m_highestSet + 7) / 8; ++i) {
        unsigned char byte = 0;
        for (size_t j = 0; j < 8; ++j) {
            if (get(i * 8 + j)) byte |= (1 << j);
        }
        bytes[i] = byte;
    }
    return bytes;
}
BitSet BitSet::fromLongArray(const std::vector<unsigned long>& longs) {
    BitSet bs;
    bs.m_data = longs;
    bs.m_highestSet = bs.findHighestSet();
    return bs;
}
BitSet BitSet::fromByteArray(const std::vector<unsigned char>& bytes) {
    BitSet bs;
    bs.m_data.resize((bytes.size() + 7) / 8, 0);
    for (unsigned long i = 0; i < (bytes.size() + 7) / 8; ++i) {
        unsigned long block = 0;
        for (unsigned long j = 0; j < 8; ++j) {
            if ((i * 8 + j) >= bytes.size()) break;
            block |= static_cast<unsigned long>(bytes[i * 8 + j]) << (j * 8);
        }
        bs.m_data[i] = block;
    }
    bs.m_highestSet = bs.findHighestSet();
    return bs;
}

}