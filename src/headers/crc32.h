#ifndef rex_crc32_util
#define rex_crc32_util 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "cutil.h"

// crc32 hash function from http://home.thep.lu.se/~bjorn/crc/
// modified by StonkDragon

uint32_t rex_crc32_for_byte(uint32_t r) {
    for(uint32_t j = 0; j < 8; ++j) {
        r = (r & 1? 0: (uint32_t)0xEDB88320L) ^ r >> 1;
    }
    return r ^ (uint32_t)0xFF000000L;
}

uint32_t rex_crc32(uint8_t* data, uint32_t n_bytes) {
    uint32_t crc = 0;
    uint32_t table[0x100];
    for(uint32_t i = 0; i < 0x100; ++i) {
        table[i] = rex_crc32_for_byte(i);
    }

    for (uint32_t i = 0; i < n_bytes; ++i) {
        crc = table[(uint8_t)crc ^ data[i]] ^ crc >> 8;
    }
    return crc;
}

#endif
