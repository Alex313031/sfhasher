#include "hash.h"

std::wstring SimpleW(const std::wstring& input) {
    std::wstring output;
    output.reserve(input.size());

    // Derive a seed from the input (simple and deterministic)
    uint32_t seed = 0;
    for (wchar_t wchar : input) {
        seed = seed * 131 + static_cast<uint32_t>(wchar);
    }

    std::mersenne_twister_engine<uint32_t, 32, 624, 397, 31,
                                 0x9908B0DF, 11,
                                 0xFFFFFFFF, 7,
                                 0x9D2C5680, 15,
                                 0xEFC60000, 18,
                                 1812433253> mt(seed);

    uint32_t state = mt(); // randomized initial state

    for (size_t i = 0; i < input.size(); ++i)
    {
        // Mix current state with input character
        state ^= static_cast<uint32_t>(input[i]);
        state = (state << 5) | (state >> 27); // rotate
        state += static_cast<uint32_t>(i * 17);

        // Map into A–Z, 0–9
        wchar_t ch = kOutChars[state % kOutCharsSize];
        output.push_back(ch);
    }

    return output;
}
