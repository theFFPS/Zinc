#include <gtest/gtest.h>
#include <util/crypto/RSA.h>

TEST(RSATest, EncryptDecrypt) {
    zinc::RSAWrapper rsa;
    rsa.generateKeys(2048);
    std::vector<unsigned char> inputData = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
    std::vector<unsigned char> encryptedData = rsa.encrypt(inputData);
    std::vector<unsigned char> decryptedData = rsa.decrypt(encryptedData);
    EXPECT_EQ(inputData, decryptedData);
}
TEST(RSATest, SignVerify) {
    zinc::RSAWrapper rsa;
    rsa.generateKeys(2048);
    std::vector<unsigned char> inputData = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
    std::vector<unsigned char> signedData = rsa.sign(inputData);
    EXPECT_TRUE(rsa.verify(inputData, signedData));
}
TEST(RSATest, PrivatePEM) {
    zinc::RSAWrapper rsa, rsa2;
    rsa.generateKeys(2048);
    rsa2.loadPrivateKeyFromPEM(rsa.getPrivateKeyPEM());
    EXPECT_EQ(rsa.getPrivateKeyPEM(), rsa2.getPrivateKeyPEM());
}
TEST(RSATest, PublicPEM) {
    zinc::RSAWrapper rsa, rsa2;
    rsa.generateKeys(2048);
    rsa2.loadPublicKeyFromPEM(rsa.getPublicKeyPEM());
    EXPECT_EQ(rsa.getPublicKeyPEM(), rsa2.getPublicKeyPEM());
}
TEST(RSATest, PrivateDER) {
    zinc::RSAWrapper rsa, rsa2;
    rsa.generateKeys(2048);
    rsa2.loadPrivateKeyFromDER(rsa.getPrivateKeyDER());
    EXPECT_EQ(rsa.getPrivateKeyDER(), rsa2.getPrivateKeyDER());
}
TEST(RSATest, PublicDER) {
    zinc::RSAWrapper rsa, rsa2;
    rsa.generateKeys(2048);
    rsa2.loadPublicKeyFromDER(rsa.getPublicKeyDER());
    EXPECT_EQ(rsa.getPublicKeyDER(), rsa2.getPublicKeyDER());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
