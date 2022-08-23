#pragma once

#include <cuchar>
#include <sstream>

#include "General.h"

namespace Juse
{
    using CH8 = char;
    using CH16 = char16_t;
    using CH32 = char32_t;
    template <typename T>
    concept IsChar = IsAnyOf<T, CH8, CH16, CH32>;
    template <IsChar T>
    using StringStream = std::basic_stringstream<T>;
    template <IsChar T = CH8>
    using String = std::basic_string<T>;
    using SS8 = std::stringstream;
    using SS16 = StringStream<CH16>;
    using SS32 = StringStream<CH32>;
    using S8 = std::string;
    using S16 = String<CH16>;
    using S32 = String<CH32>;
}
