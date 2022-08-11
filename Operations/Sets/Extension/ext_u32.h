#pragma once

#include "../../operations.h"
#include "u_operations.h"

namespace Juse {
template <IsWord T>
CompareFlags compare<U32>(T a, T b);

template <IsWord T>
T random<U64>(T, T);

template <IsWord T, IsWord U>
void calculate<U32, U64>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&, U, bool);

namespace Operations {
    namespace Unsigned {
        template <IsWord T>
        void set<U32>(GeneralRegisters<T>&, OperationArguments&);

        template <IsWord T>
        void copy<U32>(GeneralRegisters<T>&, OperationArguments&);

        template <IsWord T>
        void push<U32>(Cpu&, GeneralRegisters<T>&, OperationArguments&);

        template <IsWord T>
        void pop<U32>(Cpu&, GeneralRegisters<T>&, OperationArguments&);

        template <IsWord T, IsWord U>
        void add<U32, U64>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

        template <IsWord T, IsWord U>
        void substract<U32, U64>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

        template <IsWord T, IsWord U>
        void multiply<U32, U64>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

        template <IsWord T, IsWord U>
        void divide<U32, U64>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

        template <IsWord T, IsWord U>
        void modulo<U32, U64>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

        template <IsWord T>
        void compare<U32>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);
    }

    /* 18xx-1Bxx */
    namespace ExtU32 {
        void init();
        void add(OperationMap& operations);
        S<Operation> Set32;
        S<Operation> CopyFrom32;
        S<Operation> CopyTo32;
        S<Operation> Copy32;
        S<Operation> Push32;
        S<Operation> Pop32;
        S<Operation> CastTo32;
        S<Operation> CastFrom32;
        S<Operation> Copy32If;
        S<Operation> Add32;
        S<Operation> Substract32;
        S<Operation> Multiply32;
        S<Operation> Divide32;
        S<Operation> Modulo32;
        S<Operation> Random32;
        S<Operation> Compare32;
        S<Operation> Write32;
        S<Operation> Read32;
        S<Operation> WriteUtf32;
    }
}
}