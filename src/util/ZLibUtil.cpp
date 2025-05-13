#include <util/ZLibUtil.h>
#include <util/Logger.h>
#include <zlib-ng.h>

namespace zinc {

Logger m_zlibLogger = Logger("ZLib");

ByteBuffer ZLibUtil::compress(const ByteBuffer& buffer) {
    size_t compressedSize = zng_compressBound(buffer.size());
    std::vector<char> compressed(compressedSize);
    int ret = zng_compress2((uint8_t*)compressed.data(), &compressedSize, (const uint8_t*)buffer.data(), buffer.size(), Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        m_zlibLogger.error("Compression failed: " + std::string(zng_zError(ret)));
        return {};
    }
    m_zlibLogger.debug("Compressed " + std::to_string(buffer.size()) + " bytes");
    return compressed;
}
ByteBuffer ZLibUtil::uncompress(const ByteBuffer& buffer, const size_t& decompressedSize) {
    size_t m_decompressedSize = decompressedSize;
    size_t sourceLen = buffer.size();
    std::vector<char> decompressed(decompressedSize);
    int ret = zng_uncompress2((uint8_t*)decompressed.data(), &m_decompressedSize, (const uint8_t*)buffer.data(), &sourceLen);
    if (ret != Z_OK) {
        m_zlibLogger.error("Decompression failed: " + std::string(zng_zError(ret)));
        return {};
    }
    m_zlibLogger.debug("Decompressed " + std::to_string(buffer.size()) + " bytes");
    return decompressed;
}

}
