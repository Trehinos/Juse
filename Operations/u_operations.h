#pragma once

#include <random>

#include "../Machine/Machine.h"
#include "../Machine/Operation.h"

namespace Juse {

template <IsWord T>
CompareFlags compare(T a, T b)
{
    CompareFlags flags = Registers::createFlags();
    if (a == b) {
        flags[CompareFlag::EQ] = true;
        flags[CompareFlag::GE] = true;
        flags[CompareFlag::LE] = true;
        if (a == 0) {
            flags[CompareFlag::Z0] = true;
        }
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

namespace Operations {
    namespace Unsigned {
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

        template <IsWord T, IsWord U>
        void calculate(GeneralRegisters<T>& registers, CompareFlags& flags, Instruction& instruction, Operation& operation, U result, bool manageOverflow = false)
        {
            T overflow = T(result >> (sizeof(T) * 8));
            T mask = 0;
            for (size_t s = 0; s < sizeof(T); s++) {
                mask <<= 8;
                mask |= 0xFF;
            }
            CalculationResult<T> r { T(result & mask), overflow };
            flags = Registers::createFlags();
            registers[U8(operation.argument(instruction, 0))] = r.result;
            if (manageOverflow) {
                flags[CompareFlag::OF] = r.overflow != 0;
                registers[U8(operation.argument(instruction, 3))] = r.overflow;
            }
        }

        template <IsWord T, IsWord U>
        void add(GeneralRegisters<T>& registers, CompareFlags& flags, Instruction& instruction, Operation& operation)
        {
            T rB = registers[U8(operation.argument(instruction, 1))];
            T rC = registers[U8(operation.argument(instruction, 2))];
            calculate<T, U>(registers, flags, instruction, operation, U(rB) + U(rC), true);
        }

        template <IsWord T, IsWord U>
        void substract(GeneralRegisters<T>& registers, CompareFlags& flags, Instruction& instruction, Operation& operation)
        {
            T rB = registers[U8(operation.argument(instruction, 1))];
            T rC = registers[U8(operation.argument(instruction, 2))];
            calculate<T, U>(registers, flags, instruction, operation, U(rB - rC));
        }

        template <IsWord T, IsWord U>
        void multiply(GeneralRegisters<T>& registers, CompareFlags& flags, Instruction& instruction, Operation& operation)
        {
            T rB = registers[U8(operation.argument(instruction, 1))];
            T rC = registers[U8(operation.argument(instruction, 2))];
            calculate<T, U>(registers, flags, instruction, operation, U(rB) - U(rC), true);
        }

        template <IsWord T, IsWord U>
        void divide(GeneralRegisters<T>& registers, CompareFlags& flags, Instruction& instruction, Operation& operation)
        {
            T rB = registers[U8(operation.argument(instruction, 1))];
            T rC = registers[U8(operation.argument(instruction, 2))];
            calculate<T, U>(registers, flags, instruction, operation, U(rB / rC));
        }

        template <IsWord T, IsWord U>
        void modulo(GeneralRegisters<T>& registers, CompareFlags& flags, Instruction& instruction, Operation& operation)
        {
            T rB = registers[U8(operation.argument(instruction, 1))];
            T rC = registers[U8(operation.argument(instruction, 2))];
            calculate<T, U>(registers, flags, instruction, operation, U(rB % rC));
        }
    }
}
}