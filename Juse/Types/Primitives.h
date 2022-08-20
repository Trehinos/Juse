#pragma once

#include "JTypes.h"
#include "../operations.h"

namespace Juse
{
    namespace Types
    {
        struct Boolean : public virtual Data
        {
            bool data;
            inline ByteSet toSet() { return data ? ByteSet { 1 } : ByteSet { 0 }; }
            Boolean(bool b = false) : Data { "boolean" }, data(b) {}
            inline S8 toString() { return data ? "true" : "false"; }
        };

        template <IsWord T>
        struct Word : public virtual Data
        {
            T data;
            inline ByteSet toSet()
            {
                return word2set<T>(data);
            }
            Word(T data) : Data { "word" + sizeof(T) }, data { data } {}
            Word(ByteSet s) : Data { "word" + sizeof(T) }, data {}
            {
                data = set2word(s);
            }
            inline S8 toString() { return std::to_string(data); }
        };

        using Word8 = Word<U8>;
        using Word16 = Word<U16>;
        using Word32 = Word<U32>;
        using Word64 = Word<U64>;

        struct StringType : public virtual Data {};

        template <IsChar T, IsWord U>
        struct StringModel : public virtual StringType
        {
            String<T> data;
            ByteSet toSet()
            {
                ByteSet set {};
                for (T& c : data) {
                    set.push_back(word2set(U(c)));
                }
                return set;
            }
            StringModel(String<T> str) : StringType { "string" + sizeof(T) }, data(str) {}
            inline S8 toString() { return data; }
        };

        using StringAscii = StringModel<CH8, U8>;
        using StringUtf16 = StringModel<CH16, U16>;
        using StringUtf32 = StringModel<CH32, U32>;

        template <IsInteger T, IsWord U>
        struct Integer : public virtual Data
        {
            T data;
            inline ByteSet toSet()
            {
                return word2set<U>(U(data));
            }
            Integer(T data) : Data { "integer" + sizeof(U) }, data { data } {}
            Integer(ByteSet s) : Data { "integer" + sizeof(U) }, data {}
            {
                data = T(set2word(s));
            }
            inline S8 toString() { return std::to_string(data); }
        };

        using Integer8 = Integer<I8, U8>;
        using Integer16 = Integer<I16, U16>;
        using Integer32 = Integer<I32, U32>;
        using Integer64 = Integer<I64, U64>;

        struct ArrayModel : public virtual Composed
        {
            Collection<Data> values;
            ArrayModel(U16 maxSize = 0) : Composed { "array", "data" }, values {}, maxSize(maxSize) {};
            inline void add(Data& m) { values.push_back(m); }
            inline Data& at(U16 index) { return values.at(index); }
            ByteSet toSet()
            {
                ByteSet set {};
                for (Data& v : values) {
                    for (U8 byte : v.toSet()) {
                        set.push_back(byte);
                    }
                }
                return set;
            }
            Vector<S8> toStringArray()
            {
                Vector<S8> v;
                for (auto& s : values) {
                    v.push_back(s.get().toString());
                }
                return v;
            }
        private:
            U16 maxSize;
        };
    }
}