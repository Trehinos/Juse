#pragma once

#include "../../operations.h"
#include "u_operations.h"

namespace Juse {
template <IsWord T>
CompareFlags compare<U8>(T a, T b);

template <IsWord T>
T random<U16>(T, T);

template <IsWord T, IsWord U>
void calculate<U8, U16>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&, U, bool);

namespace Operations {
    namespace Unsigned {
        template <IsWord T>
        void set<U8>(GeneralRegisters<T>&, OperationArguments&);

        template <IsWord T>
        void copy<U8>(GeneralRegisters<T>&, OperationArguments&);

        template <IsWord T>
        void push<U8>(Cpu&, GeneralRegisters<T>&, OperationArguments&);

        template <IsWord T>
        void pop<U8>(Cpu&, GeneralRegisters<T>&, OperationArguments&);

        template <IsWord T, IsWord U>
        void add<U8, U16>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

        template <IsWord T, IsWord U>
        void substract<U8, U16>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

        template <IsWord T, IsWord U>
        void multiply<U8, U16>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

        template <IsWord T, IsWord U>
        void divide<U8, U16>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

        template <IsWord T, IsWord U>
        void modulo<U8, U16>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

        template <IsWord T>
        void compare<U8>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);
    }

    /* 10xx-13xx */
    namespace ExtU8 {
        void init();
        void add(OperationMap& operations);
        S<Operation> Set8;
        S<Operation> CopyFrom8;
        S<Operation> CopyTo8;
        S<Operation> Copy8;
        S<Operation> Push8;
        S<Operation> Pop8;
        S<Operation> Copy8If;
        S<Operation> Add8;
        S<Operation> Substract8;
        S<Operation> Multiply8;
        S<Operation> Divide8;
        S<Operation> Modulo8;
        S<Operation> Random8;
        S<Operation> Compare8;
        S<Operation> Write8;
        S<Operation> Read8;
        S<Operation> WriteAscii;
    }
}
}