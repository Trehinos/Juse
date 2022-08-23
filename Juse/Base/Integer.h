#pragma once

#include "General.h"

namespace Juse
{
    using I8 = std::int8_t;
    using I16 = std::int16_t;
    using I32 = std::int32_t;
    using I64 = std::int64_t;
    template <typename T>
    concept IsInteger = IsAnyOf<T, I8, I16, I32, I64>;
}