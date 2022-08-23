#pragma once

#include "../types.h"
#include "../utility.h"

namespace Juse
{
    namespace Types
    {
        struct Model
        {
            S8 name;
            inline virtual S8 toString() { return "{" + name + "}"; }
            inline virtual ByteSet toSet() { return ByteSet {}; }
            inline virtual void fromSet(ByteSet)
            {
            }
            inline virtual size_t size() { return toSet().size(); }
            Model(S8 name = "") : name(name) { }
        };

        using ModelMap = HeapMap<S8, Model>;
        using ParameterList = Collection<Model>;

        const struct Nil : public Model
        {
            Nil() : Model("nil") { }
            inline S8 toString() { return ""; }
        } nil;

        struct Null : public virtual Model
        {
            Null() : Model("null") { }
            inline ByteSet toSet() { return ByteSet { 0 }; }
            inline S8 toString() { return ""; }
        };

        struct Data : public virtual Model
        {
            Address address;
            Data(S8 n, Address a = {}) : Model(n), address(a) { }
            Data(Address a = {}) : Data { "data", a } { }
            Address addr(size_t offset = 0)
            {
                Address a { address };
                a += offset;
                return a;
            }
            inline S8 toString() { return "{@" + std::to_string(address.compose()) + ":" + name + "}"; }
            inline virtual bool isEmpty() { return toSet().empty(); }
            inline virtual bool isNull() { return isEmpty() || toSet().size() == 1 && toSet().at(0) == 0; }
            inline virtual bool isScalar() { return true; }
            inline virtual Ref<Data> instantiate(Data& d) { return ref(d); };
        };

        struct NullableData : public Data
        {
            NullableData(S8 n = "", Address a = {}) : Data { "?" + n, a } { }
            inline void setToNull() { ByteSet set; set.assign(size(), 0); fromSet(set); }
        };

        struct Composed : public virtual Data
        {
            Composed(S8 container, S8 content, Address a = {}) : Data { container + "<" + content + ">", a } { }
            inline S8 toString() { auto s = toStringArray(); return "[" + Utility::Strings::join(s, ", ") + "]"; }
            inline virtual U16 count() { return 0; }
            inline virtual Vector<S8> toStringArray() { return {}; }
        };

    }
}
