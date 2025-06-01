#include <gtest/gtest.h>
#include <util/ZSTDUtil.h>
#include <type/ByteBuffer.h>

TEST(ZSTDUtilTest, CompressDecompress) {
    zinc::ByteBuffer data;
    data.writeString("Hello World!");
    zinc::ByteBuffer compressed = zinc::ZSTDUtil::compress(data.getBytes());
    zinc::ByteBuffer decompressed = zinc::ZSTDUtil::uncompress(compressed.getBytes(), data.size());
    EXPECT_EQ(data.getBytes(), decompressed.getBytes());
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
