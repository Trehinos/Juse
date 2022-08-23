#pragma once

#include "../../Juse/operations.h"
#include "u_operations.h"

namespace Juse
{
    template <IsWord T>
    CompareFlags compare<U8>(T a, T b);

    template <IsWord T>
    T random<U16>(T, T);

    template <IsWord T, IsWord U>
    void calculate<U8, U16>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&, U, bool);

    namespace Operations
    {
        namespace Unsigned
        {
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
        namespace ExtU8
        {
            void init();
            void add(OperationMap& operations);
            static SPtr<Operation> Set8;
            static SPtr<Operation> CopyFrom8;
            static SPtr<Operation> CopyTo8;
            static SPtr<Operation> Copy8;
            static SPtr<Operation> Push8;
            static SPtr<Operation> Pop8;
            static SPtr<Operation> Copy8If;
            static SPtr<Operation> Add8;
            static SPtr<Operation> Substract8;
            static SPtr<Operation> Multiply8;
            static SPtr<Operation> Divide8;
            static SPtr<Operation> Modulo8;
            static SPtr<Operation> Random8;
            static SPtr<Operation> Compare8;
            static SPtr<Operation> Write8;
            static SPtr<Operation> Read8;
            static SPtr<Operation> WriteAscii;
        }
    }
}