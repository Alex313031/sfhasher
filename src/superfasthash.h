#ifndef BASE_HASH_SUPERFASTHASH_H_
#define BASE_HASH_SUPERFASTHASH_H_

#include "framework.h"

#include <random>

uint32_t SuperFastHashA(const char* data, int len);

void SuperFastHashSeededA(const void* key, int len, uint32_t /*seed*/, void* out);

uint32_t SuperFastHashW(const wchar_t* data, int len);

void SuperFastHashSeededW(const void* key, int len, uint32_t /*seed*/, void* out);

#endif
