#pragma once

#include "types.h"
#include "Machine/Machine.h"

namespace Juse {
    namespace Types {
        struct Model {
            S8 typeName;
            inline virtual ByteSet toSet()
            {
                return ByteSet{};
            }
            Model(S8 typeName) : typeName(typeName) {}
        };

        using ModelMap = HeapMap<S8, Model>;

        template <IsWord T>
        struct Word : public virtual Model
        {
            T data;
            inline ByteSet toSet()
            {
                return word2set<T>(data);
            }
            Word(T data) : Model{ "word" + sizeof(T) }, data{ data } {}
            Word(ByteSet s) : Model{ "word" + sizeof(T) }, data{}
            {
                data = set2word(s);
            }
        };

        using Word8 = Word<U8>;
        using Word16 = Word<U16>;
        using Word32 = Word<U32>;
        using Word64 = Word<U64>;

        template <IsChar T, IsWord U>
        struct StringModel : public virtual Model
        {
            String<T> data;
            inline ByteSet toSet()
            {
                ByteSet set{};
                for (T& c : data) {
                    set.push_back(word2set(U(c)));
                }
                return set;
            }
            StringModel(String<T> str) : Model{ "string" + sizeof(T) }, data(str) {}
        };

        using StringAscii = StringModel<CH8, U8>;
        using StringUtf16 = StringModel<CH16, U16>;
        using StringUtf32 = StringModel<CH32, U32>;

        struct UserType : public virtual Model
        {
            ModelMap fields;
            // TODO
        };

    }
}
