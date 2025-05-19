#include <util/crypto/Base64.h>
#include <util/Logger.h>
#include <openssl/evp.h>

namespace zinc {

constexpr size_t calculateEncodedSize(size_t input_len) noexcept { return 4 * ((input_len + 2) / 3); }
size_t calculatePadding(const std::string& encoded) noexcept {
    size_t padding = 0;
    if (!encoded.empty()) {
        if (encoded[encoded.size()-1] == '=') padding++;
        if (encoded.size() > 1 && encoded[encoded.size()-2] == '=') padding++;
    }
    return padding;
}

template<typename Container> std::string Base64::encode(const Container& data) {
    const auto* byteData = reinterpret_cast<const unsigned char*>(data.data());
    const size_t dataSize = data.size();
    if (!dataSize) return "";
    const size_t encodedSize = calculateEncodedSize(dataSize);
    std::string result(encodedSize, 0);
    const int outputLen = EVP_EncodeBlock( reinterpret_cast<unsigned char*>(result.data()), byteData, static_cast<int>(dataSize));
    if (outputLen <= 0 || static_cast<size_t>(outputLen) != encodedSize) {
        Logger("Base64").error("Encoding failed");
        return "";
    }
    return result;
}

template std::string Base64::encode<std::vector<unsigned char>>(const std::vector<unsigned char>&);
template std::string Base64::encode<std::vector<char>>(const std::vector<char>&);
template std::string Base64::encode<std::string>(const std::string&);

std::vector<unsigned char> Base64::decode(const std::string& encoded) {
    if (encoded.empty()) return {};
    if (encoded.size() % 4 != 0) {
        Logger("Base64").error("Invalid base64 length");
        return {};
    }
    const size_t padding = calculatePadding(encoded);
    const size_t maxDecodedSize = (encoded.size() / 4) * 3 - padding;
    std::vector<unsigned char> result(maxDecodedSize);
    const int outputLen = EVP_DecodeBlock(result.data(), reinterpret_cast<const unsigned char*>(encoded.data()), static_cast<int>(encoded.size()));
    if (outputLen < 0 || static_cast<size_t>(outputLen) != maxDecodedSize) {
        Logger("Base64").error("Decoding failed");
        return {};
    }
    return result;
}

}
