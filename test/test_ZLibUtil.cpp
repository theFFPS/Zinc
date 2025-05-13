#include <gtest/gtest.h>
#include <util/ZLibUtil.h>

TEST(ZLibUtilTest, CompressDecompress) {
    zinc::ByteBuffer data;
    data.writeString("Hello World!");
    zinc::ByteBuffer compressed = zinc::ZLibUtil::compress(data);
    zinc::ByteBuffer decompressed = zinc::ZLibUtil::uncompress(compressed, data.size());
    EXPECT_EQ(data.getBytes(), decompressed.getBytes());
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
