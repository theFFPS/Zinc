#include <gtest/gtest.h>
#include <util/crypto/AES.h>

TEST(AESTest, EncryptDecrypt) {
    std::vector<unsigned char> data = {'H','e','l','l','o',' ','w','o','r','l','d','!',0,0,0,0};
    std::vector<unsigned char> keyIV = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    zinc::AESWrapper aesEncrypt, aesDecrypt;
    aesEncrypt.setKey(keyIV);
    aesEncrypt.setIV(keyIV);
    aesDecrypt.setKey(keyIV);
    aesDecrypt.setIV(keyIV);
    std::vector<unsigned char> encryptedData = aesEncrypt.encryptCFB8(data);
    std::vector<unsigned char> decryptedData = aesDecrypt.decryptCFB8(encryptedData);
    EXPECT_EQ(data, decryptedData);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
