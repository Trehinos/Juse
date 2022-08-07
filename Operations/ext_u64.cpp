
#include "operations.h"

#include "u_operations.h"

using Juse::IsWord;
using Juse::U128;
using Juse::U64;

template <IsWord T>
Juse::CompareFlags Juse::compare<U64>(T a, T b);

template <IsWord T>
T Juse::random<U64>(T, T);

template <IsWord T, IsWord U>
void Juse::calculate<U64, U128>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&, U, bool);

namespace Juse::Operations {

template <IsWord T>
void Unsigned::set<U64>(GeneralRegisters<T>&, OperationArguments&);

template <IsWord T>
void Unsigned::copy<U64>(GeneralRegisters<T>&, OperationArguments&);

template <IsWord T>
void Unsigned::push<U64>(Cpu&, GeneralRegisters<T>&, OperationArguments&);

template <IsWord T>
void Unsigned::pop<U64>(Cpu&, GeneralRegisters<T>&, OperationArguments&);

template <IsWord T, IsWord U>
void Unsigned::add<U64, U128>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

template <IsWord T, IsWord U>
void Unsigned::substract<U64, U128>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

template <IsWord T, IsWord U>
void Unsigned::multiply<U64, U128>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

template <IsWord T, IsWord U>
void Unsigned::divide<U64, U128>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

template <IsWord T, IsWord U>
void Unsigned::modulo<U64, U128>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

template <IsWord T>
void Unsigned::compare<U64>(GeneralRegisters<T>&, CompareFlags&, OperationArguments&);

}

/* 1Cxx-1Fxx */
void Juse::Operations::StandardExtensions::ext_u64(Cpu& cpu)
{
    cpu.operations[0x1C00] = S<Operation>(new Operation(
        "Set Long", "SET64", "Longs[A] = B",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Juse::Operations::Unsigned::set(cpu.registers.longs, arguments);
        },
        { { SIZE8 }, { SIZE64 } }));

    cpu.operations[0x1C01] = S<Operation>(new Operation(
        "Copy Long From", "COPYFROM64", "Longs[A] = [B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            U16 address = U16(arguments[1].value);
            U64 value = set2word(machine.readData(cpu, address, SIZE64));
            cpu.registers.longs[register_index] = value;
        },
        { { SIZE8 }, { SIZE16 } }));

    cpu.operations[0x1C02] = S<Operation>(new Operation(
        "Copy Long To", "COPYTO64", "[A] = Longs[B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U16 address = U16(arguments[0].value);
            U8 register_index = U8(arguments[1].value);
            machine.writeData(cpu, address, word2set(cpu.registers.longs[register_index]));
        },
        { { SIZE16 }, { SIZE8 } }));

    cpu.operations[0x1C03] = S<Operation>(new Operation(
        "Copy Long", "COPY64", "Longs[A] = Longs[B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::copy(cpu.registers.longs, arguments);
        },
        { { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1C04] = S<Operation>(new Operation(
        "Push Long", "PUSH64", "push Longs[A]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::push(cpu, cpu.registers.longs, arguments);
        },
        { { SIZE8 } }));

    cpu.operations[0x1C05] = S<Operation>(new Operation(
        "Push Quad", "POP64", "Longs[A] = {pop}",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::pop(cpu, cpu.registers.longs, arguments);
        },
        { { SIZE8 } }));

    cpu.operations[0x1C06] = S<Operation>(new Operation(
        "Cast To Long", "CAST32TO64", "Longs[A] = Quads[A]:Quads[B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 rL = U8(arguments[0].value);
            U8 rQA = U8(arguments[1].value);
            U8 rQB = U8(arguments[2].value);
            cpu.registers.longs[rL] = (U64(cpu.registers.quads[rQA]) << sizes.at(SIZE32)) | cpu.registers.quads[rQB];
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1C07] = S<Operation>(new Operation(
        "Cast From Long", "CAST64TO32", "Quads[A]:Quads[B] = Longs[A]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 rQA = U8(arguments[0].value);
            U8 rQB = U8(arguments[1].value);
            U8 rL = U8(arguments[2].value);
            U64 l = cpu.registers.longs[rL];
            cpu.registers.quads[rQA] = U32((l & MASK_64TOP32) >> sizes.at(SIZE32));
            cpu.registers.quads[rQB] = U32(l & MASK_BOTTOM32);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1C08] = S<Operation>(new Operation(
        "Copy Long If", "COPY64IF", "?A : Longs[B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            CompareFlag flag = CompareFlag(U8(arguments[0].value));
            if (cpu.registers.compareFlags[flag]) {
                U8 register_index = U8(arguments[1].value);
                machine.writeData(cpu, cpu.offseted(), word2set(cpu.registers.longs[register_index]));
            }
        },
        { { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1D00] = S<Operation>(new Operation(
        "Add", "ADD32", "Quads[A] = Quads[B] + Quads[C] CR Quads[D]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::add<U64, U128>(cpu.registers.longs, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1D01] = S<Operation>(new Operation(
        "Substract", "SUB32", "Quads[A] = Quads[B] - Quads[C]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::substract<U64, U128>(cpu.registers.longs, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1D02] = S<Operation>(new Operation(
        "Multiply", "MUL32", "Quads[A] = Quads[B] * Quads[C] CR Quads[D]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::multiply<U64, U128>(cpu.registers.longs, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1D03] = S<Operation>(new Operation(
        "Divide", "DIV32", "Quads[A] = Quads[B] / Quads[C]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::divide<U64, U128>(cpu.registers.longs, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1D04] = S<Operation>(new Operation(
        "Modulo", "MOD32", "Quads[A] = Quads[B] % Quads[C]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::modulo<U64, U128>(cpu.registers.longs, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    // TODO : 1505 - ABS16

    cpu.operations[0x1D06] = S<Operation>(new Operation(
        "Random", "RND32", "Quads[A] = {rnd Quads[B] Quads[C]}",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 index = U8(arguments[0].value);
            U64 min = cpu.registers.longs[U8(arguments[1].value)];
            U64 max = cpu.registers.longs[U8(arguments[2].value)];
            cpu.registers.longs[index] = random(min, max);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1DF0] = S<Operation>(new Operation(
        "Compare", "CMP32", "Quads[A] ? Quads[B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::compare(cpu.registers.longs, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1F00] = S<Operation>(new Operation(
        "Write Long", "WINT64", "out Longs[A]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            std::stringstream buffer {};
            buffer << std::dec << cpu.registers.longs[register_index];
            out(machine.out, buffer, cpu.flag_debug);
        },
        { { SIZE8 } }));
    cpu.operations[0x1F01] = S<Operation>(new Operation(
        "Read Long", "RINT64", "in Longs[A]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U64 value;
            std::stringstream buffer;
            U8 register_index = U8(arguments[0].value);
            buffer << in(machine.out, machine.in, cpu.flag_debug);
            buffer >> std::dec >> value;
            cpu.registers.longs[register_index] = value;
        },
        { { SIZE8 } }));
}