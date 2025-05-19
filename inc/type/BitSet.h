#pragma once

#include <vector>

namespace zinc {

struct BitSet {
    std::vector<unsigned long> m_data;
    unsigned long m_highestSet;
    static const unsigned long NPOS = static_cast<unsigned long>(-1);

    unsigned long findHighestSet() const;
    
    BitSet() : m_highestSet(NPOS) {}
    explicit BitSet(unsigned long n) : m_data((n + 63) / 64, 0), m_highestSet(NPOS) {}

    void set(unsigned long pos);
    void clear(unsigned long pos);
    void flip(unsigned long pos);
    bool get(unsigned long pos) const;
    unsigned long size() const;
    unsigned long count() const;
    bool any() const;
    bool none() const;
    void andNot(const BitSet& other);
    unsigned long nextSetBit(unsigned long fromIndex) const;
    unsigned long nextClearBit(unsigned long fromIndex) const;

    void operator&=(const BitSet& other);
    void operator|=(const BitSet& other);
    void operator^=(const BitSet& other);
    bool operator==(const BitSet& other) const;
    bool operator!=(const BitSet& other) const;

    std::vector<unsigned long> toLongArray() const;
    std::vector<unsigned char> toByteArray() const;
    static BitSet fromLongArray(const std::vector<unsigned long>& longs);
    static BitSet fromByteArray(const std::vector<unsigned char>& bytes);
};

}