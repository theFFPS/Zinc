#pragma once

#include <vector>
#include "../Memory.h"
#include "../Logger.h"

namespace zinc {

struct AESWrapper {
private:
    Lockable<SafeVector<unsigned char>> m_key;
    Lockable<SafeVector<unsigned char>> m_iv = Lockable<SafeVector<unsigned char>>(SafeVector<unsigned char>(16));
    bool m_isKeySet;
    bool m_isIVSet;
    Logger m_logger = Logger("AESWrapper");

    std::vector<unsigned char> processCFB8(const std::vector<unsigned char>& data, int enc);
public:
    AESWrapper() : m_isKeySet(false), m_isIVSet(false) {}
    ~AESWrapper() {}

    void setKey(const std::vector<unsigned char>& key);
    void setIV(const std::vector<unsigned char>& iv);

    std::vector<unsigned char> encryptCFB8(const std::vector<unsigned char>& data);
    std::vector<unsigned char> decryptCFB8(const std::vector<unsigned char>& data);
};

}
