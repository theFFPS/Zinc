#include <string>
#include <util/crypto/AES.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <util/Memory.h>

namespace zinc {

void AESWrapper::setKey(const std::vector<unsigned char>& key) {
    if (key.size() != 16 && key.size() != 24 && key.size() != 32) {
        m_logger.error("Invalid AES key size: " + std::to_string(key.size()) + " bytes");
        return;
    }
    m_key = Lockable<SafeVector<unsigned char>>(SafeVector<unsigned char>(key.data(), key.size()));
    m_isKeySet = true;
    m_logger.debug("AES key updated");
}
void AESWrapper::setIV(const std::vector<unsigned char>& iv) {
    if (iv.size() != 16) {
        m_logger.error("IV size must be 16 bytes");
        return;
    }
    m_iv = Lockable<SafeVector<unsigned char>>(SafeVector<unsigned char>(iv.data(), iv.size()));
    m_isIVSet = true;
    m_logger.debug("AES IV updated");
}

std::vector<unsigned char> AESWrapper::encryptCFB8(const std::vector<unsigned char>& data) {
    return processCFB8(data, 1);
}

std::vector<unsigned char> AESWrapper::decryptCFB8(const std::vector<unsigned char>& data) {
    return processCFB8(data, 0);
}

std::vector<unsigned char> AESWrapper::processCFB8(const std::vector<unsigned char>& data, int enc) {
    if (!m_isKeySet || !m_isIVSet) {
        m_logger.error("You must set both AES Key and IV");
        return {};
    }

    const EVP_CIPHER* cipher = nullptr;
    switch (m_key.data().size()) {
        case 16: cipher = EVP_aes_128_cfb8(); break;
        case 24: cipher = EVP_aes_192_cfb8(); break;
        case 32: cipher = EVP_aes_256_cfb8(); break;
        default:
            m_logger.error("Invalid key length: " + std::to_string(m_key.data().size()));
            return {};
    }

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        m_logger.error("Failed to create EVP context");
        return {};
    }

    if (1 != EVP_CipherInit_ex(ctx, cipher, nullptr, m_key.data().data(), m_iv.data().data(), enc)) {
        char err_buf[256];
        ERR_error_string_n(ERR_get_error(), err_buf, sizeof(err_buf));
        m_logger.error(std::string("EVP_CipherInit_ex failed: ") + err_buf);
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }

    std::vector<unsigned char> out(data.size() + EVP_MAX_BLOCK_LENGTH);
    int out_len1 = 0;
    int out_len2 = 0;

    if (1 != EVP_CipherUpdate(ctx, out.data(), &out_len1, data.data(), zinc_safe_cast<size_t, int>(data.size()))) {
        char err_buf[256];
        ERR_error_string_n(ERR_get_error(), err_buf, sizeof(err_buf));
        m_logger.error(std::string("EVP_CipherUpdate failed: ") + err_buf);
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }

    if (1 != EVP_CipherFinal_ex(ctx, out.data() + out_len1, &out_len2)) {
        char err_buf[256];
        ERR_error_string_n(ERR_get_error(), err_buf, sizeof(err_buf));
        m_logger.error(std::string("EVP_CipherFinal_ex failed: ") + err_buf);
        EVP_CIPHER_CTX_free(ctx);
        return {};
    }

    const int total_len = out_len1 + out_len2;
    out.resize(zinc_safe_cast<int, size_t>(total_len));

    size_t iv_len = zinc_safe_cast<int, size_t>(EVP_CIPHER_CTX_get_iv_length(ctx));
    if (iv_len == 16) {
        if (1 != EVP_CIPHER_CTX_get_updated_iv(ctx, m_iv.data().data(), iv_len)) {
           char err_buf[256];
           ERR_error_string_n(ERR_get_error(), err_buf, sizeof(err_buf));
           m_logger.error(std::string("EVP_CIPHER_CTX_get_updated_iv failed: ") + err_buf);
           m_isIVSet = false;
        }
    } else {
        m_logger.error("Unexpected IV length: " + std::to_string(iv_len) + " (expected 16)");
        m_isIVSet = false;
    }

    EVP_CIPHER_CTX_free(ctx);

    m_logger.debug(enc ? "called encryptCFB8()" : "called decryptCFB8()");
    return out;
}

}
