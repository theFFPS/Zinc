#pragma once

#include <type/ByteBuffer.h>

namespace zinc {

struct ZLibUtil {
    static ByteBuffer compress(const ByteBuffer& buffer);
    static ByteBuffer uncompress(const ByteBuffer& buffer, const size_t& decompressedSize);
};

}