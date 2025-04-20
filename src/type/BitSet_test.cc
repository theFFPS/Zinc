#include <type/BitSet.h>
#include <gtest/gtest.h>

using namespace zinc;

TEST(BitSetTest, DefaultConstructor) {
    BitSet bitSet;
    EXPECT_FALSE(bitSet.get(0));
    EXPECT_FALSE(bitSet.get(63));
    EXPECT_FALSE(bitSet.get(64));
}

TEST(BitSetTest, ConstructorWithWords) {
    std::vector<long> words = {1, 2, 3};
    BitSet bitSet(words);
    EXPECT_TRUE(bitSet.get(0));
    EXPECT_TRUE(bitSet.get(65));
    EXPECT_FALSE(bitSet.get(66));
}

TEST(BitSetTest, SetAndGet) {
    BitSet bitSet;
    bitSet.set(0);
    bitSet.set(64);
    bitSet.set(128);
    EXPECT_TRUE(bitSet.get(0));
    EXPECT_TRUE(bitSet.get(64));
    EXPECT_TRUE(bitSet.get(128));
    EXPECT_FALSE(bitSet.get(1));
    EXPECT_FALSE(bitSet.get(63));
    EXPECT_FALSE(bitSet.get(65));
}

TEST(BitSetTest, ToLongArray) {
    std::vector<long> words = {1, 2, 3};
    BitSet bitSet(words);
    std::vector<int64_t> longArray = bitSet.toLongArray();
    EXPECT_EQ(longArray.size(), words.size());
    for (size_t i = 0; i < words.size(); ++i) {
        EXPECT_EQ(longArray[i], static_cast<int64_t>(words[i]));
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
