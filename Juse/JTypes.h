#pragma once

#include "types.h"
#include "Machine/Machine.h"

namespace Juse {
    namespace Types {
        struct Type {
            inline virtual ByteSet toSet()
            {
                return ByteSet{};
            }
            Type() {}
        };

        using ModelsHeap = HeapMap<S8, Type>;

        template <IsWord T>
        struct Word : public virtual Type
        {
            T data;
            inline ByteSet toSet()
            {
                return word2set<T>(data);
            }
            Word(T data) : Type{}, data{ data } {}
            Word(ByteSet s) : Type{}, data{}
            {
                data = set2word(s);
            }
        };

        using Word8 = Word<U8>;
        using Word16 = Word<U16>;
        using Word32 = Word<U32>;
        using Word64 = Word<U64>;

        template <IsChar T, IsWord U>
        struct StringType : public virtual Type
        {
            String<T> data;
            inline ByteSet toSet()
            {
                ByteSet set{};
                for (T& c : data) {
                    set.push_back(word2set(U(c));
                }
                return set;
            }
            String(String<T> str) : Type{}, data(str) {}
        };

        using StringAscii = StringType<CH8, U8>;
        using StringUtf16 = StringType<CH16, U16>;
        using StringUtf32 = StringType<CH32, U32>;

        struct Object : public virtual Type
        {
        };

    }
}
