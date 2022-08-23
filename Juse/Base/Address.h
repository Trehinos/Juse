#pragma once

#include "Unsigned.h"

namespace Juse
{


    struct A24
    {
        U8 top;
        U16 bottom;
        A24(U32 data = 0) : top { U8((data & MASK_24TOP8) >> 16) }, bottom { U16(data & MASK_BOTTOM16) } { }
        inline operator U32() { return U32(U32(*this)); }
        inline A24 operator+(U32 operand) { return A24 { U32(*this) + operand }; }
        inline A24 operator+(A24 operand) { return *this + U32(operand); }
        inline A24& operator+=(U32 operand) { *this = A24 { U32(*this) + operand }; return *this; }
        inline A24& operator+=(A24 operand) { *this = A24 { *this + U32(operand) }; return *this; }
        inline A24& operator++() { *this = A24 { U32(*this) + 1 }; return *this; }
        inline A24 operator++(int) { A24 old = *this; operator++(); return old; }
    };

    template <typename T>
    concept IsWordOrAddr = IsAnyOf<T, U8, U16, U32, U64, U128, A24>;

    using PoolId = U16;
    using SegmentId = A24;
    using DataId = A24;

    static const U32 SEGMENT_SIZE = U32(1 << 24);

    struct Address
    {
        PoolId pool;
        SegmentId segment;
        DataId address;
        Address(PoolId p = {}, SegmentId s = {}, DataId a = {}) : pool(p), segment(s), address(a) { }
        static Address from(U64 a) {
            return Address {
                PoolId((a & mask<U64, 8, 6>()) >> 48),
                SegmentId{U32(a & mask<U64, 6, 3>()) >> 24},
                DataId{U32(a & mask<U64, 3>()) }
            };
        }
        U64 compose() {
            return (U64(pool) << 48) & (U64(segment) << 24) & U64(address);
        }
        inline operator U64() { return compose(); }
        static U64 with(PoolId p, SegmentId s, DataId a) {
            return U64(Address { p,s,a });
        }
        Address& operator+=(const U32& steps) {
            U32 tmpAddr = U32(address);
            tmpAddr += steps;
            if (tmpAddr >= SEGMENT_SIZE) {
                address = DataId(tmpAddr % SEGMENT_SIZE);
                if (segment == SEGMENT_SIZE) {
                    segment = 0;
                    if (pool == UINT16_MAX) {
                        pool = 0;
                    } else {
                        pool++;
                    }
                } else {
                    segment++;
                }
            }
            return *this;
        }
        Address& operator++(int) {
            return this->operator+=(1);
        }
    };
}