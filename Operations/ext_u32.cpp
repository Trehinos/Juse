

#include "operations.h"

#include "u_operations.h"

using Juse::IsWord;
using Juse::U32;
using Juse::U64;

template <IsWord T>
void Juse::setWord<U32>(Juse::Operation&, Juse::Instruction&, Juse::GeneralRegisters<T>&);

template <IsWord T>
Juse::CompareFlags Juse::compare<U32>(T a, T b);

template <IsWord T>
T Juse::random<U32>(T, T);

template <IsWord T, IsWord U>
void Juse::Operations::Unsigned::calculate<U32, U64>(GeneralRegisters<T>&, CompareFlags&, Instruction&, Operation&, U, bool);

template <IsWord T, IsWord U>
void Juse::Operations::Unsigned::add<U32, U64>(Juse::GeneralRegisters<T>&, Juse::CompareFlags&, Juse::Instruction&, Juse::Operation&);

template <IsWord T, IsWord U>
void Juse::Operations::Unsigned::substract<U32, U64>(Juse::GeneralRegisters<T>&, Juse::CompareFlags&, Juse::Instruction&, Juse::Operation&);

template <IsWord T, IsWord U>
void Juse::Operations::Unsigned::multiply<U32, U64>(Juse::GeneralRegisters<T>&, Juse::CompareFlags&, Juse::Instruction&, Juse::Operation&);

template <IsWord T, IsWord U>
void Juse::Operations::Unsigned::divide<U32, U64>(Juse::GeneralRegisters<T>&, Juse::CompareFlags&, Juse::Instruction&, Juse::Operation&);

template <IsWord T, IsWord U>
void Juse::Operations::Unsigned::modulo<U32, U64>(Juse::GeneralRegisters<T>&, Juse::CompareFlags&, Juse::Instruction&, Juse::Operation&);

template <IsWord T>
void Juse::Operations::Unsigned::compare<U32>(GeneralRegisters<T>&, CompareFlags&, Instruction&, Operation&);

/* 18xx-1Bxx */
void Juse::Operations::StandardExtensions::ext_u32(Cpu& cpu)
{
    cpu.operations[0x1800] = S<Operation>(new Operation(
        "Set Quad", "SET32", "Quads[A] = B",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            setWord<U32>(operation, instruction, machine.cpu.registers.quads);
        },
        { { SIZE8 }, { SIZE32 } }));

    cpu.operations[0x1801] = S<Operation>(new Operation(
        "Copy Quad From", "COPYFROM32", "Quads[A] = [B]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U8 register_index = U8(operation.argument(instruction, 0));
            U16 address = U16(operation.argument(instruction, 1));
            U32 value = U32(set2word(machine.readData(address, SIZE32)));
            machine.cpu.registers.quads[register_index] = value;
        },
        { { SIZE8 }, { SIZE16 } }));

    cpu.operations[0x1802] = S<Operation>(new Operation(
        "Copy Quad To", "COPYTO32", "[A] = Quads[B]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U16 address = U16(operation.argument(instruction, 0));
            U8 register_index = U8(operation.argument(instruction, 1));
            machine.writeData(address, word2set<U32>(machine.cpu.registers.quads[register_index]));
        },
        { { SIZE16 }, { SIZE8 } }));

    cpu.operations[0x1900] = S<Operation>(new Operation(
        "Add", "ADD32", "Quads[A] = Quads[B] + Quads[C] CR Quads[D]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            Operations::Unsigned::add<U32, U64>(machine.cpu.registers.quads, machine.cpu.registers.compareFlags, instruction, operation);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1901] = S<Operation>(new Operation(
        "Substract", "SUB32", "Quads[A] = Quads[B] - Quads[C]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            Operations::Unsigned::substract<U32, U64>(machine.cpu.registers.quads, machine.cpu.registers.compareFlags, instruction, operation);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1902] = S<Operation>(new Operation(
        "Multiply", "MUL32", "Quads[A] = Quads[B] * Quads[C] CR Quads[D]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            Operations::Unsigned::multiply<U32, U64>(machine.cpu.registers.quads, machine.cpu.registers.compareFlags, instruction, operation);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1903] = S<Operation>(new Operation(
        "Divide", "DIV32", "Quads[A] = Quads[B] / Quads[C]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            Operations::Unsigned::divide<U32, U64>(machine.cpu.registers.quads, machine.cpu.registers.compareFlags, instruction, operation);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1904] = S<Operation>(new Operation(
        "Modulo", "MOD32", "Quads[A] = Quads[B] % Quads[C]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            Operations::Unsigned::modulo<U32, U64>(machine.cpu.registers.quads, machine.cpu.registers.compareFlags, instruction, operation);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    // TODO : 1505 - ABS16

    cpu.operations[0x1906] = S<Operation>(new Operation(
        "Random", "RND32", "Quads[A] = {rnd Quads[B] Quads[C]}",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U8 index = U8(operation.argument(instruction, 0));
            U32 min = machine.cpu.registers.quads[U8(operation.argument(instruction, 1))];
            U32 max = machine.cpu.registers.quads[U8(operation.argument(instruction, 2))];
            machine.cpu.registers.quads[index] = random<U32>(min, max);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x19F0] = S<Operation>(new Operation(
        "Compare", "CMP32", "Quads[A] ? Quads[B]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            Operations::Unsigned::compare<U32>(machine.cpu.registers.quads, machine.cpu.registers.compareFlags, instruction, operation);
        },
        { { SIZE8 }, { SIZE8 } }));


    cpu.operations[0x1B00] = S<Operation>(new Operation(
        "Write Quad", "WINT32", "out Quads[A]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U8 register_index = U8(operation.argument(instruction, 0));
            std::stringstream buffer {};
            buffer << std::dec << machine.cpu.registers.quads[register_index];
            out(machine.out, buffer, machine.cpu.flag_debug);
        },
        { { SIZE8 } }));
    cpu.operations[0x1B01] = S<Operation>(new Operation(
        "Read Quad", "RINT32", "in Quads[A]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U32 value;
            std::stringstream buffer;
            U8 register_index = U8(operation.argument(instruction, 0));
            buffer << in(machine.out, machine.in, machine.cpu.flag_debug);
            buffer >> std::dec >> value;
            machine.cpu.registers.quads[register_index] = value;
        },
        { { SIZE8 } }));

    cpu.operations[0x1B10] = S<Operation>(new Operation(
        "Write Utf-32", "WUTF32", "out S32 A",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            // TODO convert encoding
            U16 address = U16(operation.argument(instruction, 0));
            U16 offset = 0;
            SS8 buffer {};
            U32 character {};
            Converter<CH32, CH8> CH32_CH8;
            do {
                character = CH32(U32(set2word(machine.readData(address + offset, SIZE32))));
                if (character != U'\0') {
                    buffer << CH32_CH8.convert(character);
                }
                offset++;
            } while (character != U'\0');
            out(machine.out, buffer, machine.cpu.flag_debug);
        },
        { { SIZE16 } }));
}