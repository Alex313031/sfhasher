#ifndef SFHASHER_HASH_H_
#define SFHASHER_HASH_H_

#include "framework.h"

#include "superfasthash.h"
#include "xxhash32.h"

static constexpr wchar_t kOutChars[] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
static constexpr size_t  kOutCharsSize = 36;

std::wstring SimpleMersenneW(const std::wstring& input);

std::wstring SFHashW(const std::wstring& input);

std::wstring XXHashW(const std::wstring& input);

#endif // SFHASHER_HASH_H_
