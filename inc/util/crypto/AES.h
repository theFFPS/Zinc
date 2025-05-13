#pragma once

#include <vector>
#include <openssl/aes.h>
#include "../Logger.h"

namespace zinc {

struct AESWrapper {
private:
    AES_KEY m_aesKey;
    std::vector<unsigned char> m_iv = std::vector<unsigned char>(16);
    bool m_isKeySet;
    bool m_isIVSet;
    Logger m_logger = Logger("AESWrapper");
public:
    AESWrapper() : m_isKeySet(false), m_isIVSet(false) {}
    ~AESWrapper() {}

    void setKey(const std::vector<unsigned char>& key);
    void setIV(const std::vector<unsigned char>& iv);

    std::vector<unsigned char> encryptCFB8(const std::vector<unsigned char>& data);
    std::vector<unsigned char> decryptCFB8(const std::vector<unsigned char>& data);
};

}
