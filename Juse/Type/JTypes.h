#pragma once

#include "../utility.h"
#include "../../operations/Standard/move.h"

namespace Juse {

    namespace Types {
        struct Model {
            S8 typeName;
            inline virtual ByteSet toSet()
            {
                return ByteSet{};
            }
            Model(S8 typeName = "") : typeName(typeName) {}
        };

        const struct Nil : public Model {
            Nil() : Model("nil") {}
        } nil;

        struct Null : public virtual Model {
            Null() : Model("null") {}
            inline ByteSet toSet()
            {
                return ByteSet{ 0 };
            }
        };

        struct Data : public Model {
            Address address;
            Data(S8 n, Address a = {}) : Model(n), address(a) {}
            Data(Address a = {}) : Data{ "data", a } {}
            Address addr(U16 offset = 0)
            {
                Address a{ address };
                Utility::MachineMemory::forward(a, offset);
                return a;
            }
            inline bool isEmpty() { return toSet().empty(); }
            inline bool isNull() { return toSet().size() == 1 && toSet().at(0) == 0; }
        };

        struct Boolean : public virtual Data
        {
            bool data;
            inline ByteSet toSet()
            {
                if (data) {
                    return ByteSet{ 1 };
                }
                return ByteSet{ 0 };
            }
            Boolean(bool b = false) : Data{ "boolean" }, data(b) {}
        };

        template <IsWord T>
        struct Word : public virtual Data
        {
            T data;
            inline ByteSet toSet()
            {
                return word2set<T>(data);
            }
            Word(T data) : Data{ "word" + sizeof(T) }, data{ data } {}
            Word(ByteSet s) : Data{ "word" + sizeof(T) }, data{}
            {
                data = set2word(s);
            }
        };

        using Word8 = Word<U8>;
        using Word16 = Word<U16>;
        using Word32 = Word<U32>;
        using Word64 = Word<U64>;

        template <IsChar T, IsWord U>
        struct StringModel : public virtual Data
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
            StringModel(String<T> str) : Data{ "string" + sizeof(T) }, data(str) {}
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
            Integer(T data) : Data{ "integer" + sizeof(U) }, data{ data } {}
            Integer(ByteSet s) : Data{ "integer" + sizeof(U) }, data{}
            {
                data = T(set2word(s));
            }
        };

        using Integer8 = Integer<I8, U8>;
        using Integer16 = Integer<I16, U16>;
        using Integer32 = Integer<I32, U32>;
        using Integer64 = Integer<I64, U64>;

        struct ArrayModel : public virtual Data
        {
            Collection<Data> values;
            ArrayModel(U16 maxSize) : Data{ "array" }, values{}, maxSize(maxSize) {};
            void add(Data& m)
            {
                values.push_back(m);
            }
            Data& at(U16 index)
            {
                return values.at(index);
            }
        private:
            U16 maxSize;
        };

        using ModelMap = HeapMap<S8, Data>;

        struct TupleModel : public virtual Data
        {
            ModelMap fields;
            TupleModel(S8 n, ModelMap f = {}) : Data(n), fields(f) {}
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

        struct Parameter {
            Wrap<Data> defaultModel;
            Wrap<Data> valueModel;
        };

        using ParameterList = Collection<Data>;

        struct ModelMethod : public virtual Data
        {
            enum class Visibility {
                PRIVATE, PROTECTED, PUBLIC
            } visibility;
            enum class Scope {
                INSTANCE, STATIC
            } scale;
            Ref<Data> returnModel;
            Collection<Parameter> parameters;
            Vector<Instruction> instructions;
            ModelMethod(
                Visibility v,
                Scope s,
                Ref<Data> r,
                S8 name,
                ParameterList parameters,
                Vector<Instruction> instructions
            );
            ModelMethod(S8 name, Vector<Instruction>& instructions, Wrap<Data> r = std::nullopt, ParameterList p = {})
                : ModelMethod(Visibility::PUBLIC, Scope::INSTANCE, r.value(), name, p, instructions)
            {
            }
            void operator()(Machine& m, Cpu& c, Data* thisModel)
            {

                for (Instruction& i : instructions) {
                    executeInstruction(m, c, i);
                }
            }
            void call(Machine& m, Cpu& c)
            {
                this->operator()(m, c, this);
            }

        private:
            static void executeInstruction(Machine& m, Cpu& c, Instruction& i)
            {
                auto operation = c.findOperation(m, i.identifier());
                (*operation)(m, c, i);
            }
        };

        using MethodMap = HeapMap<S8, ModelMethod>;

        struct Object;

        struct ObjectModel : public virtual TupleModel
        {
            MethodMap methods;
            ObjectModel(S8 n, ModelMap f = {}, MethodMap m = {}) : TupleModel(n, f), methods(m) {}
            void callStatic(S8 methodName)
            {

            }
            virtual SPtr<Object> instantiate(ParameterList parameters)
            {
                SPtr<Object> o{ new Object{ this, {}, fields, parameters } };
                return o;
            }
        };

        struct Object : public virtual TupleModel {
            SPtr<ObjectModel> model;
            SPtr<ModelMethod> method(S8 methodName)
            {
                if (methods().get().contains(methodName)) {
                    return methods().get().at(methodName);
                }
                return nullptr;
            }
            void call(S8 methodName, Machine& m, Cpu& c)
            {
                (*method(methodName))(m, c, this);
            }
            Object(ObjectModel* m, S8 name, ModelMap& fields, ParameterList& p) : TupleModel(name), model(m)
            {

            }
            Object(ObjectModel* m, S8 name, ModelMap& fields) : TupleModel(name), model(m) {}
        private:
            inline Ref<MethodMap> methods()
            {
                return model.get()->methods;
            }
        };

    }
}
