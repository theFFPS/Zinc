#include <util/crypto/RSA.h>
#include <util/Memory.h>
#include <util/Logger.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/sha.h>

namespace zinc {

bool RSAWrapper::generateKeys(int bits) {
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr);
    if (!ctx) return false;
    if (EVP_PKEY_keygen_init(ctx) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return false;
    }    
    if (EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, bits) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return false;
    }
    EVP_PKEY* pkey = nullptr;
    if (EVP_PKEY_generate(ctx, &pkey) <= 0) {
        EVP_PKEY_CTX_free(ctx);
        return false;
    }
    EVP_PKEY_CTX_free(ctx);
    if (m_pkey) EVP_PKEY_free(m_pkey);
    m_pkey = pkey;
    m_isKeyLoaded = true;
    return true;
}

std::vector<unsigned char> RSAWrapper::encrypt(const std::vector<unsigned char>& data) {
    Logger logger("RSAEncrypt");
    if (!m_isKeyLoaded) {
        logger.error("RSA key not loaded");
        return {};
    }
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(m_pkey, nullptr);
    if (!ctx) {
        logger.error("Context creation failed");
        return {};
    }
    if (EVP_PKEY_encrypt_init(ctx) <= 0) {
        logger.error("Encrypt init failed");
        EVP_PKEY_CTX_free(ctx);
        return {};
    }
    if (EVP_PKEY_CTX_set_rsa_padding(ctx, m_padding) <= 0) {
        logger.error("Padding set failed");
        EVP_PKEY_CTX_free(ctx);
        return {};
    }
    size_t outlen;
    if (EVP_PKEY_encrypt(ctx, nullptr, &outlen, data.data(), data.size()) <= 0) {
        logger.error("Size determination failed");
        EVP_PKEY_CTX_free(ctx);
        return {};
    }
    std::vector<unsigned char> encrypted(outlen);
    if (EVP_PKEY_encrypt(ctx, encrypted.data(), &outlen, data.data(), data.size()) <= 0) {
        logger.error("Encryption failed");
        EVP_PKEY_CTX_free(ctx);
        return {};
    }
    EVP_PKEY_CTX_free(ctx);
    encrypted.resize(outlen);
    return encrypted;
}
std::vector<unsigned char> RSAWrapper::decrypt(const std::vector<unsigned char>& encryptedData) {
    Logger logger("RSADecrypt");
    if (!m_isKeyLoaded) {
        logger.error("RSA key not loaded");
        return {};
    }
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(m_pkey, nullptr);
    if (!ctx) {
        logger.error("Context creation failed");
        return {};
    }
    if (EVP_PKEY_decrypt_init(ctx) <= 0) {
        logger.error("Decrypt init failed");
        EVP_PKEY_CTX_free(ctx);
        return {};
    }
    if (EVP_PKEY_CTX_set_rsa_padding(ctx, m_padding) <= 0) {
        logger.error("Padding set failed");
        EVP_PKEY_CTX_free(ctx);
        return {};
    }    
    size_t outlen;
    if (EVP_PKEY_decrypt(ctx, nullptr, &outlen, encryptedData.data(), encryptedData.size()) <= 0) {
        logger.error("Size determination failed");
        EVP_PKEY_CTX_free(ctx);
        return {};
    }
    std::vector<unsigned char> decrypted(outlen);
    if (EVP_PKEY_decrypt(ctx, decrypted.data(), &outlen, encryptedData.data(), encryptedData.size()) <= 0) {
        logger.error("Decryption failed");
        EVP_PKEY_CTX_free(ctx);
        return {};
    }
    EVP_PKEY_CTX_free(ctx);
    decrypted.resize(outlen);
    return decrypted;
}

std::string RSAWrapper::getPublicKeyPEM() const {
    if (!m_isKeyLoaded) return "";
    BIO* bio = BIO_new(BIO_s_mem());
    if (!bio) return "";
    if (PEM_write_bio_PUBKEY(bio, m_pkey) != 1) {
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
    unsigned char* der = nullptr;
    int derLen = i2d_PUBKEY(m_pkey, &der);
    if (derLen <= 0) return {};
    std::vector<unsigned char> derVec(der, der + derLen);
    OPENSSL_free(der);
    return derVec;
}
std::vector<unsigned char> RSAWrapper::getPrivateKeyDER() const {
    if (!m_isKeyLoaded) return {};
    unsigned char* der = nullptr;
    int derLen = i2d_PrivateKey(m_pkey, &der);
    if (derLen <= 0) return {};
    std::vector<unsigned char> derVec(der, der + derLen);
    OPENSSL_free(der);
    return derVec;
}
std::string RSAWrapper::getPrivateKeyPEM() const {
    if (!m_isKeyLoaded) return "";
    BIO* bio = BIO_new(BIO_s_mem());
    if (!bio) return "";
    if (PEM_write_bio_PrivateKey(bio, m_pkey, nullptr, nullptr, 0, nullptr, nullptr) != 1) {
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
    BIO* bio = BIO_new_mem_buf(pem.data(), zinc_safe_cast<size_t, int>(pem.size()));
    if (!bio) return false;
    EVP_PKEY* pkey = PEM_read_bio_PUBKEY(bio, nullptr, nullptr, nullptr);
    BIO_free(bio);
    if (!pkey) return false;
    if (m_pkey) EVP_PKEY_free(m_pkey);
    m_pkey = pkey;
    m_isKeyLoaded = true;
    return true;
}
bool RSAWrapper::loadPrivateKeyFromPEM(const std::string& pem) {
    BIO* bio = BIO_new_mem_buf(pem.data(), zinc_safe_cast<size_t, int>(pem.size()));
    if (!bio) return false;
    EVP_PKEY* pkey = PEM_read_bio_PrivateKey(bio, nullptr, nullptr, nullptr);
    BIO_free(bio);
    if (!pkey) return false;
    if (m_pkey) EVP_PKEY_free(m_pkey);
    m_pkey = pkey;
    m_isKeyLoaded = true;
    return true;
}
bool RSAWrapper::loadPublicKeyFromDER(const std::vector<unsigned char>& der) {
    const unsigned char* derPtr = der.data();
    EVP_PKEY* pkey = d2i_PUBKEY(nullptr, &derPtr, zinc_safe_cast<size_t, int>(der.size()));
    if (!pkey) return false;
    if (m_pkey) EVP_PKEY_free(m_pkey);
    m_pkey = pkey;
    m_isKeyLoaded = true;
    return true;
}
bool RSAWrapper::loadPrivateKeyFromDER(const std::vector<unsigned char>& der) {
    const unsigned char* derPtr = der.data();
    EVP_PKEY* pkey = d2i_PrivateKey(EVP_PKEY_RSA, nullptr, &derPtr, zinc_safe_cast<size_t, int>(der.size()));
    if (!pkey) return false;
    
    if (m_pkey) EVP_PKEY_free(m_pkey);
    m_pkey = pkey;
    m_isKeyLoaded = true;
    return true;
}

std::vector<unsigned char> RSAWrapper::sign(const std::vector<unsigned char>& data) {
    Logger logger("RSASign");
    if (!m_isKeyLoaded) {
        logger.error("RSA key not loaded");
        return {};
    }
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (!mdctx) {
        logger.error("Context creation failed");
        return {};
    }
    if (EVP_DigestSignInit(mdctx, nullptr, EVP_sha256(), nullptr, m_pkey) <= 0) {
        logger.error("Sign init failed");
        EVP_MD_CTX_free(mdctx);
        return {};
    }
    if (EVP_DigestSignUpdate(mdctx, data.data(), data.size()) <= 0) {
        logger.error("Sign update failed");
        EVP_MD_CTX_free(mdctx);
        return {};
    }
    size_t siglen;
    if (EVP_DigestSignFinal(mdctx, nullptr, &siglen) <= 0) {
        logger.error("Sign size failed");
        EVP_MD_CTX_free(mdctx);
        return {};
    }
    std::vector<unsigned char> signature(siglen);
    if (EVP_DigestSignFinal(mdctx, signature.data(), &siglen) <= 0) {
        logger.error("Sign final failed");
        EVP_MD_CTX_free(mdctx);
        return {};
    }
    EVP_MD_CTX_free(mdctx);
    signature.resize(siglen);
    return signature;
}

bool RSAWrapper::verify(const std::vector<unsigned char>& data, const std::vector<unsigned char>& signature) {
    Logger logger("RSAVerify");
    if (!m_isKeyLoaded) {
        logger.error("RSA key not loaded");
        return false;
    }
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (!mdctx) {
        logger.error("Context creation failed");
        return false;
    }
    if (EVP_DigestVerifyInit(mdctx, nullptr, EVP_sha256(), nullptr, m_pkey) <= 0) {
        logger.error("Verify init failed");
        EVP_MD_CTX_free(mdctx);
        return false;
    }
    if (EVP_DigestVerifyUpdate(mdctx, data.data(), data.size()) <= 0) {
        logger.error("Verify update failed");
        EVP_MD_CTX_free(mdctx);
        return false;
    }
    int result = EVP_DigestVerifyFinal(mdctx, signature.data(), signature.size());
    EVP_MD_CTX_free(mdctx);
    return result == 1;
}

}
