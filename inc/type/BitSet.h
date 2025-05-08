#pragma once

#include <vector>

namespace zinc {

struct BitSet {
private:
    std::vector<unsigned long> data;
    unsigned long highest_set;
    static const unsigned long npos = static_cast<unsigned long>(-1);

    unsigned long findHighestSet() const;
public:
    BitSet() : highest_set(npos) {}
    explicit BitSet(unsigned long n) : data((n + 63) / 64, 0), highest_set(npos) {}

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