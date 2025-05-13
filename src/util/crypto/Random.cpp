#include <util/crypto/Random.h>
#include <openssl/rand.h>
#include <util/Logger.h>

namespace zinc {

std::vector<unsigned char> RandomUtil::randomBytes(const size_t& length) {
    std::vector<unsigned char> buffer(length);
    int result = RAND_bytes(buffer.data(), static_cast<int>(length));
    if (result != 1) {
        Logger().error("Failed to generate random bytes. Defaulting to 0s");
        return std::vector<unsigned char>(length);
    }
    return buffer;
}

}