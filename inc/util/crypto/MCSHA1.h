#pragma once

#include <openssl/evp.h>
#include <vector>
#include <util/Logger.h>
#include <string>

namespace zinc {

struct MCSHA1 {
private:
    EVP_MD_CTX *m_ctx;
    Logger m_logger = Logger("SHA1");
    std::vector<unsigned char> m_digest = std::vector<unsigned char>(20);
public:
    MCSHA1() {
        m_ctx = EVP_MD_CTX_new();
        if (!m_ctx) m_logger.error("EVP_MD_CTX_new failed");
        if (1 != EVP_DigestInit_ex(m_ctx, EVP_sha1(), nullptr)) {
            EVP_MD_CTX_free(m_ctx);
            m_logger.error("EVP_DigestInit_ex failed");
        }
    }
    ~MCSHA1() {
        if (m_ctx) EVP_MD_CTX_free(m_ctx);
    }

    void update(const std::vector<unsigned char>& data);
    void update(const std::vector<char>& data);
    void final();
    std::string digestJava();
};

}