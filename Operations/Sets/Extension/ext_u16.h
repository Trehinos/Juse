#pragma once

#include "../../operations.h"
#include "u_operations.h"

namespace Juse {
template <IsWord T>
CompareFlags compare<U16>(T a, T b);

template <IsWord T>
T random<U32>(T, T);

template <IsWord T, IsWord U>
void calculate<U16, U32>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&, U, bool);

namespace Operations {
    namespace Unsigned {
        template <IsWord T>
        void set<U16>(GeneralRegisters<T>&, OperationArguments&);

        template <IsWord T>
        void copy<U16>(GeneralRegisters<T>&, OperationArguments&);

        template <IsWord T>
        void push<U16>(Cpu&, GeneralRegisters<T>&, OperationArguments&);

        template <IsWord T>
        void pop<U16>(Cpu&, GeneralRegisters<T>&, OperationArguments&);

        template <IsWord T, IsWord U>
        void add<U16, U32>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

        template <IsWord T, IsWord U>
        void substract<U16, U32>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

        template <IsWord T, IsWord U>
        void multiply<U16, U32>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

        template <IsWord T, IsWord U>
        void divide<U16, U32>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

        template <IsWord T, IsWord U>
        void modulo<U16, U32>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

        template <IsWord T>
        void compare<U16>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);
    }

    /* 14xx-17xx */
    namespace ExtU16 {
        void init();
        void add(OperationMap& operations);
        S<Operation> Set16;
        S<Operation> CopyFrom16;
        S<Operation> CopyTo16;
        S<Operation> Copy16;
        S<Operation> Push16;
        S<Operation> Pop16;
        S<Operation> CastTo16;
        S<Operation> CastFrom16;
        S<Operation> Copy16If;
        S<Operation> Add16;
        S<Operation> Substract16;
        S<Operation> Multiply16;
        S<Operation> Divide16;
        S<Operation> Modulo16;
        S<Operation> Random16;
        S<Operation> Compare16;
        S<Operation> Write16;
        S<Operation> Read16;
        S<Operation> WriteUtf16;
    }
}
}