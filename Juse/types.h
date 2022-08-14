#pragma once

/*
* This file is part of the project "Juse".
* This file is under the MIT licence. Please read LICENSE file to know your rights.
*
* Models/types.h
*   This file defines a lot of shortcuts for all types used by Juse.
*   It is included in every implementation files.
*
*/

#include <any>
#include <optional>
#include <array>
#include <chrono>
#include <concepts>
#include <cstdint>
#include <cuchar>
#include <functional>
#include <map>
#include <memory>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

/*
* U128 to support U64 overflows
* It defines a std::uint128_t type.
*/
#ifdef _MSC_VER
#include "U128.h"
#endif

namespace Juse {
    /* Usual types */
    template <class T>
    using UPtr = std::unique_ptr<T>;
    template <class T>
    using SPtr = std::shared_ptr<T>;
    template <typename T>
    using Ref = std::reference_wrapper<T>;
    template <typename T>
    using Opt = std::optional<T>;
    template <typename T>
    using Wrap = Opt<Ref<T>>;

    template <typename T, typename U>
    using Pair = std::pair<T, U>;
    template <typename T, typename U>
    using Map = std::map<T, U>;
    template <typename T>
    using Vector = std::vector<T>;
    template <typename T, size_t s>
    using Array = std::array<T, s>;

    /*
    * Utility concept for some templates.
    */
    template <typename T, typename... U>
    concept IsAnyOf = (std::same_as<T, U> || ...);

    /*
    * SIZESXX where XX is a size in bits ; maps with size in bytes.
    */
    const size_t SIZE8 = 1;
    const size_t SIZE16 = 2;
    const size_t SIZE32 = 4;
    const size_t SIZE64 = 8;
    const size_t SIZE128 = 16;

    /*
    * Inverse of SIZESXX : returns the size in bits.
    */
    const Map<size_t, size_t> sizes = {
        { SIZE8, 8 }, { SIZE16, 16 }, { SIZE32, 32 }, { SIZE64, 64 }, { SIZE128, 128 }
    };

    /*
    * Words of different sizes :
    * Base types of registers of Juse::Machine.
    */
    using U8 = std::uint8_t;
    using U16 = std::uint16_t;
    using U32 = std::uint32_t;
    using U64 = std::uint64_t;
    using U128 = std::uint128_t;
    template <typename T>
    concept IsWord = IsAnyOf<T, U8, U16, U32, U64, U128>;

    const U8 MASK_BOTTOM8 = 0xFF;
    const U16 MASK_16TOP8 = 0xFF00;
    const U16 MASK_BOTTOM16 = 0xFFFF;
    const U32 MASK_32TOP16 = 0xFFFF0000;
    const U32 MASK_BOTTOM32 = 0xFFFFFFFF;
    const U64 MASK_64TOP32 = 0xFFFFFFFF00000000;

    /*
    * Char types used in Juse.
    */
    using CH8 = char;
    using CH16 = char16_t;
    using CH32 = char32_t;
    template <typename T>
    concept IsChar = IsAnyOf<T, CH8, CH16, CH32>;
    template <IsChar T>
    using StringStream = std::basic_stringstream<T>;
    template <IsChar T = CH8>
    using String = std::basic_stringstream<T>;
    using SS8 = std::stringstream;
    using SS16 = StringStream<CH16>;
    using SS32 = StringStream<CH32>;
    using S8 = std::string;
    using S16 = String<CH16>;
    using S32 = String<CH32>;

    /*
    * Signed integer types
    */
    using I8 = std::int8_t;
    using I16 = std::int16_t;
    using I32 = std::int32_t;
    using I64 = std::int64_t;

    // ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    template <class T>
    const auto makeU = std::make_unique<T>;
    template <class T>
    const auto makeS = std::make_shared<T>;
    template <class T>
    inline UPtr<T> unique(T& t) { return std::make_unique<T>(t); }
    template <class T>
    inline SPtr<T> share(T& t) { return std::make_shared<T>(t); }
    template <typename T>
    inline Ref<T> ref(T& v) { return std::ref<T>(v); }
    template <typename T>
    inline Opt<T> opt(T v) { return std::make_optional<T>(v); }
    template <typename T>
    inline Wrap<T> wrap(T& v) { return Wrap<T>{opt<Ref<T>>(ref<T>(v))}; }

    template <typename T>
    inline T optval(Opt<T> v) { return v.has_value() ? v.value() : T{}; }
    template <typename T>
    inline T& rawref(Ref<T> v) { return v.get(); }
    template <typename T>
    inline T& rawwrap(Wrap<T> v)
    {
        return rawref<T>(
            v.has_value()
            ? optval<Ref<T>>(v)
            : toref<T>(T{})
            );
    }

    template <typename T, typename U>
    inline Pair<T, U> pair(T v1, U v2) { return std::make_pair(v1, v2); }

    template <int size>
    using ByteArray = Array<U8, size>;
    using ByteSet = Vector<U8>;
    using Stack = std::stack<U8>;

    template <typename T>
    using Collection = Vector<Ref<T>>;

    template <typename T, typename U>
    using RefHeap = Map<T, Ref<U>>;

    template <typename T, typename U>
    using HeapMap = Map<T, SPtr<U>>;

    const size_t SEGMENT_SIZE = 1 << 16; // 64 KiB
    using Segment = ByteArray<SEGMENT_SIZE>;
    using Pool = HeapMap<U32, Segment>;
    using Memory = HeapMap<U16, Pool>;

    template <IsWord Type>
    using GeneralRegisters = Array<Type, 256>;

    class Operation;
    using OperationMap = HeapMap<U16, Operation>;

    struct Address {
        U16 pool;
        U32 segment;
        U16 addr16;

        U64 compose() { return (U64(pool) << 48) + (U64(segment) << 16) + addr16; }

        static U64 with(U16 p, U32 s, U16 d) { return Address{ p, s, d }.compose(); }

        static Address from(U64 address)
        {
            U16 pool = U16((address & 0xFFFF000000000000) >> 48);
            U32 segment = U32((address & 0x0000FFFFFFFF0000) >> 16);
            U16 addr16 = U16(address & 0x000000000000FFFF);

            return Address{ pool, segment, addr16 };
        }
    };

    struct Instruction {
        ByteSet data;

        U16 identifier() { return U16(data[0] << 8) + (data[1]); }

        U64 argument(size_t offset, size_t size)
        {
            U64 buffer = 0;
            for (size_t i = SIZE16 + offset; i < SIZE16 + offset + size; i++) {
                buffer <<= 8;
                buffer += data[i];
            }
            return buffer;
        }
    };

    class Cpu;

    // Comparaison flags for arithmetic and logic Cpu operations.
    enum class CompareFlag {
        EQ = 0,
        GT = 1,
        LW = 2,
        GE = 3,
        LE = 4,
        NE = 5,
        Z0 = 6,
        CR = 7,
        OF = 8,
        SN = 9, // signe negative
        ERR = 255
    };
    using CompareFlags = Map<CompareFlag, bool>;

    class Machine;

    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    using Duration = TimePoint::duration;

    struct OperationArgument;
    using OperationArguments = std::vector<OperationArgument>;
    using FunctionType = void(Machine&, Cpu&, OperationArguments);
    using OperationFunction = std::function<FunctionType>;

} // namespace Juses