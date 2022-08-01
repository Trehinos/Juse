
#include "operations.h"

#include "u_operations.h"

using Juse::IsWord;
using Juse::U64;
using Juse::U128;

template <IsWord T>
void Juse::Operations::Unsigned::setWord<U64>(Juse::Operation&, Juse::Instruction&, Juse::GeneralRegisters<T>&);

template <IsWord T>
Juse::CompareFlags Juse::compare<U64>(T a, T b);

template <IsWord T>
T Juse::random<U64>(T, T);

template <IsWord T, IsWord U>
void Juse::calculate<U64, U128>(GeneralRegisters<T>&, CompareFlags&, Instruction&, Operation&, U, bool);

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
            Juse::Operations::Unsigned::setWord<U64>(operation, instruction, machine.cpu.registers.longs);
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

    cpu.operations[0x1D00] = S<Operation>(new Operation(
        "Add", "ADD32", "Quads[A] = Quads[B] + Quads[C] CR Quads[D]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            Operations::Unsigned::add<U64, U128>(machine.cpu.registers.longs, machine.cpu.registers.compareFlags, instruction, operation);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1D01] = S<Operation>(new Operation(
        "Substract", "SUB32", "Quads[A] = Quads[B] - Quads[C]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            Operations::Unsigned::substract<U64, U128>(machine.cpu.registers.longs, machine.cpu.registers.compareFlags, instruction, operation);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1D02] = S<Operation>(new Operation(
        "Multiply", "MUL32", "Quads[A] = Quads[B] * Quads[C] CR Quads[D]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            Operations::Unsigned::multiply<U64, U128>(machine.cpu.registers.longs, machine.cpu.registers.compareFlags, instruction, operation);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1D03] = S<Operation>(new Operation(
        "Divide", "DIV32", "Quads[A] = Quads[B] / Quads[C]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            Operations::Unsigned::divide<U64, U128>(machine.cpu.registers.longs, machine.cpu.registers.compareFlags, instruction, operation);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1D04] = S<Operation>(new Operation(
        "Modulo", "MOD32", "Quads[A] = Quads[B] % Quads[C]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            Operations::Unsigned::modulo<U64, U128>(machine.cpu.registers.longs, machine.cpu.registers.compareFlags, instruction, operation);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    // TODO : 1505 - ABS16

    cpu.operations[0x1D06] = S<Operation>(new Operation(
        "Random", "RND32", "Quads[A] = {rnd Quads[B] Quads[C]}",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U8 index = U8(operation.argument(instruction, 0));
            U32 min = machine.cpu.registers.quads[U8(operation.argument(instruction, 1))];
            U32 max = machine.cpu.registers.quads[U8(operation.argument(instruction, 2))];
            machine.cpu.registers.longs[index] = random<U64>(min, max);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1DF0] = S<Operation>(new Operation(
        "Compare", "CMP32", "Quads[A] ? Quads[B]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            Operations::Unsigned::compare<U64>(machine.cpu.registers.longs, machine.cpu.registers.compareFlags, instruction, operation);
        },
        { { SIZE8 }, { SIZE8 } }));

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