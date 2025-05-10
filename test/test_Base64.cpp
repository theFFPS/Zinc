#include <gtest/gtest.h>
#include <util/crypto/Base64.h>

TEST(Base64Test, EncodeDecode) {
    std::string inputString = "Hello World!";
    std::string encodedData = zinc::Base64::encode(inputString);
    std::vector<unsigned char> decodedData = zinc::Base64::decode(encodedData);
    EXPECT_EQ(std::vector<unsigned char>(inputString.begin(), inputString.end()), decodedData);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
