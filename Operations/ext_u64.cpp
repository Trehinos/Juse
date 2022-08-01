
#include "operations.h"

#include "u_operations.h"

using Juse::IsWord;
using Juse::U64;
using Juse::U128;

template <IsWord T>
void Juse::setWord<U64>(Juse::Operation&, Juse::Instruction&, Juse::GeneralRegisters<T>&);

template <IsWord T>
Juse::CompareFlags Juse::compare<U64>(T a, T b);

template <IsWord T>
T Juse::random<U64>(T, T);

template <IsWord T, IsWord U>
void Juse::Operations::Unsigned::calculate<U64, U128>(GeneralRegisters<T>&, CompareFlags&, Instruction&, Operation&, U, bool);

template <IsWord T, IsWord U>
void Juse::Operations::Unsigned::add<U64, U128>(Juse::GeneralRegisters<T>&, Juse::CompareFlags&, Juse::Instruction&, Juse::Operation&);

template <IsWord T, IsWord U>
void Juse::Operations::Unsigned::substract<U64, U128>(Juse::GeneralRegisters<T>&, Juse::CompareFlags&, Juse::Instruction&, Juse::Operation&);

template <IsWord T, IsWord U>
void Juse::Operations::Unsigned::multiply<U64, U128>(Juse::GeneralRegisters<T>&, Juse::CompareFlags&, Juse::Instruction&, Juse::Operation&);

template <IsWord T, IsWord U>
void Juse::Operations::Unsigned::divide<U64, U128>(Juse::GeneralRegisters<T>&, Juse::CompareFlags&, Juse::Instruction&, Juse::Operation&);

template <IsWord T, IsWord U>
void Juse::Operations::Unsigned::modulo<U64, U128>(Juse::GeneralRegisters<T>&, Juse::CompareFlags&, Juse::Instruction&, Juse::Operation&);

template <IsWord T>
void Juse::Operations::Unsigned::compare<U64>(GeneralRegisters<T>&, CompareFlags&, Instruction&, Operation&);

/* 1Cxx-1Fxx */
void Juse::Operations::StandardExtensions::ext_u64(Cpu& cpu)
{
    cpu.operations[0x1C00] = S<Operation>(new Operation(
        "Set Long", "SET64", "Longs[A] = B",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            setWord<U64>(operation, instruction, machine.cpu.registers.longs);
        },
        { { SIZE8 }, { SIZE64 } }));

    cpu.operations[0x1C01] = S<Operation>(new Operation(
        "Copy Long From", "COPYFROM64", "Longs[A] = [B]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U8 register_index = U8(operation.argument(instruction, 0));
            U16 address = U16(operation.argument(instruction, 1));
            U64 value = set2word(machine.readData(address, SIZE64));
            machine.cpu.registers.longs[register_index] = value;
        },
        { { SIZE8 }, { SIZE16 } }));

    cpu.operations[0x1C02] = S<Operation>(new Operation(
        "Copy Long To", "COPYTO64", "[A] = Longs[B]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U16 address = U16(operation.argument(instruction, 0));
            U8 register_index = U8(operation.argument(instruction, 1));
            machine.writeData(address, word2set<U64>(machine.cpu.registers.longs[register_index]));
        },
        { { SIZE16 }, { SIZE8 } }));
    cpu.operations[0x1F00] = S<Operation>(new Operation(
        "Write Long", "WINT64", "out Longs[A]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U8 register_index = U8(operation.argument(instruction, 0));
            std::stringstream buffer {};
            buffer << std::dec << machine.cpu.registers.longs[register_index];
            out(machine.out, buffer, machine.cpu.flag_debug);
        },
        { { SIZE8 } }));
    cpu.operations[0x1F01] = S<Operation>(new Operation(
        "Read Long", "RINT64", "in Longs[A]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U64 value;
            std::stringstream buffer;
            U8 register_index = U8(operation.argument(instruction, 0));
            buffer << in(machine.out, machine.in, machine.cpu.flag_debug);
            buffer >> std::dec >> value;
            machine.cpu.registers.longs[register_index] = value;
        },
        { { SIZE8 } }));
}