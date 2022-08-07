#pragma once

#include <random>

#include "../Machine/Machine.h"
#include "../Machine/Operation.h"

namespace Juse {

template <IsWord T>
CompareFlags compare(T a, T b)
{
    CompareFlags flags = Registers::createFlags();
    if (a == 0 || b == 0) {
        flags[CompareFlag::Z0] = true;
    }
    if (a == b) {
        flags[CompareFlag::EQ] = true;
        flags[CompareFlag::GE] = true;
        flags[CompareFlag::LE] = true;
    } else {
        flags[CompareFlag::NE] = true;
    }
    if (a > b) {
        flags[CompareFlag::GT] = true;
    }
    if (a < b) {
        flags[CompareFlag::LW] = true;
    }
    return flags;
}

template <IsWord T>
T random(T min, T max)
{
    static std::random_device r;
    static std::uniform_int_distribution<T> uniform(min, max);
    static std::default_random_engine e(r());
    return uniform(e);
}

template <IsWord T>
struct CalculationResult {
    T result;
    T overflow;

    CalculationResult(T r, T o)
        : result { r }
        , overflow { o }
    {
    }
};

// Common code of add/substract/multîply/divide/modulo operations
template <IsWord T, IsWord U>
void calculate(GeneralRegisters<T>& registers, CompareFlags& flags, OperationArguments& arguments, U result, bool manageOverflow = false)
{
    T overflow = T(result >> (sizeof(T) * 8));
    T mask = 0;
    for (size_t s = 0; s < sizeof(T); s++) {
        mask <<= 8;
        mask |= 0xFF;
    }
    CalculationResult<T> r { T(result & mask), overflow };
    flags = Registers::createFlags();
    registers[U8(arguments[0].value)] = r.result;
    if (manageOverflow) {
        flags[CompareFlag::OF] = r.overflow != 0;
        registers[U8(arguments[3].value)] = r.overflow;
    }
}

namespace Operations {
    namespace Unsigned {
        template <IsWord T>
        void set(GeneralRegisters<T>& registers, OperationArguments& arguments)
        {
            U8 register_index = U8(arguments[0].value);
            T value = T(arguments[1].value);
            registers[register_index] = value;
        }

        template <IsWord T>
        void copy(GeneralRegisters<T>& registers, OperationArguments& arguments)
        {
            U8 rA = U8(arguments[0].value);
            U8 rB = U8(arguments[1].value);
            registers[rA] = registers[rB];
        }

        template <IsWord T>
        void push(Cpu& cpu, GeneralRegisters<T>& registers, OperationArguments& arguments)
        {
            U8 register_index = U8(arguments[0].value);
            cpu.multiPush(word2set<T>(registers[register_index]));
        }

        template <IsWord T>
        void pop(Cpu& cpu, GeneralRegisters<T>& registers, OperationArguments& arguments)
        {
            U8 register_index = U8(arguments[0].value);
            registers[register_index] = T(set2word(cpu.multiPop(sizeof(T))));
        }

        template <IsWord T, IsWord U>
        void add(GeneralRegisters<T>& registers, CompareFlags& flags, OperationArguments& arguments)
        {
            T rB = registers[U8(arguments[1].value)];
            T rC = registers[U8(arguments[2].value)];
            calculate<T, U>(registers, flags, arguments, U(rB) + U(rC), true);
        }

        template <IsWord T, IsWord U>
        void substract(GeneralRegisters<T>& registers, CompareFlags& flags, OperationArguments& arguments)
        {
            T rB = registers[U8(arguments[1].value)];
            T rC = registers[U8(arguments[2].value)];
            calculate<T, U>(registers, flags, arguments, U(rB - rC));
        }

        template <IsWord T, IsWord U>
        void multiply(GeneralRegisters<T>& registers, CompareFlags& flags, OperationArguments& arguments)
        {
            T rB = registers[U8(arguments[1].value)];
            T rC = registers[U8(arguments[2].value)];
            calculate<T, U>(registers, flags, arguments, U(rB) - U(rC), true);
        }

        template <IsWord T, IsWord U>
        void divide(GeneralRegisters<T>& registers, CompareFlags& flags, OperationArguments& arguments)
        {
            T rB = registers[U8(arguments[1].value)];
            T rC = registers[U8(arguments[2].value)];
            calculate<T, U>(registers, flags, arguments, U(rB / rC));
        }

        template <IsWord T, IsWord U>
        void modulo(GeneralRegisters<T>& registers, CompareFlags& flags, OperationArguments& arguments)
        {
            T rB = registers[U8(arguments[1].value)];
            T rC = registers[U8(arguments[2].value)];
            calculate<T, U>(registers, flags, arguments, U(rB % rC));
        }

        template <IsWord T>
        void compare(GeneralRegisters<T>& registers, CompareFlags& flags, OperationArguments& arguments)
        {
            U8 iA = U8(arguments[0].value);
            U8 iB = U8(arguments[1].value);
            T rA = registers[iA];
            T rB = registers[iB];
            flags = Juse::compare<T>(rA, rB);
        }
    }
}
}