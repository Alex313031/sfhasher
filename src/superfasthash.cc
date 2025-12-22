#include "superfasthash.h"

/* By Paul Hsieh (C) 2004, 2005.  Covered under the Paul Hsieh derivative 
   license. See: 
   http://www.azillionmonkeys.com/qed/weblicense.html for license details. */

// Original C code from http://www.azillionmonkeys.com/qed/hash.html
// Modified from an already C++-ified version from here https://github.com/aappleby/smhasher/blob/master/src/SuperFastHash.cpp

inline uint16_t get16bits(const void* p) {
  return *(const uint16_t*)p;
}

uint32_t SuperFastHashA(const char* data, int len) {
  uint32_t hash = 0;
  uint32_t tmp;
  int rem;

  if (len <= 0 || data == NULL || data == nullptr) {
    return 0;
  }

  rem = len & 3;
  len >>= 2;

  /* Main loop */
  for (;len > 0; len--) {
    hash  += get16bits (data);
    tmp    = (get16bits (data+2) << 11) ^ hash;
    hash   = (hash << 16) ^ tmp;
    data  += 2*sizeof (uint16_t);
    hash  += hash >> 11;
  }

  /* Handle end cases */
  switch (rem) {
    case 3:	hash += get16bits (data);
        hash ^= hash << 16;
        hash ^= data[sizeof (uint16_t)] << 18;
        hash += hash >> 11;
        break;
    case 2:	hash += get16bits (data);
        hash ^= hash << 11;
        hash += hash >> 17;
        break;
    case 1: hash += *data;
        hash ^= hash << 10;
        hash += hash >> 1;
  }

  /* Force "avalanching" of final 127 bits */
  hash ^= hash << 3;
  hash += hash >> 5;
  hash ^= hash << 4;
  hash += hash >> 17;
  hash ^= hash << 25;
  hash += hash >> 6;

  return hash;
}

uint32_t SuperFastHashW(const wchar_t* data, int len) {
  uint32_t hash = 0;
  uint32_t tmp;
  int rem;

  if (len <= 0 || data == NULL || data == nullptr) {
    return 0;
  }

  rem = len & 3;
  len >>= 2;

  /* Main loop */
  for (;len > 0; len--) {
    hash  += get16bits (data);
    tmp    = (get16bits (data+2) << 11) ^ hash;
    hash   = (hash << 16) ^ tmp;
    data  += 2*sizeof (uint16_t);
    hash  += hash >> 11;
  }

  /* Handle end cases */
  switch (rem) {
    case 3:	hash += get16bits (data);
        hash ^= hash << 16;
        hash ^= data[sizeof (uint16_t)] << 18;
        hash += hash >> 11;
        break;
    case 2:	hash += get16bits (data);
        hash ^= hash << 11;
        hash += hash >> 17;
        break;
    case 1: hash += *data;
        hash ^= hash << 10;
        hash += hash >> 1;
  }

  /* Force "avalanching" of final 127 bits */
  hash ^= hash << 3;
  hash += hash >> 5;
  hash ^= hash << 4;
  hash += hash >> 17;
  hash ^= hash << 25;
  hash += hash >> 6;

  return hash;
}

void SuperFastHashSeededA(const void* key, int len, uint32_t /*seed*/, void* out) {
  *(uint32_t*)out = SuperFastHashA((const char*)key, len);
}

void SuperFastHashSeededW(const void* key, int len, uint32_t /*seed*/, void* out) {
  *(uint32_t*)out = SuperFastHashW((const wchar_t*)key, len);
}
