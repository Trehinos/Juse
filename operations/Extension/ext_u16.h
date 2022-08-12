#pragma once

#include "../../Juse/operations.h"
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
        static S<Operation> Set16;
        static S<Operation> CopyFrom16;
        static S<Operation> CopyTo16;
        static S<Operation> Copy16;
        static S<Operation> Push16;
        static S<Operation> Pop16;
        static S<Operation> CastTo16;
        static S<Operation> CastFrom16;
        static S<Operation> Copy16If;
        static S<Operation> Add16;
        static S<Operation> Substract16;
        static S<Operation> Multiply16;
        static S<Operation> Divide16;
        static S<Operation> Modulo16;
        static S<Operation> Random16;
        static S<Operation> Compare16;
        static S<Operation> Write16;
        static S<Operation> Read16;
        static S<Operation> WriteUtf16;
    }
}
}