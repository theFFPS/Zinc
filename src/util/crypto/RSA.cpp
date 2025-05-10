#include <util/crypto/RSA.h>
#include <util/Logger.h>
#include <openssl/err.h>
#include <openssl/pem.h>

namespace zinc {

bool RSAWrapper::generateKeys(int bits) {
    BIGNUM* bne = BN_new();
    if (!bne) return false;
    BN_set_word(bne, RSA_F4);
    int ret = RSA_generate_key_ex(m_rsa, bits, bne, NULL);
    BN_free(bne);
    if (ret != 1) return false;
    m_isKeyLoaded = true;
    return true;
}

std::vector<unsigned char> RSAWrapper::encrypt(const std::vector<unsigned char>& data) {
    Logger logger ("RSAEncrypt");
    if (!m_isKeyLoaded) {
        logger.error("RSA keyring is not loaded!");
        return {};
    }
    if (m_padding != RSA_PKCS1_PADDING && m_padding != RSA_PKCS1_OAEP_PADDING) {
        logger.error("Unknown RSA padding detected");
        return {};
    }
    int maxSize = RSA_size(m_rsa);
    if (data.size() > maxSize - (m_padding == RSA_PKCS1_PADDING ? 11 : 42)) {
        logger.error("Data is too large to be encrypted");
        return {};
    }
    std::vector<unsigned char> encrypted(maxSize);
    int encryptedSize = RSA_public_encrypt(data.size(), data.data(), encrypted.data(), m_rsa, m_padding);
    if (encryptedSize == -1) {
        logger.error("Encryption error occured (" + std::string(ERR_error_string(ERR_get_error(), nullptr)) + ")");
        return {};
    }

    encrypted.resize(encryptedSize);
    return encrypted;
}
std::vector<unsigned char> RSAWrapper::decrypt(const std::vector<unsigned char>& encryptedData) {
    Logger logger ("RSADecrypt");
    if (!m_isKeyLoaded) {
        logger.error("RSA keyring is not loaded!");
        return {};
    }
    if (m_padding != RSA_PKCS1_PADDING && m_padding != RSA_PKCS1_OAEP_PADDING) {
        logger.error("Unknown RSA padding detected");
        return {};
    }

    int maxSize = RSA_size(m_rsa);
    if (encryptedData.size() > maxSize) {
        logger.error("Data is too large to be decrypted");
        return {};
    }

    std::vector<unsigned char> decrypted(maxSize);
    int decryptedSize = RSA_private_decrypt(encryptedData.size(), encryptedData.data(), decrypted.data(), m_rsa, m_padding);
    if (decryptedSize == -1) {
        logger.error("Decryption error occured (" + std::string(ERR_error_string(ERR_get_error(), nullptr)) + ")");
        return {};
    }

    decrypted.resize(decryptedSize);
    return decrypted;
}

std::string RSAWrapper::getPublicKeyPEM() const {
    if (!m_isKeyLoaded) return "";
    BIO* bio = BIO_new(BIO_s_mem());
    if (!bio) return "";
    if (PEM_write_bio_RSAPublicKey(bio, m_rsa) != 1) {
        BIO_free(bio);
        return "";
    }
    BUF_MEM* bufferPtr;
    BIO_get_mem_ptr(bio, &bufferPtr);
    std::string pem(bufferPtr->data, bufferPtr->length);
    BIO_free(bio);
    return pem;
}
std::vector<unsigned char> RSAWrapper::getPublicKeyDER() const {
    if (!m_isKeyLoaded) return {};
    unsigned char* der = NULL;
    int derLen = i2d_RSAPublicKey(m_rsa, &der);
    if (derLen <= 0) return {};
    std::vector<unsigned char> derVec(der, der + derLen);
    OPENSSL_free(der);
    return derVec;
}
std::vector<unsigned char> RSAWrapper::getPrivateKeyDER() const {
    if (!m_isKeyLoaded) return {};
    unsigned char* der = NULL;
    int derLen = i2d_RSAPrivateKey(m_rsa, &der);
    if (derLen <= 0) return {};
    std::vector<unsigned char> derVec(der, der + derLen);
    OPENSSL_free(der);
    return derVec;
}
std::string RSAWrapper::getPrivateKeyPEM() const {
    if (!m_isKeyLoaded) return "";
    BIO* bio = BIO_new(BIO_s_mem());
    if (!bio) return "";
    if (PEM_write_bio_RSAPrivateKey(bio, m_rsa, NULL, NULL, 0, NULL, NULL) != 1) {
        BIO_free(bio);
        return "";
    }
    BUF_MEM* bufferPtr;
    BIO_get_mem_ptr(bio, &bufferPtr);
    std::string pem(bufferPtr->data, bufferPtr->length);
    BIO_free(bio);
    return pem;
}

bool RSAWrapper::loadPublicKeyFromPEM(const std::string& pem) {
    BIO* bio = BIO_new_mem_buf((void*)pem.c_str(), pem.size());
    if (!bio) return false;
    RSA* loadedRSA = PEM_read_bio_RSAPublicKey(bio, NULL, NULL, NULL);
    BIO_free(bio);
    if (!loadedRSA) return false;
    if (m_rsa) RSA_free(m_rsa);
    m_rsa = loadedRSA;
    m_isKeyLoaded = true;
    return true;
}
bool RSAWrapper::loadPrivateKeyFromPEM(const std::string& pem) {
    BIO* bio = BIO_new_mem_buf((void*)pem.c_str(), pem.size());
    if (!bio) return false;
    RSA* loadedRSA = PEM_read_bio_RSAPrivateKey(bio, NULL, NULL, NULL);
    BIO_free(bio);
    if (!loadedRSA) return false;
    if (m_rsa) RSA_free(m_rsa);
    m_rsa = loadedRSA;
    m_isKeyLoaded = true;
    return true;
}
bool RSAWrapper::loadPublicKeyFromDER(const std::vector<unsigned char>& der) {
    const unsigned char* derPtr = der.data();
    RSA* loadedRSA = d2i_RSAPublicKey(NULL, &derPtr, der.size());
    if (!loadedRSA) return false;
    if (m_rsa) RSA_free(m_rsa);
    m_rsa = loadedRSA;
    m_isKeyLoaded = true;
    return true;
}
bool RSAWrapper::loadPrivateKeyFromDER(const std::vector<unsigned char>& der) {
    const unsigned char* derPtr = der.data();
    RSA* loadedRSA = d2i_RSAPrivateKey(NULL, &derPtr, der.size());
    if (!loadedRSA) return false;
    if (m_rsa) RSA_free(m_rsa);
    m_rsa = loadedRSA;
    m_isKeyLoaded = true;
    return true;
}

}
