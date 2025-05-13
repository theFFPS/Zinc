#pragma once

#include <openssl/sha.h>
#include <vector>
#include <string>

namespace zinc {

struct MCSHA1 {
private:
    SHA_CTX m_ctx;
    std::vector<unsigned char> m_digest = std::vector<unsigned char>(SHA_DIGEST_LENGTH);
public:
    MCSHA1() {
        SHA1_Init(&m_ctx);
    }

    void update(const std::vector<unsigned char>& data);
    void update(const std::vector<char>& data);
    void final();
    std::string digestJava();
};

}