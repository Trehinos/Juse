#pragma once

#include "../../Juse/operations.h"
#include "u_operations.h"

namespace Juse {
    template <IsWord T>
    CompareFlags compare<U64>(T a, T b);

    template <IsWord T>
    T random<U128>(T, T);

    template <IsWord T, IsWord U>
    void calculate<U64, U128>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&, U, bool);

    namespace Operations {
        namespace Unsigned {
            template <IsWord T>
            void set<U64>(GeneralRegisters<T>&, OperationArguments&);

            template <IsWord T>
            void copy<U64>(GeneralRegisters<T>&, OperationArguments&);

            template <IsWord T>
            void push<U64>(Cpu&, GeneralRegisters<T>&, OperationArguments&);

            template <IsWord T>
            void pop<U64>(Cpu&, GeneralRegisters<T>&, OperationArguments&);

            template <IsWord T, IsWord U>
            void add<U64, U128>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

            template <IsWord T, IsWord U>
            void substract<U64, U128>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

            template <IsWord T, IsWord U>
            void multiply<U64, U128>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

            template <IsWord T, IsWord U>
            void divide<U64, U128>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

            template <IsWord T, IsWord U>
            void modulo<U64, U128>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

            template <IsWord T>
            void compare<U64>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);
        }

        /* 18xx-1Bxx */
        namespace ExtU64 {
            void init();
            void add(OperationMap& operations);
            static SPtr<Operation> Set64;
            static SPtr<Operation> CopyFrom64;
            static SPtr<Operation> CopyTo64;
            static SPtr<Operation> Copy64;
            static SPtr<Operation> Push64;
            static SPtr<Operation> Pop64;
            static SPtr<Operation> CastTo64;
            static SPtr<Operation> CastFrom64;
            static SPtr<Operation> Copy64If;
            static SPtr<Operation> Add64;
            static SPtr<Operation> Substract64;
            static SPtr<Operation> Multiply64;
            static SPtr<Operation> Divide64;
            static SPtr<Operation> Modulo64;
            static SPtr<Operation> Random64;
            static SPtr<Operation> Compare64;
            static SPtr<Operation> Write64;
            static SPtr<Operation> Read64;
        }
    }
}