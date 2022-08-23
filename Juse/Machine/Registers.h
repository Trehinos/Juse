#pragma once

#include "../types.h"

namespace Juse
{

    struct Registers
    {
        static const U8 SCALAR_RESULT = 0x00;
        static const U8 SCALAR_ARG = 0x01;
        static const Array<U8, 14> SCALAR_ARGS;

        static const Array<U8, 15> FUNC_ARGS;
        static const U8 FUNC_RESULT = 0x1F;

        static const Array<U8, 16> VEC_RESULTS;
        static const Array<U8, 64> VEC_ARGS;

        struct R64
        {
            static const U8 MODEL_THIS = 0x70;
            static const U8 ARRAY_PTR = 0x71;
            static const U8 STRING_PTR = 0x72;
            static const U8 OBJECT_PTR = 0x73;
        };
        struct R32
        {
            static const U8 MODEL_SELF = 0x70;
            static const U8 STACK_SIZE_BEFORE = 0x71;
            static const U8 STACK_SIZE_AFTER = 0x72;
            static const U8 STACK_SIZE = 0x73;
        };
        struct R24
        {

        };
        struct R16
        {
            static const U8 MODEL_THIS = 0x70;
            static const U8 ARRAY_PTR = 0x71;
            static const U8 STRING_PTR = 0x72;
            static const U8 OBJECT_PTR = 0x73;
        };
        struct R8
        {
            static const U8 STACK_TOP = 0x70;
        };

        GeneralRegisters<U8> bytes;
        GeneralRegisters<U16> words;
        GeneralRegisters<A24> addrs;
        GeneralRegisters<U32> quads;
        GeneralRegisters<U64> longs;
        CompareFlags compareFlags;

        static CompareFlags createFlags() {
            CompareFlags flags {};
            flags[CompareFlag::EQ] = false;
            flags[CompareFlag::GT] = false;
            flags[CompareFlag::LW] = false;
            flags[CompareFlag::GE] = false;
            flags[CompareFlag::LE] = false;
            flags[CompareFlag::NE] = false;
            flags[CompareFlag::Z0] = false;
            flags[CompareFlag::CR] = false;
            flags[CompareFlag::OF] = false;
            flags[CompareFlag::SN] = false;
            flags[CompareFlag::ERR] = false;
            return flags;
        }
    };

}