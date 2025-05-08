#include <type/BitSet.h>

namespace zinc {

unsigned long BitSet::findHighestSet() const {
    for (unsigned long i = data.size(); i-- > 0;) {
        if (data[i] != 0) return i * 64 + (63 - __builtin_clzll(data[i]));
    }
    return npos;
}

void BitSet::set(unsigned long pos) {
    unsigned long required_blocks = (pos + 64) / 64;
    if (required_blocks > data.size()) {
        data.resize(required_blocks);
    }
    unsigned long block = pos / 64;
    unsigned long mask = 1ULL << (pos % 64);
    if (!(data[block] & mask)) {
        data[block] |= mask;
        if (highest_set == npos || pos > highest_set) highest_set = pos;
    }
}
void BitSet::clear(unsigned long pos) {
    unsigned long block = pos / 64;
    if (block >= data.size()) return;
    unsigned long mask = 1ULL << (pos % 64);
    if (data[block] & mask) {
        data[block] &= ~mask;
        if (pos == highest_set) highest_set = findHighestSet();
    }
}
void BitSet::flip(unsigned long pos) {
    unsigned long required_blocks = (pos + 64) / 64;
    if (required_blocks > data.size()) {
        data.resize(required_blocks);
    }
    unsigned long block = pos / 64;
    unsigned long mask = 1ULL << (pos % 64);
    data[block] ^= mask;
    if (data[block] & mask) {
        if (highest_set == npos || pos > highest_set) highest_set = pos;
    } else {
        if (pos == highest_set) highest_set = findHighestSet();
    }
}
bool BitSet::get(unsigned long pos) const {
    unsigned long block = pos / 64;
    if (block >= data.size()) return false;
    return (data[block] & (1ULL << (pos % 64))) != 0;
}
unsigned long BitSet::size() const {
    return (highest_set == npos) ? 0 : highest_set + 1;
}
unsigned long BitSet::count() const {
    unsigned long total = 0;
    for (unsigned long block : data) {
        total += __builtin_popcountll(block);
    }
    return total;
}
bool BitSet::any() const { return highest_set != npos; }
bool BitSet::none() const { return highest_set == npos; }
void BitSet::andNot(const BitSet& other) {
    unsigned long min_blocks = std::min(data.size(), other.data.size());
    for (unsigned long i = 0; i < min_blocks; ++i) {
        data[i] &= ~other.data[i];
    }
    highest_set = findHighestSet();
}
unsigned long BitSet::nextSetBit(unsigned long fromIndex) const {
    if (fromIndex > highest_set) return npos;
    unsigned long block_idx = fromIndex / 64;
    unsigned long bit_offset = fromIndex % 64;
    while (block_idx < data.size()) {
        unsigned long block = data[block_idx];
        if (block != 0) {
            unsigned long masked = block >> bit_offset;
            if (masked != 0) {
                unsigned long tz = __builtin_ctzll(masked);
                return block_idx * 64 + bit_offset + tz;
            }
        }
        ++block_idx;
        bit_offset = 0;
    }
    return npos;
}
unsigned long BitSet::nextClearBit(unsigned long fromIndex) const {
    unsigned long block_idx = fromIndex / 64;
    unsigned long bit_offset = fromIndex % 64;
    while (block_idx < data.size()) {
        unsigned long block = data[block_idx];
        unsigned long inverted = ~block;
        inverted &= ~((1ULL << bit_offset) - 1);
        if (inverted != 0) {
            unsigned long tz = __builtin_ctzll(inverted);
            return block_idx * 64 + tz;
        }
        ++block_idx;
        bit_offset = 0;
    }
    return data.size() * 64;
}

void BitSet::operator&=(const BitSet& other) {
    unsigned long min_blocks = std::min(data.size(), other.data.size());
    for (unsigned long i = 0; i < min_blocks; ++i) {
        data[i] &= other.data[i];
    }
    for (unsigned long i = min_blocks; i < data.size(); ++i) {
        data[i] = 0;
    }
    highest_set = findHighestSet();
}
void BitSet::operator|=(const BitSet& other) {
    unsigned long required_blocks = std::max(data.size(), other.data.size());
    if (required_blocks > data.size()) {
        data.resize(required_blocks);
    }
    for (unsigned long i = 0; i < other.data.size(); ++i) {
        data[i] |= other.data[i];
    }
    highest_set = findHighestSet();
}
void BitSet::operator^=(const BitSet& other) {
    unsigned long required_blocks = std::max(data.size(), other.data.size());
    if (required_blocks > data.size()) {
        data.resize(required_blocks);
    }
    for (unsigned long i = 0; i < other.data.size(); ++i) {
        data[i] ^= other.data[i];
    }
    highest_set = findHighestSet();
}
bool BitSet::operator==(const BitSet& other) const {
    unsigned long max_blocks = std::max(data.size(), other.data.size());
    for (unsigned long i = 0; i < max_blocks; ++i) {
        unsigned long this_block = (i < data.size()) ? data[i] : 0;
        unsigned long other_block = (i < other.data.size()) ? other.data[i] : 0;
        if (this_block != other_block) return false;
    }
    return true;
}
bool BitSet::operator!=(const BitSet& other) const {
    return !operator==(other);
}

std::vector<unsigned long> BitSet::toLongArray() const {
    if (highest_set == npos) {
        return {};
    }
    unsigned long num_blocks = (highest_set / 64) + 1;
    return std::vector<unsigned long>(data.begin(), data.begin() + num_blocks);
}
std::vector<unsigned char> BitSet::toByteArray() const {
    if (highest_set == npos) {
        return {};
    }
    unsigned long num_bytes = (highest_set + 7) / 8;
    std::vector<unsigned char> bytes(num_bytes, 0);
    for (unsigned long i = 0; i < num_bytes; ++i) {
        unsigned char byte = 0;
        for (int j = 0; j < 8; ++j) {
            unsigned long pos = i * 8 + j;
            if (get(pos)) {
                byte |= (1 << j);
            }
        }
        bytes[i] = byte;
    }
    return bytes;
}
BitSet BitSet::fromLongArray(const std::vector<unsigned long>& longs) {
    BitSet bs;
    bs.data = longs;
    bs.highest_set = bs.findHighestSet();
    return bs;
}
BitSet BitSet::fromByteArray(const std::vector<unsigned char>& bytes) {
    BitSet bs;
    unsigned long num_blocks = (bytes.size() + 7) / 8;
    bs.data.resize(num_blocks, 0);
    for (unsigned long i = 0; i < num_blocks; ++i) {
        unsigned long block = 0;
        for (unsigned long j = 0; j < 8; ++j) {
            unsigned long byte_index = i * 8 + j;
            if (byte_index >= bytes.size()) break;
            block |= static_cast<unsigned long>(bytes[byte_index]) << (j * 8);
        }
        bs.data[i] = block;
    }
    bs.highest_set = bs.findHighestSet();
    return bs;
}

}