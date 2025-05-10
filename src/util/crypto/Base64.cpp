#include <util/crypto/Base64.h>
#include <util/Logger.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/evp.h>

namespace zinc {

std::string Base64::encode(const std::vector<unsigned char>& data) {
    BIO* bio = BIO_new(BIO_s_mem());
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO_push(b64, bio);

    BIO_write(b64, data.data(), data.size());
    BIO_flush(b64);

    BUF_MEM* bufferPtr;
    BIO_get_mem_ptr(bio, &bufferPtr);

    std::string encoded(bufferPtr->data, bufferPtr->length);
    BIO_free_all(b64);

    return encoded;
}
std::string Base64::encode(const std::vector<char>& data) {
    return encode(std::vector<unsigned char>(data.begin(), data.end()));
}
std::string Base64::encode(const std::string& data) {
    return encode(std::vector<unsigned char>(data.begin(), data.end()));
}
std::vector<unsigned char> Base64::decode(const std::string& encodedData) {
    BIO* bio = BIO_new_mem_buf((void*)encodedData.c_str(), encodedData.size());
    BIO* b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO_push(b64, bio);

    std::vector<unsigned char> decoded(encodedData.size());
    int decodedSize = BIO_read(b64, decoded.data(), encodedData.size());
    BIO_free_all(b64);

    if (decodedSize <= 0) {
        Logger("Base64").error("Base64 decoding failed");
        return {};
    }

    decoded.resize(decodedSize);
    return decoded;
}

}
