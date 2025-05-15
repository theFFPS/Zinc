#pragma once

namespace zinc {

void initCRC32();
unsigned crc32(const unsigned char* data, unsigned long length);

}