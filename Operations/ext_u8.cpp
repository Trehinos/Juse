
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
void Juse::calculate<U8, U16>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&, U, bool);

namespace Juse::Operations {

template <IsWord T>
void Unsigned::set<U8>(GeneralRegisters<T>&, OperationArguments&);

template <IsWord T>
void Unsigned::copy<U8>(GeneralRegisters<T>&, OperationArguments&);

template <IsWord T>
void Unsigned::push<U8>(Cpu&, GeneralRegisters<T>&, OperationArguments&);

template <IsWord T>
void Unsigned::pop<U8>(Cpu&, GeneralRegisters<T>&, OperationArguments&);

template <IsWord T, IsWord U>
void Unsigned::add<U8, U16>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

template <IsWord T, IsWord U>
void Unsigned::substract<U8, U16>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

template <IsWord T, IsWord U>
void Unsigned::multiply<U8, U16>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

template <IsWord T, IsWord U>
void Unsigned::divide<U8, U16>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

template <IsWord T, IsWord U>
void Unsigned::modulo<U8, U16>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

template <IsWord T>
void Unsigned::compare<U8>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

}

/* 10xx-13xx */
void Juse::Operations::StandardExtensions::ext_u8(Cpu& cpu)
{
    // 10xx : U8 moves, stack operations, casts
    cpu.operations[0x1000] = S<Operation>(new Operation(
        "Set Byte", "SET8", "Bytes[A] = B",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Juse::Operations::Unsigned::set(cpu.registers.bytes, arguments);
        },
        { { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1001] = S<Operation>(new Operation(
        "Copy Byte From", "COPYFROM8", "Bytes[A] = [B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            U16 address = U16(arguments[1].value);
            U8 value = U8(set2word(machine.readData(cpu, address, SIZE8)));
            cpu.registers.bytes[register_index] = value;
        },
        { { SIZE8 }, { SIZE16 } }));

    cpu.operations[0x1002] = S<Operation>(new Operation(
        "Copy Byte To", "COPYTO8", "[A] = Bytes[B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U16 address = U16(arguments[0].value);
            U8 register_index = U8(arguments[1].value);
            machine.writeData(cpu, address, word2set(cpu.registers.bytes[register_index]));
        },
        { { SIZE16 }, { SIZE8 } }));

    cpu.operations[0x1003] = S<Operation>(new Operation(
        "Copy Byte", "COPY8", "Bytes[A] = Bytes[B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::copy(cpu.registers.bytes, arguments);
        },
        { { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1004] = S<Operation>(new Operation(
        "Push Byte", "PUSH8", "push Bytes[A]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::push(cpu, cpu.registers.bytes, arguments);
        },
        { { SIZE8 } }));

    cpu.operations[0x1005] = S<Operation>(new Operation(
        "Pop Byte", "POP8", "Bytes[A] = {pop}",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::pop(cpu, cpu.registers.bytes, arguments);
        },
        { { SIZE8 } }));

    // 1006 - 1007 : casts operations, not applicable to u8

    cpu.operations[0x1008] = S<Operation>(new Operation(
        "Copy Byte If", "COPY8IF", "?A : Bytes[B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            CompareFlag flag = CompareFlag(U8(arguments[0].value));
            if (cpu.registers.compareFlags[flag]) {
                U8 register_index = U8(arguments[1].value);
                machine.writeData(cpu, cpu.offseted(), word2set(cpu.registers.bytes[register_index]));
            }
        },
        { { SIZE8 }, { SIZE8 } }));

    // 11xx - U8 Operations
    cpu.operations[0x1100] = S<Operation>(new Operation(
        "Add", "ADD8", "Bytes[A] = Bytes[B] + Bytes[C] CR Bytes[D]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::add<U8, U16>(cpu.registers.bytes, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 }, { SIZE8 } }));
    cpu.operations[0x1101] = S<Operation>(new Operation(
        "Substract", "SUB8", "Bytes[A] = Bytes[B] - Bytes[C]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::substract<U8, U16>(cpu.registers.bytes, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));
    cpu.operations[0x1102] = S<Operation>(new Operation(
        "Multiply", "MUL8", "Bytes[A] = Bytes[B] * Bytes[C] CR Bytes[D]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::multiply<U8, U16>(cpu.registers.bytes, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 }, { SIZE8 } }));
    cpu.operations[0x1103] = S<Operation>(new Operation(
        "Divide", "DIV8", "Bytes[A] = Bytes[B] / Bytes[C]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::divide<U8, U16>(cpu.registers.bytes, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));
    cpu.operations[0x1104] = S<Operation>(new Operation(
        "Modulo", "MOD8", "Bytes[A] = Bytes[B] % Bytes[C]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::modulo<U8, U16>(cpu.registers.bytes, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1106] = S<Operation>(new Operation(
        "Random", "RND16", "Bytes[A] = {rnd Bytes[B] Bytes[C]}",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 index = U8(arguments[0].value);
            U8 min = cpu.registers.bytes[U8(arguments[1].value)];
            U8 max = cpu.registers.bytes[U8(arguments[2].value)];
            cpu.registers.bytes[index] = U8(random<U16>(min, max));
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x11F0] = S<Operation>(new Operation(
        "Compare", "CMP8", "Bytes[A] ? Bytes[B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::compare(cpu.registers.bytes, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1300] = S<Operation>(new Operation(
        "Write Byte", "WINT8", "out Bytes[A]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            std::stringstream buffer {};
            buffer << std::dec << +cpu.registers.bytes[register_index];
            out(machine.out, buffer, cpu.flag_debug);
        },
        { { SIZE8 } }));

    cpu.operations[0x1301] = S<Operation>(new Operation(
        "Read Byte", "RINT8", "in Bytes[A]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U16 value;
            std::stringstream buffer;
            U8 register_index = U8(arguments[0].value);
            buffer << in(machine.out, machine.in, cpu.flag_debug);
            buffer >> std::dec >> value;
            cpu.registers.bytes[register_index] = U8(value);
        },
        { { SIZE8 } }));

    cpu.operations[0x1310] = S<Operation>(new Operation(
        "Write Ascii", "WASCII", "out S8 A",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U16 address = U16(arguments[0].value);
            U16 offset = 0;
            std::stringstream buffer {};
            CH8 character {};
            do {
                character = CH8(U8(set2word(machine.readData(cpu, address + offset, SIZE8))));
                if (character != '\0') {
                    buffer << character;
                }
                offset++;
            } while (character != '\0');
            out(machine.out, buffer, cpu.flag_debug);
        },
        { { SIZE16 } }));
}