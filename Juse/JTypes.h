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
            ByteSet toSet()
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

        template <IsInteger T, IsWord U>
        struct Integer : public virtual Model
        {
            T data;
            inline ByteSet toSet()
            {
                return word2set<U>(U(data));
            }
            Integer(T data) : Model{ "integer" + sizeof(U) }, data{ data } {}
            Integer(ByteSet s) : Model{ "integer" + sizeof(U) }, data{}
            {
                data = T(set2word(s));
            }
        };

        using Integer8 = Integer<I8, U8>;
        using Integer16 = Integer<I16, U16>;
        using Integer32 = Integer<I32, U32>;
        using Integer64 = Integer<I64, U64>;

        struct ArrayModel : public virtual Model
        {
            Collection<Model> values;
            ArrayModel(U16 maxSize) : Model{"array"}, values{}, maxSize(maxSize) {};
            void add(Model& m)
            {
                values.push_back(m);
            }
            Model& at(U16 index)
            {
                return values.at(index);
            }
        private:
            U16 maxSize;
        };

        struct StructModel : public virtual Model
        {
            ModelMap fields;
            StructModel(S8 n, ModelMap f = {}) : Model(n), fields(f) {}
            ByteSet toSet()
            {
                ByteSet output{};
                for (auto& [fieldName, field] : fields) {
                    for (U8 byte : field.get()->toSet()) {
                        output.push_back(byte);
                    }
                }
                return output;
            }
        };

    }
}
