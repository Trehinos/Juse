
#include "ext_u32.h"

namespace Juse::Operations::ExtU32 {

/* 18xx-1Axx */
void add(OperationMap& operations)
{
    // 18xx - U32 Moves & Casts
    operations[0x1800] = Set32;
    operations[0x1801] = CopyFrom32;
    operations[0x1802] = CopyTo32;
    operations[0x1803] = Copy32;
    operations[0x1804] = Push32;
    operations[0x1805] = Pop32;
    operations[0x1806] = CastTo32;
    operations[0x1807] = CastFrom32;
    operations[0x1808] = Copy32If;

    // 19xx - U32 Operations
    operations[0x1900] = Add32;
    operations[0x1901] = Substract32;
    operations[0x1902] = Multiply32;
    operations[0x1903] = Divide32;
    operations[0x1904] = Modulo32;

    // TODO : 1905 - ABS32

    operations[0x1906] = Random32;

    operations[0x19F0] = Compare32;

    // 1Bxx - U32 I/O
    operations[0x1B00] = Write32;
    operations[0x1B01] = Read32;

    operations[0x1B10] = WriteUtf32;
}

void init()
{
    // 18xx - U32 Moves & Casts
    Set32 = SPtr<Operation>(new Operation(
        "Set Quad", "SET32", "Quads[A] = B",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Juse::Operations::Unsigned::set(cpu.registers.quads, arguments);
        },
        { { SIZE8 }, { SIZE32 } }));

    CopyFrom32 = SPtr<Operation>(new Operation(
        "Copy Quad From", "COPYFROM32", "Quads[A] = [B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            U32 address = U32(arguments[1].value);
            U32 value = U32(set2word(machine.readData(cpu, address, SIZE32)));
            cpu.registers.quads[register_index] = value;
        },
        { { SIZE8 }, { SIZE32 } }));

    CopyTo32 = SPtr<Operation>(new Operation(
        "Copy Quad To", "COPYTO32", "[A] = Quads[B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U32 address = U32(arguments[0].value);
            U8 register_index = U8(arguments[1].value);
            machine.writeData(cpu, address, word2set(cpu.registers.quads[register_index]));
        },
        { { SIZE32 }, { SIZE8 } }));

    Copy32 = SPtr<Operation>(new Operation(
        "Copy Quad", "COPY32", "Quads[A] = Quads[B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::copy(cpu.registers.quads, arguments);
        },
        { { SIZE8 }, { SIZE8 } }));

    Push32 = SPtr<Operation>(new Operation(
        "Push Quad", "PUSH32", "push Quads[A]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::push(cpu, cpu.registers.quads, arguments);
        },
        { { SIZE8 } }));

    Pop32 = SPtr<Operation>(new Operation(
        "Pop Quad", "POP32", "Quads[A] = {pop}",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::pop(cpu, cpu.registers.quads, arguments);
        },
        { { SIZE8 } }));

    CastTo32 = SPtr<Operation>(new Operation(
        "Cast To Quad", "CAST8TO32", "Quads[A] = Words[A]:Words[B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 rW = U8(arguments[0].value);
            U8 rBA = U8(arguments[1].value);
            U8 rBB = U8(arguments[2].value);
            cpu.registers.quads[rW] = cpu.registers.words[rBA] << sizes.at(SIZE16) | cpu.registers.words[rBB];
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    CastFrom32 = SPtr<Operation>(new Operation(
        "Cast From Quad", "CAST32TO8", "Words[A]:Words[B] = Quads[A]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 rBA = U8(arguments[0].value);
            U8 rBB = U8(arguments[1].value);
            U8 rW = U8(arguments[2].value);
            U32 w = cpu.registers.quads[rW];
            cpu.registers.words[rBA] = U16(U32(w & MASK_32TOP16) >> sizes.at(SIZE8));
            cpu.registers.words[rBB] = U16(w & MASK_BOTTOM16);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    Copy32If = SPtr<Operation>(new Operation(
        "Copy Quad If", "COPY32IF", "?A : Quads[B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            CompareFlag flag = CompareFlag(U8(arguments[0].value));
            if (cpu.registers.compareFlags[flag]) {
                U8 register_index = U8(arguments[1].value);
                machine.writeData(cpu, cpu.offseted(), word2set(cpu.registers.quads[register_index]));
            }
        },
        { { SIZE8 }, { SIZE8 } }));

    // 19xx - U32 Operations
    Add32 = SPtr<Operation>(new Operation(
        "Add", "ADD32", "Quads[A] = Quads[B] + Quads[C] CR Quads[D]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::add<U32, U64>(cpu.registers.quads, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    Substract32 = SPtr<Operation>(new Operation(
        "Substract", "SUB32", "Quads[A] = Quads[B] - Quads[C]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::substract<U32, U64>(cpu.registers.quads, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    Multiply32 = SPtr<Operation>(new Operation(
        "Multiply", "MUL32", "Quads[A] = Quads[B] * Quads[C] CR Quads[D]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::multiply<U32, U64>(cpu.registers.quads, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    Divide32 = SPtr<Operation>(new Operation(
        "Divide", "DIV32", "Quads[A] = Quads[B] / Quads[C]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::divide<U32, U64>(cpu.registers.quads, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    Modulo32 = SPtr<Operation>(new Operation(
        "Modulo", "MOD32", "Quads[A] = Quads[B] % Quads[C]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::modulo<U32, U64>(cpu.registers.quads, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    // TODO : 1905 - ABS32

    Random32 = SPtr<Operation>(new Operation(
        "Random", "RND32", "Quads[A] = {rnd Quads[B] Quads[C]}",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 index = U8(arguments[0].value);
            U32 min = cpu.registers.quads[U8(arguments[1].value)];
            U32 max = cpu.registers.quads[U8(arguments[2].value)];
            cpu.registers.quads[index] = random(min, max);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    Compare32 = SPtr<Operation>(new Operation(
        "Compare", "CMP32", "Quads[A] ? Quads[B]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::compare(cpu.registers.quads, cpu.registers.compareFlags, arguments);
        },
        { { SIZE8 }, { SIZE8 } }));

    // 1Bxx - U32 I/O
    Write32 = SPtr<Operation>(new Operation(
        "Write Quad", "WINT32", "out Quads[A]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            SS8 buffer {};
            buffer << std::dec << cpu.registers.quads[register_index];
            out(machine.out, buffer, cpu.flag_debug);
        },
        { { SIZE8 } }));
    Read32 = SPtr<Operation>(new Operation(
        "Read Quad", "RINT32", "in Quads[A]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U32 value;
            SS8 buffer;
            U8 register_index = U8(arguments[0].value);
            buffer << in(machine.out, machine.in, cpu.flag_debug);
            buffer >> std::dec >> value;
            cpu.registers.quads[register_index] = value;
        },
        { { SIZE8 } }));

    WriteUtf32 = SPtr<Operation>(new Operation(
        "Write Utf-32", "WUTF32", "out S32 A",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            // TODO convert encoding
            U32 address = U32(arguments[0].value);
            U32 offset = 0;
            SS8 buffer {};
            U32 character {};
            Converter<CH32, CH8> CH32_CH8;
            do {
                character = CH32(U32(set2word(machine.readData(cpu, address + offset, SIZE32))));
                if (character != u'\0') {
                    buffer << CH32_CH8.convert(character);
                }
                offset++;
            } while (character != u'\0');
            out(machine.out, buffer, cpu.flag_debug);
        },
        { { SIZE32 } }));
}
}