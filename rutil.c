#ifndef stringutil_c
#define stringutil_c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

size_t trimwhitespace(char *out, size_t len, const char *str)
{
  if(len == 0)
    return 0;

  const char *end;
  size_t out_size;

  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
  {
    *out = 0;
    return 1;
  }

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;
  end++;

  // Set output size to minimum of trimmed string length and buffer size minus 1
  out_size = (end - str) < len-1 ? (end - str) : len-1;

  // Copy trimmed string and add null terminator
  memcpy(out, str, out_size);
  out[out_size] = 0;

  return out_size;
}

// crc32 hash function from http://home.thep.lu.se/~bjorn/crc/
// modified by StonkDragon
uint32_t crc32_for_byte(uint32_t r) {
  for(int j = 0; j < 8; ++j) {
    r = (r & 1? 0: (uint32_t)0xEDB88320L) ^ r >> 1;
  }
  return r ^ (uint32_t)0xFF000000L;
}

uint32_t crc32(const void *data, size_t n_bytes) {
  uint32_t crc = 0;
  uint32_t table[0x100];
  for(size_t i = 0; i < 0x100; ++i) {
    table[i] = crc32_for_byte(i);
  }

  for (size_t i = 0; i < n_bytes; ++i) {
    crc = table[crc ^ ((uint8_t*)data)[i]] ^ crc >> 8;
  }
  return crc;
}

#endif