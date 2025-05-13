#include <util/crypto/AES.h>
#include <openssl/err.h>
#include <cstring>

namespace zinc {

void AESWrapper::setKey(const std::vector<unsigned char>& key) {
    if (AES_set_encrypt_key(key.data(), key.size() * 8, &m_aesKey)) {
        m_logger.error("AES key is invalid");
        return;
    }
    m_isKeySet = true;
    m_logger.debug("AES key updated");
}
void AESWrapper::setIV(const std::vector<unsigned char>& iv) {
    if (iv.size() != AES_BLOCK_SIZE) {
        m_logger.error("IV size must be equal to AES_BLOCK_SIZE [" + std::to_string(AES_BLOCK_SIZE) + "]");
        return;
    }
    memcpy(m_iv.data(), iv.data(), AES_BLOCK_SIZE);
    m_isIVSet = true;
    m_logger.debug("AES IV updated");
}

std::vector<unsigned char> AESWrapper::encryptCFB8(const std::vector<unsigned char>& data) {
    if (!m_isKeySet || !m_isIVSet) {
        m_logger.error("You must set both AES Key and IV");
        return {};
    }
    std::vector<unsigned char> encrypted(data.size());
    int num = 0;
    AES_cfb8_encrypt(data.data(), encrypted.data(), data.size(), &m_aesKey, m_iv.data(), &num, AES_ENCRYPT);
    m_logger.debug("called encryptCFB8()");
    return encrypted;
}

std::vector<unsigned char> AESWrapper::decryptCFB8(const std::vector<unsigned char>& data) {
    if (!m_isKeySet || !m_isIVSet) {
        m_logger.error("You must set both AES Key and IV");
        return {};
    }
    std::vector<unsigned char> decrypted(data.size());
    int num = 0;
    AES_cfb8_encrypt(data.data(), decrypted.data(), data.size(), &m_aesKey, m_iv.data(), &num, AES_DECRYPT);
    m_logger.debug("called decryptCFB8()");
    return decrypted;
}

}
