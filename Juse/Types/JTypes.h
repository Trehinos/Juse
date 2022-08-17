#pragma once

#include "../types.h"
#include "../utility.h"

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

        using ModelMap = HeapMap<S8, Model>;

        struct Parameter {
            Wrap<Model> defaultModel;
            Wrap<Model> valueModel;
        };

        using ParameterList = Collection<Model>;

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
    }
}
