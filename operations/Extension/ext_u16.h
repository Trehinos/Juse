#pragma once

#include "../../Juse/operations.h"
#include "u_operations.h"

namespace Juse
{
    template <IsWord T>
    CompareFlags compare<U16>(T a, T b);

    template <IsWord T>
    T random<U32>(T, T);

    template <IsWord T, IsWord U>
    void calculate<U16, U32>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&, U, bool);

    namespace Operations
    {
        namespace Unsigned
        {
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
        namespace ExtU16
        {
            void init();
            void add(OperationMap& operations);
            static SPtr<Operation> Set16;
            static SPtr<Operation> CopyFrom16;
            static SPtr<Operation> CopyTo16;
            static SPtr<Operation> Copy16;
            static SPtr<Operation> Push16;
            static SPtr<Operation> Pop16;
            static SPtr<Operation> CastTo16;
            static SPtr<Operation> CastFrom16;
            static SPtr<Operation> Copy16If;
            static SPtr<Operation> Add16;
            static SPtr<Operation> Substract16;
            static SPtr<Operation> Multiply16;
            static SPtr<Operation> Divide16;
            static SPtr<Operation> Modulo16;
            static SPtr<Operation> Random16;
            static SPtr<Operation> Compare16;
            static SPtr<Operation> Write16;
            static SPtr<Operation> Read16;
            static SPtr<Operation> WriteUtf16;
        }
    }
}