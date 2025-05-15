#include <util/crypto/CRC32.h>

namespace zinc {

unsigned crc32Table[256];
bool isCRC32Init = false;
void initCRC32() {
    if (isCRC32Init) return;
    for (unsigned i = 0; i < 256; ++i) {
        unsigned c = i;
        for (unsigned long j = 0; j < 8; ++j) {
            if (c & 1) c = (c >> 1) ^ 0xEDB88320;
            else c >>= 1;
        }
        crc32Table[i] = c;
    }
    isCRC32Init = true;
}
unsigned crc32(const unsigned char* data, unsigned long length) {
    unsigned crc = 0xFFFFFFFF;
    for (unsigned long i = 0; i < length; ++i) {
        crc = (crc >> 8) ^ crc32Table[(crc & 0xFF) ^ data[i]];
    }
    return crc ^ 0xFFFFFFFF;
}

}