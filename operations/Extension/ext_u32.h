#pragma once

#include "../../Juse/operations.h"
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
        static S<Operation> Set32;
        static S<Operation> CopyFrom32;
        static S<Operation> CopyTo32;
        static S<Operation> Copy32;
        static S<Operation> Push32;
        static S<Operation> Pop32;
        static S<Operation> CastTo32;
        static S<Operation> CastFrom32;
        static S<Operation> Copy32If;
        static S<Operation> Add32;
        static S<Operation> Substract32;
        static S<Operation> Multiply32;
        static S<Operation> Divide32;
        static S<Operation> Modulo32;
        static S<Operation> Random32;
        static S<Operation> Compare32;
        static S<Operation> Write32;
        static S<Operation> Read32;
        static S<Operation> WriteUtf32;
    }
}
}