#pragma once

#include <vector>
#include <string>
#include <openssl/evp.h>
#include <openssl/rsa.h>

namespace zinc {

struct RSAWrapper {
private:
    EVP_PKEY* m_pkey = nullptr;
    bool m_isKeyLoaded;
    int m_padding;
public:
    RSAWrapper() : m_isKeyLoaded(false), m_padding(RSA_PKCS1_OAEP_PADDING) {}
    RSAWrapper(const int& padding) : m_isKeyLoaded(false), m_padding(padding) {}
    ~RSAWrapper() {
        if (m_pkey) EVP_PKEY_free(m_pkey);
    }

    std::vector<unsigned char> sign(const std::vector<unsigned char>& data);
    bool verify(const std::vector<unsigned char>& data, const std::vector<unsigned char>& signature);

    bool generateKeys(int bits = 2048);

    std::vector<unsigned char> encrypt(const std::vector<unsigned char>& data);
    std::vector<unsigned char> decrypt(const std::vector<unsigned char>& encryptedData);

    std::string getPublicKeyPEM() const;
    std::string getPrivateKeyPEM() const;
    std::vector<unsigned char> getPublicKeyDER() const;
    std::vector<unsigned char> getPrivateKeyDER() const;

    bool loadPublicKeyFromPEM(const std::string& pem);
    bool loadPrivateKeyFromPEM(const std::string& pem);
    bool loadPublicKeyFromDER(const std::vector<unsigned char>& der);
    bool loadPrivateKeyFromDER(const std::vector<unsigned char>& der);
};

}
