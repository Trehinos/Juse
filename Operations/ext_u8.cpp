
#include "operations.h"
#include "u_operations.h"

using Juse::IsWord;
using Juse::U16;
using Juse::U8;

template <IsWord T>
Juse::CompareFlags Juse::compare<U8>(T a, T b);

template <IsWord T>
T Juse::random<U16>(T, T);

template <IsWord T, IsWord U>
void Juse::calculate<U8, U16>(GeneralRegisters<T>&, CompareFlags&, Instruction&, Operation&, U, bool);

namespace Juse::Operations {

template <IsWord T>
void Unsigned::set<U8>(GeneralRegisters<T>&, Operation&, Instruction&);

template <IsWord T>
void Unsigned::copy<U8>(GeneralRegisters<T>&, Instruction&, Operation&);

template <IsWord T, IsWord U>
void Unsigned::add<U8, U16>(GeneralRegisters<T>&, CompareFlags&, Instruction&, Operation&);

template <IsWord T, IsWord U>
void Unsigned::substract<U8, U16>(GeneralRegisters<T>&, CompareFlags&, Instruction&, Operation&);

template <IsWord T, IsWord U>
void Unsigned::multiply<U8, U16>(GeneralRegisters<T>&, CompareFlags&, Instruction&, Operation&);

template <IsWord T, IsWord U>
void Unsigned::divide<U8, U16>(GeneralRegisters<T>&, CompareFlags&, Instruction&, Operation&);

template <IsWord T, IsWord U>
void Unsigned::modulo<U8, U16>(GeneralRegisters<T>&, CompareFlags&, Instruction&, Operation&);

template <IsWord T>
void Unsigned::compare<U8>(GeneralRegisters<T>&, CompareFlags&, Instruction&, Operation&);

}

/* 10xx-13xx */
void Juse::Operations::StandardExtensions::ext_u8(Cpu& cpu)
{
    cpu.operations[0x1000] = S<Operation>(new Operation(
        "Set Byte", "SET8", "Bytes[A] = B",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            Juse::Operations::Unsigned::set(machine.cpu.registers.bytes, operation, instruction);
        },
        { { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1001] = S<Operation>(new Operation(
        "Copy Byte From", "COPYFROM8", "Bytes[A] = [B]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U8 register_index = U8(operation.argument(instruction, 0));
            U16 address = U16(operation.argument(instruction, 1));
            U8 value = U8(set2word(machine.readData(address, SIZE8)));
            machine.cpu.registers.bytes[register_index] = value;
        },
        { { SIZE8 }, { SIZE16 } }));

    cpu.operations[0x1002] = S<Operation>(new Operation(
        "Copy Byte To", "COPYTO8", "[A] = Bytes[B]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U16 address = U16(operation.argument(instruction, 0));
            U8 register_index = U8(operation.argument(instruction, 1));
            machine.writeData(address, word2set(machine.cpu.registers.bytes[register_index]));
        },
        { { SIZE16 }, { SIZE8 } }));

    cpu.operations[0x1003] = S<Operation>(new Operation(
        "Copy Byte", "COPY8", "Bytes[A] = Bytes[B]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            Operations::Unsigned::copy(machine.cpu.registers.bytes, instruction, operation);
        },
        { { SIZE8 }, { SIZE8 } }));

    // 11xx - U8 Operations
    cpu.operations[0x1100] = S<Operation>(new Operation(
        "Add", "ADD8", "Bytes[A] = Bytes[B] + Bytes[C] CR Bytes[D]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            Operations::Unsigned::add<U8, U16>(machine.cpu.registers.bytes, machine.cpu.registers.compareFlags, instruction, operation);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 }, { SIZE8 } }));
    cpu.operations[0x1101] = S<Operation>(new Operation(
        "Substract", "SUB8", "Bytes[A] = Bytes[B] - Bytes[C]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            Operations::Unsigned::substract<U8, U16>(machine.cpu.registers.bytes, machine.cpu.registers.compareFlags, instruction, operation);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));
    cpu.operations[0x1102] = S<Operation>(new Operation(
        "Multiply", "MUL8", "Bytes[A] = Bytes[B] * Bytes[C] CR Bytes[D]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            Operations::Unsigned::multiply<U8, U16>(machine.cpu.registers.bytes, machine.cpu.registers.compareFlags, instruction, operation);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 }, { SIZE8 } }));
    cpu.operations[0x1103] = S<Operation>(new Operation(
        "Divide", "DIV8", "Bytes[A] = Bytes[B] / Bytes[C]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            Operations::Unsigned::divide<U8, U16>(machine.cpu.registers.bytes, machine.cpu.registers.compareFlags, instruction, operation);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));
    cpu.operations[0x1104] = S<Operation>(new Operation(
        "Modulo", "MOD8", "Bytes[A] = Bytes[B] % Bytes[C]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            Operations::Unsigned::modulo<U8, U16>(machine.cpu.registers.bytes, machine.cpu.registers.compareFlags, instruction, operation);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1106] = S<Operation>(new Operation(
        "Random", "RND16", "Bytes[A] = {rnd Bytes[B] Bytes[C]}",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U8 index = U8(operation.argument(instruction, 0));
            U8 min = machine.cpu.registers.bytes[U8(operation.argument(instruction, 1))];
            U8 max = machine.cpu.registers.bytes[U8(operation.argument(instruction, 2))];
            machine.cpu.registers.bytes[index] = U8(random<U16>(min, max));
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x11F0] = S<Operation>(new Operation(
        "Compare", "CMP8", "Bytes[A] ? Bytes[B]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            Operations::Unsigned::compare(machine.cpu.registers.bytes, machine.cpu.registers.compareFlags, instruction, operation);
        },
        { { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1300] = S<Operation>(new Operation(
        "Write Byte", "WINT8", "out Bytes[A]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U8 register_index = U8(operation.argument(instruction, 0));
            std::stringstream buffer {};
            buffer << std::dec << +machine.cpu.registers.bytes[register_index];
            out(machine.out, buffer, machine.cpu.flag_debug);
        },
        { { SIZE8 } }));

    cpu.operations[0x1301] = S<Operation>(new Operation(
        "Read Byte", "RINT8", "in Bytes[A]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U16 value;
            std::stringstream buffer;
            U8 register_index = U8(operation.argument(instruction, 0));
            buffer << in(machine.out, machine.in, machine.cpu.flag_debug);
            buffer >> std::dec >> value;
            machine.cpu.registers.bytes[register_index] = U8(value);
        },
        { { SIZE8 } }));

    cpu.operations[0x1310] = S<Operation>(new Operation(
        "Write Ascii", "WASCII", "out S8 A",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U16 address = U16(operation.argument(instruction, 0));
            U16 offset = 0;
            std::stringstream buffer {};
            CH8 character {};
            do {
                character = CH8(U8(set2word(machine.readData(address + offset, SIZE8))));
                if (character != '\0') {
                    buffer << character;
                }
                offset++;
            } while (character != '\0');
            out(machine.out, buffer, machine.cpu.flag_debug);
        },
        { { SIZE16 } }));
}