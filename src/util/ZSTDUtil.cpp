#include <util/ZSTDUtil.h>
#include <util/Logger.h>
#include <zstd.h>

namespace zinc {

std::vector<char> ZSTDUtil::compress(const std::vector<char>& buffer, const int& level) {
    size_t compressBound = ZSTD_compressBound(buffer.size());
    std::vector<char> compressed(compressBound);
    size_t compressedSize = ZSTD_compress(
        compressed.data(), compressBound,
        buffer.data(), buffer.size(), 
        level
    );
    if (ZSTD_isError(compressedSize)) {
        Logger("ZSTDUtil").error("Compression failed: " + std::string(ZSTD_getErrorName(compressedSize)));
        return std::vector<char>();
    }
    compressed.resize(compressedSize);
    return compressed;
}
std::vector<char> ZSTDUtil::uncompress(const std::vector<char>& buffer, const size_t& decompressedSize) {
    if (!buffer.size()) {
        Logger("ZSTDUtil").error("Compressed buffer is empty");
        return std::vector<char>();
    }
    std::vector<char> decompressed(decompressedSize);
    size_t actualDecompressedSize = ZSTD_decompress(
        decompressed.data(), decompressedSize,
        buffer.data(), buffer.size()
    );
    if (ZSTD_isError(actualDecompressedSize)) {
        Logger("ZSTDUtil").error("Decompression failed: " + std::string(ZSTD_getErrorName(actualDecompressedSize)));
        return std::vector<char>();
    }
    if (actualDecompressedSize != decompressedSize) {
        Logger("ZSTDUtil").error("Decompressed size mismatch");
        return std::vector<char>();
    }
    return decompressed;
}

}