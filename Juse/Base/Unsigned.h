#pragma once

#include <cstdint>
#include "General.h"

#ifdef _MSC_VER
#include "../U128.h"
#endif

namespace Juse
{
    using U8 = std::uint8_t;
    using U16 = std::uint16_t;
    using U32 = std::uint32_t;
    using U64 = std::uint64_t;
    using U128 = std::uint128_t;

    template <typename T>
    concept IsWord = IsAnyOf<T, U8, U16, U32, U64, U128>;

    template <size_t size>
    using ByteArray = Array<U8, size>;
    using ByteSet = Vector<U8>;

    template <IsWord T, size_t FILLF = sizeof(T), size_t FILL0 = 0>
    static const T mask() {
        T buf {};
        for (size_t i = FILLF - 1; i >= FILL0; i--) {
            buf |= (0xFF << (8 * i));
        }
        return buf;
    }

    const U8 MASK_BOTTOM8 = mask<U8>();
    const U16 MASK_16TOP8 = mask<U16, 2, 1>();
    const U16 MASK_BOTTOM16 = mask<U16>();
    const U32 MASK_24TOP8 = mask<U32, 3, 2>();
    const U32 MASK_24TOP16 = mask<U32, 3, 1>();
    const U32 MASK_32TOP16 = mask<U32, 4, 2>();
    const U32 MASK_BOTTOM32 = mask<U32>();
    const U64 MASK_64TOP32 = mask<U64, 8, 4>();

}