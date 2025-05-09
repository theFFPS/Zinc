#pragma once

namespace zinc {

enum class NBTElementType : char {
    End,
    Byte, Short, Int, Long, Float, Double,
    ByteArray, String, List, Compound, IntArray, LongArray
};

}