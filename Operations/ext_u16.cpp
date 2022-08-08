
#include "operations.h"

#include "u_operations.h"

using Juse::IsWord;
using Juse::U16;
using Juse::U32;

template <IsWord T>
Juse::CompareFlags Juse::compare<U16>(T a, T b);

template <IsWord T>
T Juse::random<U16>(T, T);

template <IsWord T, IsWord U>
void Juse::calculate<U16, U32>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&, U, bool);

namespace Juse::Operations {

template <IsWord T>
void Unsigned::set<U16>(GeneralRegisters<T>&, OperationArguments&);

template <IsWord T>
void Unsigned::copy<U16>(GeneralRegisters<T>&, OperationArguments&);

template <IsWord T>
void Unsigned::push<U16>(Cpu&, GeneralRegisters<T>&, OperationArguments&);

template <IsWord T>
void Unsigned::pop<U16>(Cpu&, GeneralRegisters<T>&, OperationArguments&);

template <IsWord T, IsWord U>
void Unsigned::add<U16, U32>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

template <IsWord T, IsWord U>
void Unsigned::substract<U16, U32>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

template <IsWord T, IsWord U>
void Unsigned::multiply<U16, U32>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

template <IsWord T, IsWord U>
void Unsigned::divide<U16, U32>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

template <IsWord T, IsWord U>
void Unsigned::modulo<U16, U32>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

template <IsWord T>
void Unsigned::compare<U16>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

}

/* 14xx-17xx */
void Juse::Operations::StandardExtensions::addExtU16(Cpu& cpu)
{
    // 14xx - U16 Moves & Casts
    cpu.operations[0x1400] = S<Operation>(new Operation(
        "Set Word", "SET16", "Words[A] = B",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Juse::Operations::Unsigned::set(cpu.registers.words, arguments);
        },
        { { SIZE8 }, { SIZE16 } }));

    cpu.operations[0x1401] = S<Operation>(new Operation(
        "Copy Word From", "COPYFROM16", "Words[A] = [B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            U16 address = U16(arguments[1].value);
            U16 value = U16(set2word(machine.readData(cpu, address, SIZE16)));
            cpu.registers.words[register_index] = value;
        },
        { { SIZE8 }, { SIZE16 } }));

    cpu.operations[0x1402] = S<Operation>(new Operation(
        "Copy Word To", "COPYTO16", "[A] = Words[B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U16 address = U16(arguments[0].value);
            U8 register_index = U8(arguments[1].value);
            machine.writeData(cpu, address, word2set(cpu.registers.words[register_index]));
        },
        { { SIZE16 }, { SIZE8 } }));

    cpu.operations[0x1403] = S<Operation>(new Operation(
        "Copy Word", "COPY16", "Words[A] = Words[B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::copy(cpu.registers.words, arguments);
        },
        { { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1404] = S<Operation>(new Operation(
        "Push Word", "PUSH16", "push Words[A]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::push(cpu, cpu.registers.words, arguments);
        },
        { { SIZE8 } }));

    cpu.operations[0x1405] = S<Operation>(new Operation(
        "Pop Word", "POP16", "Words[A] = {pop}",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::pop(cpu, cpu.registers.words, arguments);
        },
        { { SIZE8 } }));

    cpu.operations[0x1406] = S<Operation>(new Operation(
        "Cast To Word", "CAST8TO16", "Words[A] = Bytes[A]:Bytes[B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 rW = U8(arguments[0].value);
            U8 rBA = U8(arguments[1].value);
            U8 rBB = U8(arguments[2].value);
            cpu.registers.words[rW] = cpu.registers.bytes[rBA] << sizes.at(SIZE8) | cpu.registers.bytes[rBB];
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1407] = S<Operation>(new Operation(
        "Cast From Word", "CAST16TO8", "Bytes[A]:Bytes[B] = Words[A]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 rBA = U8(arguments[0].value);
            U8 rBB = U8(arguments[1].value);
            U8 rW = U8(arguments[2].value);
            U16 w = cpu.registers.words[rW];
            cpu.registers.bytes[rBA] = U8((w & MASK_16TOP8) >> sizes.at(SIZE8));
            cpu.registers.bytes[rBB] = U8(w & MASK_BOTTOM8);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1408] = S<Operation>(new Operation(
        "Copy Word If", "COPY16IF", "?A : Words[B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            CompareFlag flag = CompareFlag(U8(arguments[0].value));
            if (cpu.registers.compareFlags[flag]) {
                U8 register_index = U8(arguments[1].value);
                machine.writeData(cpu, cpu.offseted(), word2set(cpu.registers.words[register_index]));
            }
        },
        { { SIZE8 }, { SIZE8 } }));

    // 15xx - U16 Operations
    cpu.operations[0x1500] = S<Operation>(new Operation(
        "Add", "ADD16", "Words[A] = Words[B] + Words[C] CR Words[D]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::add<U16, U32>(cpu.registers.words, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1501] = S<Operation>(new Operation(
        "Substract", "SUB16", "Words[A] = Words[B] - Words[C]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::substract<U16, U32>(cpu.registers.words, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1502] = S<Operation>(new Operation(
        "Multiply", "MUL16", "Words[A] = Words[B] * Words[C] CR Words[D]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::multiply<U16, U32>(cpu.registers.words, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1503] = S<Operation>(new Operation(
        "Divide", "DIV16", "Words[A] = Words[B] / Words[C]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::divide<U16, U32>(cpu.registers.words, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1504] = S<Operation>(new Operation(
        "Modulo", "MOD16", "Words[A] = Words[B] % Words[C]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::modulo<U16, U32>(cpu.registers.words, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    // TODO : 1505 - ABS16

    cpu.operations[0x1506] = S<Operation>(new Operation(
        "Random", "RND16", "Words[A] = {rnd Words[B] Words[C]}",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 index = U8(arguments[0].value);
            U16 min = cpu.registers.words[U8(arguments[1].value)];
            U16 max = cpu.registers.words[U8(arguments[2].value)];
            cpu.registers.words[index] = random(min, max);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x15F0] = S<Operation>(new Operation(
        "Compare", "CMP16", "Words[A] ? Words[B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::compare(cpu.registers.words, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 } }));

    // 17xx - U16 I/O
    cpu.operations[0x1700] = S<Operation>(new Operation(
        "Write Word", "WINT16", "out Words[A]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            std::stringstream buffer {};
            buffer << std::dec << cpu.registers.words[register_index];
            out(machine.out, buffer, cpu.flag_debug);
        },
        { { SIZE8 } }));
    cpu.operations[0x1701] = S<Operation>(new Operation(
        "Read Word", "RINT16", "in Words[A]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U16 value;
            std::stringstream buffer;
            U8 register_index = U8(arguments[0].value);
            buffer << in(machine.out, machine.in, cpu.flag_debug);
            buffer >> std::dec >> value;
            cpu.registers.words[register_index] = value;
        },
        { { SIZE8 } }));

    cpu.operations[0x1710] = S<Operation>(new Operation(
        "Write Utf-16", "WUTF16", "out S16 A",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            // TODO convert encoding
            U16 address = U16(arguments[0].value);
            U16 offset = 0;
            SS8 buffer {};
            U16 character {};
            Converter<CH16, CH8> CH16_CH8;
            do {
                character = CH16(U16(set2word(machine.readData(cpu, address + offset, SIZE16))));
                if (character != u'\0') {
                    buffer << CH16_CH8.convert(character);
                }
                offset++;
            } while (character != u'\0');
            out(machine.out, buffer, cpu.flag_debug);
        },
        { { SIZE16 } }));
}