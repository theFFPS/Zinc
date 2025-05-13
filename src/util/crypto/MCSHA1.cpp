#include <util/crypto/MCSHA1.h>
#include <sstream>
#include <iomanip>
#include <cstring>

namespace zinc {

void MCSHA1::update(const std::vector<unsigned char>& data) {
    SHA1_Update(&m_ctx, data.data(), data.size());
}
void MCSHA1::update(const std::vector<char>& data) {
    SHA1_Update(&m_ctx, data.data(), data.size());
}
void MCSHA1::final() {
    SHA1_Final(m_digest.data(), &m_ctx);
}
std::string MCSHA1::digestJava() {
    bool isNegative = (m_digest[0] & 0x80) != 0;
    unsigned char twosComplement[SHA_DIGEST_LENGTH];
    if (isNegative) {
        for (size_t i = 0; i < SHA_DIGEST_LENGTH; ++i) {
            twosComplement[i] = ~m_digest[i];
        }
        for (size_t i = SHA_DIGEST_LENGTH - 1; i != 0; --i) {
            twosComplement[i] += 1;
            if (twosComplement[i] != 0) break;
        }
    } else {
        memcpy(twosComplement, m_digest.data(), SHA_DIGEST_LENGTH);
    }
    std::stringstream ss;
    if (isNegative) ss << "-";
    for (size_t i = 0; i < SHA_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)twosComplement[i];
    }
    return ss.str();
}

}