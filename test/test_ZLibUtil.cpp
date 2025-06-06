#include <gtest/gtest.h>
#include <util/ZLibUtil.h>
#include <type/ByteBuffer.h>

TEST(ZLibUtilTest, CompressDecompress) {
    zinc::ByteBuffer data;
    data.writeString("Hello World!");
    zinc::ByteBuffer compressed = zinc::ZLibUtil::compress(data.getBytes());
    zinc::ByteBuffer decompressed = zinc::ZLibUtil::uncompress(compressed.getBytes(), data.size());
    EXPECT_EQ(data.getBytes(), decompressed.getBytes());
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
