
#include "ext_u64.h"

namespace Juse::Operations::ExtU64 {

    /* 1Cxx-1Fxx */
    void add(OperationMap& operations)
    {
        // 1Cxx - U64 Moves & Casts
        operations[0x1C00] = Set64;
        operations[0x1C01] = CopyFrom64;
        operations[0x1C02] = CopyTo64;
        operations[0x1C03] = Copy64;
        operations[0x1C04] = Push64;
        operations[0x1C05] = Pop64;
        operations[0x1C06] = CastTo64;
        operations[0x1C07] = CastFrom64;
        operations[0x1C08] = Copy64If;

        // 1Dxx - U64 Operations
        operations[0x1D00] = Add64;
        operations[0x1D01] = Substract64;
        operations[0x1D02] = Multiply64;
        operations[0x1D03] = Divide64;
        operations[0x1D04] = Modulo64;

        // TODO : 1D05 - ABS64

        operations[0x1D06] = Random64;

        operations[0x1DF0] = Compare64;

        // 1Fxx - U64 I/O
        operations[0x1F00] = Write64;
        operations[0x1F01] = Read64;
    }

    void init()
    {
        // 1Cxx - U64 Moves & Casts
        Set64 = S<Operation>(new Operation(
            "Set Word", "SET64", "Words[A] = B",
            [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Juse::Operations::Unsigned::set(cpu.registers.longs, arguments);
        },
            { { SIZE8 }, { SIZE64 } }));

        CopyFrom64 = S<Operation>(new Operation(
            "Copy Word From", "COPYFROM64", "Words[A] = [B]",
            [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            U16 address = U16(arguments[1].value);
            U64 value = U64(set2word(machine.readData(cpu, address, SIZE64)));
            cpu.registers.longs[register_index] = value;
        },
            { { SIZE8 }, { SIZE64 } }));

        CopyTo64 = S<Operation>(new Operation(
            "Copy Word To", "COPYTO64", "[A] = Words[B]",
            [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U16 address = U16(arguments[0].value);
            U8 register_index = U8(arguments[1].value);
            machine.writeData(cpu, address, word2set(cpu.registers.longs[register_index]));
        },
            { { SIZE64 }, { SIZE8 } }));

        Copy64 = S<Operation>(new Operation(
            "Copy Word", "COPY64", "Words[A] = Words[B]",
            [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::copy(cpu.registers.longs, arguments);
        },
            { { SIZE8 }, { SIZE8 } }));

        Push64 = S<Operation>(new Operation(
            "Push Word", "PUSH64", "push Words[A]",
            [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::push(cpu, cpu.registers.longs, arguments);
        },
            { { SIZE8 } }));

        Pop64 = S<Operation>(new Operation(
            "Pop Word", "POP64", "Words[A] = {pop}",
            [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::pop(cpu, cpu.registers.longs, arguments);
        },
            { { SIZE8 } }));

        CastTo64 = S<Operation>(new Operation(
            "Cast To Word", "CAST8TO64", "Words[A] = Bytes[A]:Bytes[B]",
            [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 rW = U8(arguments[0].value);
            U8 rBA = U8(arguments[1].value);
            U8 rBB = U8(arguments[2].value);
            cpu.registers.longs[rW] = U64(U64(cpu.registers.quads[rBA]) << sizes.at(SIZE32)) | cpu.registers.quads[rBB];
        },
            { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

        CastFrom64 = S<Operation>(new Operation(
            "Cast From Word", "CAST64TO8", "Bytes[A]:Bytes[B] = Words[A]",
            [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 rBA = U8(arguments[0].value);
            U8 rBB = U8(arguments[1].value);
            U8 rW = U8(arguments[2].value);
            U64 w = cpu.registers.longs[rW];
            cpu.registers.quads[rBA] = U32((w & MASK_64TOP32) >> sizes.at(SIZE32));
            cpu.registers.quads[rBB] = U32(w & MASK_BOTTOM32);
        },
            { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

        Copy64If = S<Operation>(new Operation(
            "Copy Word If", "COPY64IF", "?A : Words[B]",
            [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            CompareFlag flag = CompareFlag(U8(arguments[0].value));
            if (cpu.registers.compareFlags[flag]) {
                U8 register_index = U8(arguments[1].value);
                machine.writeData(cpu, cpu.offseted(), word2set(cpu.registers.longs[register_index]));
            }
        },
            { { SIZE8 }, { SIZE8 } }));

        // 1Dxx - U64 Operations
        Add64 = S<Operation>(new Operation(
            "Add", "ADD64", "Words[A] = Words[B] + Words[C] CR Words[D]",
            [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::add<U64, U128>(cpu.registers.longs, cpu.registers.compareFlags, arguments);
        },
            { { SIZE8 }, { SIZE8 }, { SIZE8 }, { SIZE8 } }));

        Substract64 = S<Operation>(new Operation(
            "Substract", "SUB64", "Words[A] = Words[B] - Words[C]",
            [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::substract<U64, U128>(cpu.registers.longs, cpu.registers.compareFlags, arguments);
        },
            { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

        Multiply64 = S<Operation>(new Operation(
            "Multiply", "MUL64", "Words[A] = Words[B] * Words[C] CR Words[D]",
            [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::multiply<U64, U128>(cpu.registers.longs, cpu.registers.compareFlags, arguments);
        },
            { { SIZE8 }, { SIZE8 }, { SIZE8 }, { SIZE8 } }));

        Divide64 = S<Operation>(new Operation(
            "Divide", "DIV64", "Words[A] = Words[B] / Words[C]",
            [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::divide<U64, U128>(cpu.registers.longs, cpu.registers.compareFlags, arguments);
        },
            { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

        Modulo64 = S<Operation>(new Operation(
            "Modulo", "MOD64", "Words[A] = Words[B] % Words[C]",
            [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::modulo<U64, U128>(cpu.registers.longs, cpu.registers.compareFlags, arguments);
        },
            { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

        // TODO : 1D05 - ABS64

        Random64 = S<Operation>(new Operation(
            "Random", "RND64", "Words[A] = {rnd Words[B] Words[C]}",
            [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 index = U8(arguments[0].value);
            U64 min = cpu.registers.longs[U8(arguments[1].value)];
            U64 max = cpu.registers.longs[U8(arguments[2].value)];
            cpu.registers.longs[index] = random(min, max);
        },
            { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

        Compare64 = S<Operation>(new Operation(
            "Compare", "CMP64", "Words[A] ? Words[B]",
            [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::compare(cpu.registers.longs, cpu.registers.compareFlags, arguments);
        },
            { { SIZE8 }, { SIZE8 } }));

        // 1Fxx - U64 I/O
        Write64 = S<Operation>(new Operation(
            "Write Word", "WINT64", "out Words[A]",
            [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            std::stringstream buffer{};
            buffer << std::dec << cpu.registers.longs[register_index];
            out(machine.out, buffer, cpu.flag_debug);
        },
            { { SIZE8 } }));
        Read64 = S<Operation>(new Operation(
            "Read Word", "RINT64", "in Words[A]",
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
}