#pragma once

#include <type_traits>
#include <random>
#include <cmath>

#include "../../Juse/Machine/Machine.h"
#include "../../Juse/Machine/Operation.h"

namespace Juse
{

    template <IsWord T>
    CompareFlags compare(T a, T b) {
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
    T random(T min, T max) {
        static std::random_device r;
        static std::uniform_int_distribution<T> uniform(min, max);
        static std::default_random_engine e(r());
        return uniform(e);
    }

    template <IsWord T>
    struct CalculationResult
    {
        T result;
        T overflow;

        CalculationResult(T r, T o)
            : result { r }
            , overflow { o } { }
    };

    // Common code of add/substract/multîply/divide/modulo operations
    template <IsWord T, IsWord U>
    void calculate(GeneralRegisters<T>& registers, CompareFlags& flags, OperationArguments& arguments, U result, bool manageOverflow = false) {
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
        T signMask = T(T(0b10000000) << T(std::llabs(sizeof(T) - 8)));
        flags[CompareFlag::SN] = (signMask & r.result) > 0;
    }

    namespace Operations
    {

        template <IsWord T>
        Ref<GeneralRegisters<T>> registersOf(Cpu& cpu) {
            Wrap<GeneralRegisters<T>> reg = std::nullopt;
            if (IsAnyOf<T, U8>) {
                reg = cpu.registers.bytes;
            } else if (IsAnyOf<T, U16>) {
                reg = cpu.registers.words;
            } else if (IsAnyOf<T, U32>) {
                reg = cpu.registers.quads;
            } else if (IsAnyOf<T, U64>) {
                reg = cpu.registers.longs;
            }
            if (reg == std::nullopt) {
                throw std::exception("Not valid type for register...");
            }
            return reg.value();
        }

        namespace Unsigned
        {
            template <IsWord T>
            SPtr<Operation> set(S8 type) {
                S8 description{"Set " + type};
                S8 jumne{"SET" + (8 * sizeof WO)};
                S8 juselang{type + "[A] = B"};

                auto& code = [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
                    GeneralRegisters<T>& registers = registersOf<T>(cpu);
                    U8 register_index = U8(arguments[0].value);
                    T value = T(arguments[1].value);
                    registers[register_index] = value;
                };

                return new Operation(
                    description, jumne, juselang, code,
                    { { SIZE8 }, { 8 * sizeof T } }
                );
            }

            template <IsWord T, class ADDR = DataId>
            SPtr<Operation> copyTo(S8 type) {
                S8 description = "Copy " + type + " To";
                S8 jumne = "COPYTO" + (8 * sizeof T);
                S8 juselang = "[A] = " + type + "[B]";

                auto& code = [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
                    GeneralRegisters<T>& registers = registersOf<T>(cpu);
                    ADDR address = ADDR(arguments[0].value);
                    U8 register_index = U8(arguments[1].value);
                    machine.writeData(cpu, address, word2set<T>(registers[register_index]));
                };

                return new Operation(
                    description, jumne, juselang, code,
                    { { 8 * sizeof ADDR }, { SIZE8 } }
                );
            }


            template <IsWord T>
            SPtr<Operation> copy(S8 type) {
                S8 description = "Copy " + type;
                S8 jumne = "COPY" + (8 * sizeof T);
                S8 juselang = type + "[A] = " + type + "[B]";

                auto& code = [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
                    GeneralRegisters<T>& registers = registersOf<T>(cpu);
                    U8 rA = U8(arguments[0].value);
                    U8 rB = U8(arguments[1].value);
                    registers[rA] = registers[rB];
                };

                return new Operation(
                    description, jumne, juselang, code,
                    { {SIZE8}, {SIZE8} }
                );
            }



            template <IsWord T>
            void push(Cpu& cpu, GeneralRegisters<T>& registers, OperationArguments& arguments) {
                U8 register_index = U8(arguments[0].value);
                cpu.multiPush(word2set<T>(registers[register_index]));
            }

            template <IsWord T>
            void pop(Cpu& cpu, GeneralRegisters<T>& registers, OperationArguments& arguments) {
                U8 register_index = U8(arguments[0].value);
                registers[register_index] = T(set2word(cpu.multiPop(sizeof(T))));
            }

            template <IsWord T, IsWord U>
            void add(GeneralRegisters<T>& registers, CompareFlags& flags, OperationArguments& arguments) {
                T rB = registers[U8(arguments[1].value)];
                T rC = registers[U8(arguments[2].value)];
                calculate<T, U>(registers, flags, arguments, U(rB) + U(rC), true);
            }

            template <IsWord T, IsWord U>
            void substract(GeneralRegisters<T>& registers, CompareFlags& flags, OperationArguments& arguments) {
                T rB = registers[U8(arguments[1].value)];
                T rC = registers[U8(arguments[2].value)];
                calculate<T, U>(registers, flags, arguments, U(rB - rC));
            }

            template <IsWord T, IsWord U>
            void multiply(GeneralRegisters<T>& registers, CompareFlags& flags, OperationArguments& arguments) {
                T rB = registers[U8(arguments[1].value)];
                T rC = registers[U8(arguments[2].value)];
                calculate<T, U>(registers, flags, arguments, U(rB) - U(rC), true);
            }

            template <IsWord T, IsWord U>
            void divide(GeneralRegisters<T>& registers, CompareFlags& flags, OperationArguments& arguments) {
                T rB = registers[U8(arguments[1].value)];
                T rC = registers[U8(arguments[2].value)];
                calculate<T, U>(registers, flags, arguments, U(rB / rC));
            }

            template <IsWord T, IsWord U>
            void modulo(GeneralRegisters<T>& registers, CompareFlags& flags, OperationArguments& arguments) {
                T rB = registers[U8(arguments[1].value)];
                T rC = registers[U8(arguments[2].value)];
                calculate<T, U>(registers, flags, arguments, U(rB % rC));
            }

            template <IsWord T>
            void compare(GeneralRegisters<T>& registers, CompareFlags& flags, OperationArguments& arguments) {
                U8 iA = U8(arguments[0].value);
                U8 iB = U8(arguments[1].value);
                T rA = registers[iA];
                T rB = registers[iB];
                flags = Juse::compare<T>(rA, rB);
            }
        }
    }
}