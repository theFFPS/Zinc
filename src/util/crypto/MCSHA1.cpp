#include <util/crypto/MCSHA1.h>
#include <util/Memory.h>
#include <sstream>
#include <iomanip>
#include <cstring>

namespace zinc {

void MCSHA1::update(const std::vector<unsigned char>& data) {
    EVP_DigestUpdate(m_ctx, data.data(), data.size());
}
void MCSHA1::update(const std::vector<char>& data) {
    EVP_DigestUpdate(m_ctx, data.data(), data.size());
}
void MCSHA1::final() {
    unsigned int len = 0;
    if (1 != EVP_DigestFinal_ex(m_ctx, m_digest.data(), &len)) {
        m_logger.error("EVP_DigestFinal_ex failed");
    }
}
std::string MCSHA1::digestJava() {
    bool isNegative = (m_digest[0] & 0x80) != 0;
    unsigned char twosComplement[20];
    if (isNegative) {
        for (size_t i = 0; i < 20; ++i) twosComplement[i] = ~m_digest[i];
        for (size_t i = 20 - 1; i != 0; --i) {
            twosComplement[i] += 1;
            if (twosComplement[i] != 0) break;
        }
    } else {
        memcpy(twosComplement, m_digest.data(), 20);
    }
    std::stringstream ss;
    if (isNegative) ss << "-";
    for (size_t i = 0; i < 20; ++i) ss << std::hex << std::setw(2) << std::setfill('0') << zinc_safe_cast<uint8_t, int>(twosComplement[i]);
    return ss.str();
}

}