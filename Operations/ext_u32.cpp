

#include "operations.h"

#include "u_operations.h"

using Juse::IsWord;
using Juse::U32;
using Juse::U64;

template <IsWord T>
Juse::CompareFlags Juse::compare<U32>(T a, T b);

template <IsWord T>
T Juse::random<U32>(T, T);

template <IsWord T, IsWord U>
void Juse::calculate<U32, U64>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&, U, bool);

namespace Juse::Operations {

template <IsWord T>
void Unsigned::set<U32>(GeneralRegisters<T>&, OperationArguments&);

template <IsWord T>
void Unsigned::copy<U32>(GeneralRegisters<T>&, OperationArguments&);

template <IsWord T>
void Unsigned::push<U32>(Cpu&, GeneralRegisters<T>&, OperationArguments&);

template <IsWord T>
void Unsigned::pop<U32>(Cpu&, GeneralRegisters<T>&, OperationArguments&);

template <IsWord T, IsWord U>
void Unsigned::add<U32, U64>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

template <IsWord T, IsWord U>
void Unsigned::substract<U32, U64>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

template <IsWord T, IsWord U>
void Unsigned::multiply<U32, U64>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

template <IsWord T, IsWord U>
void Unsigned::divide<U32, U64>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

template <IsWord T, IsWord U>
void Unsigned::modulo<U32, U64>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

template <IsWord T>
void Unsigned::compare<U32>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

}

/* 18xx-1Bxx */
void Juse::Operations::StandardExtensions::addExtU32(Cpu& cpu)
{
    cpu.operations[0x1800] = S<Operation>(new Operation(
        "Set Quad", "SET32", "Quads[A] = B",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Juse::Operations::Unsigned::set(cpu.registers.quads, arguments);
        },
        { { SIZE8 }, { SIZE32 } }));

    cpu.operations[0x1801] = S<Operation>(new Operation(
        "Copy Quad From", "COPYFROM32", "Quads[A] = [B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            U16 address = U16(arguments[1].value);
            U32 value = U32(set2word(machine.readData(cpu, address, SIZE32)));
            cpu.registers.quads[register_index] = value;
        },
        { { SIZE8 }, { SIZE16 } }));

    cpu.operations[0x1802] = S<Operation>(new Operation(
        "Copy Quad To", "COPYTO32", "[A] = Quads[B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U16 address = U16(arguments[0].value);
            U8 register_index = U8(arguments[1].value);
            machine.writeData(cpu, address, word2set(cpu.registers.quads[register_index]));
        },
        { { SIZE16 }, { SIZE8 } }));

    cpu.operations[0x1803] = S<Operation>(new Operation(
        "Copy Quad", "COPY32", "Quads[A] = Quads[B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::copy(cpu.registers.quads, arguments);
        },
        { { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1804] = S<Operation>(new Operation(
        "Push Quad", "PUSH32", "push Quads[A]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::push(cpu, cpu.registers.quads, arguments);
        },
        { { SIZE8 } }));

    cpu.operations[0x1805] = S<Operation>(new Operation(
        "Pop Quad", "POP32", "Quads[A] = {pop}",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::pop(cpu, cpu.registers.quads, arguments);
        },
        { { SIZE8 } }));

    cpu.operations[0x1806] = S<Operation>(new Operation(
        "Cast To Quad", "CAST16TO32", "Quads[A] = Words[A]:Words[B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 rQ = U8(arguments[0].value);
            U8 rWA = U8(arguments[1].value);
            U8 rWB = U8(arguments[2].value);
            cpu.registers.quads[rQ] = cpu.registers.words[rWA] << sizes.at(SIZE16) | cpu.registers.words[rWB];
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1807] = S<Operation>(new Operation(
        "Cast From Quad", "CAST32TO16", "Words[A]:Words[B] = Quads[A]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 rWA = U8(arguments[0].value);
            U8 rWB = U8(arguments[1].value);
            U8 rQ = U8(arguments[2].value);
            U32 q = cpu.registers.quads[rQ];
            cpu.registers.words[rWA] = U16((q & MASK_32TOP16) >> sizes.at(SIZE16));
            cpu.registers.words[rWB] = U16(q & MASK_BOTTOM16);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1808] = S<Operation>(new Operation(
        "Copy Quad If", "COPY32IF", "?A : Quads[B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            CompareFlag flag = CompareFlag(U8(arguments[0].value));
            if (cpu.registers.compareFlags[flag]) {
                U8 register_index = U8(arguments[1].value);
                machine.writeData(cpu, cpu.offseted(), word2set(cpu.registers.quads[register_index]));
            }
        },
        { { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1900] = S<Operation>(new Operation(
        "Add", "ADD32", "Quads[A] = Quads[B] + Quads[C] CR Quads[D]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::add<U32, U64>(cpu.registers.quads, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1901] = S<Operation>(new Operation(
        "Substract", "SUB32", "Quads[A] = Quads[B] - Quads[C]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::substract<U32, U64>(cpu.registers.quads, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1902] = S<Operation>(new Operation(
        "Multiply", "MUL32", "Quads[A] = Quads[B] * Quads[C] CR Quads[D]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::multiply<U32, U64>(cpu.registers.quads, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1903] = S<Operation>(new Operation(
        "Divide", "DIV32", "Quads[A] = Quads[B] / Quads[C]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::divide<U32, U64>(cpu.registers.quads, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1904] = S<Operation>(new Operation(
        "Modulo", "MOD32", "Quads[A] = Quads[B] % Quads[C]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::modulo<U32, U64>(cpu.registers.quads, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    // TODO : 1505 - ABS16

    cpu.operations[0x1906] = S<Operation>(new Operation(
        "Random", "RND32", "Quads[A] = {rnd Quads[B] Quads[C]}",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 index = U8(arguments[0].value);
            U32 min = cpu.registers.quads[U8(arguments[1].value)];
            U32 max = cpu.registers.quads[U8(arguments[2].value)];
            cpu.registers.quads[index] = random(min, max);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x19F0] = S<Operation>(new Operation(
        "Compare", "CMP32", "Quads[A] ? Quads[B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::compare(cpu.registers.quads, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1B00] = S<Operation>(new Operation(
        "Write Quad", "WINT32", "out Quads[A]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            std::stringstream buffer {};
            buffer << std::dec << cpu.registers.quads[register_index];
            out(machine.out, buffer, cpu.flag_debug);
        },
        { { SIZE8 } }));
    cpu.operations[0x1B01] = S<Operation>(new Operation(
        "Read Quad", "RINT32", "in Quads[A]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U32 value;
            std::stringstream buffer;
            U8 register_index = U8(arguments[0].value);
            buffer << in(machine.out, machine.in, cpu.flag_debug);
            buffer >> std::dec >> value;
            cpu.registers.quads[register_index] = value;
        },
        { { SIZE8 } }));

    cpu.operations[0x1B10] = S<Operation>(new Operation(
        "Write Utf-32", "WUTF32", "out S32 A",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            // TODO convert encoding
            U16 address = U16(arguments[0].value);
            U16 offset = 0;
            SS8 buffer {};
            U32 character {};
            Converter<CH32, CH8> CH32_CH8;
            do {
                character = CH32(U32(set2word(machine.readData(cpu, address + offset, SIZE32))));
                if (character != U'\0') {
                    buffer << CH32_CH8.convert(character);
                }
                offset++;
            } while (character != U'\0');
            out(machine.out, buffer, cpu.flag_debug);
        },
        { { SIZE16 } }));
}