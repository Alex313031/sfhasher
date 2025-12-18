#ifndef SFHASHER_HASH_H_
#define SFHASHER_HASH_H_

#include "framework.h"

#include <random>

static constexpr wchar_t kOutChars[] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
static constexpr size_t  kOutCharsSize = 36;

std::wstring SimpleW(const std::wstring& input);

#endif // SFHASHER_HASH_H_
