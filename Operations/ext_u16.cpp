

#include "operations.h"

template <Juse::IsWord T>
void Juse::setWord<Juse::U16>(Juse::Operation&, Juse::Instruction&, Juse::GeneralRegisters<T>&);

template <Juse::IsWord T>
T Juse::random<Juse::U16>(T, T);

template <Juse::IsWord T>
Juse::CompareFlags Juse::compare<Juse::U16>(T a, T b);

/* 14xx-17xx */
void Juse::Operations::StandardExtensions::ext_u16(Cpu& cpu)
{
    // 14xx - U16 Moves & Casts
    cpu.operations[0x1400] = S<Operation>(new Operation(
        "Set Word", "SET16", "Words[A] = B",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            setWord<U16>(operation, instruction, machine.cpu.registers.words);
        },
        { { SIZE8 }, { SIZE16 } }));

    cpu.operations[0x1401] = S<Operation>(new Operation(
        "Copy Word From", "COPYFROM16", "Words[A] = [B]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U8 register_index = U8(operation.argument(instruction, 0));
            U16 address = U16(operation.argument(instruction, 1));
            U16 value = U16(set2word(machine.readData(address, SIZE16)));
            machine.cpu.registers.words[register_index] = value;
        },
        { { SIZE8 }, { SIZE16 } }));

    cpu.operations[0x1402] = S<Operation>(new Operation(
        "Copy Word To", "COPYTO16", "[A] = Words[B]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U16 address = U16(operation.argument(instruction, 0));
            U8 register_index = U8(operation.argument(instruction, 1));
            machine.writeData(address, word2set<U16>(machine.cpu.registers.words[register_index]));
        },
        { { SIZE16 }, { SIZE8 } }));

    // 15xx - U16 Operations
    cpu.operations[0x1500] = S<Operation>(new Operation(
        "Add", "ADD16", "Words[A] = Words[B] + Words[C], Words[D]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U16 rB = machine.cpu.registers.words[U8(operation.argument(instruction, 1))];
            U16 rC = machine.cpu.registers.words[U8(operation.argument(instruction, 2))];
            U32 result = rB + rC;
            machine.cpu.registers.words[U8(operation.argument(instruction, 0))] = result & MASK_BOTTOM16;
            machine.cpu.registers.words[U8(operation.argument(instruction, 4))] = (result & MASK_32TOP16) >> 16;
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1501] = S<Operation>(new Operation(
        "Substract", "SUB16", "Words[A] = Words[B] - Words[C]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U16 rB = machine.cpu.registers.words[U8(operation.argument(instruction, 1))];
            U16 rC = machine.cpu.registers.words[U8(operation.argument(instruction, 2))];
            U16 result = rB - rC;
            machine.cpu.registers.words[U8(operation.argument(instruction, 0))] = result;
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1502] = S<Operation>(new Operation(
        "Multiply", "MUL16", "Words[A] = Words[B] * Words[C], Words[D]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U16 rB = machine.cpu.registers.words[U8(operation.argument(instruction, 1))];
            U16 rC = machine.cpu.registers.words[U8(operation.argument(instruction, 2))];
            U32 result = rB * rC;
            machine.cpu.registers.words[U8(operation.argument(instruction, 0))] = result & MASK_BOTTOM16;
            machine.cpu.registers.words[U8(operation.argument(instruction, 4))] = (result & MASK_32TOP16) >> 16;
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1503] = S<Operation>(new Operation(
        "Divide", "DIV16", "Words[A] = Words[B] / Words[C]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            // TODO DIV a, b, 0, d

            U16 rB = machine.cpu.registers.words[U8(operation.argument(instruction, 1))];
            U16 rC = machine.cpu.registers.words[U8(operation.argument(instruction, 2))];
            U16 result = rB / rC;
            machine.cpu.registers.words[U8(operation.argument(instruction, 0))] = result;
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1504] = S<Operation>(new Operation(
        "Modulo", "MOD16", "Words[A] = Words[B] % Words[C]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U16 rB = machine.cpu.registers.words[U8(operation.argument(instruction, 1))];
            U16 rC = machine.cpu.registers.words[U8(operation.argument(instruction, 2))];
            U16 result = rB % rC;
            machine.cpu.registers.words[U8(operation.argument(instruction, 0))] = result;
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    // TODO : 1505 - ABD16

    cpu.operations[0x1506] = S<Operation>(new Operation(
        "Random", "RND16", "Words[A] = {RND(B, C)}",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U8 index = U8(operation.argument(instruction, 0));
            U16 min = machine.cpu.registers.words[U8(operation.argument(instruction, 1))];
            U16 max = machine.cpu.registers.words[U8(operation.argument(instruction, 2))];
            machine.cpu.registers.words[index] = random<U16>(min, max);
        },
        { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x15F0] = S<Operation>(new Operation(
        "Compare", "CMP16", "Words[A] ? Words[B]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U8 iA = U8(operation.argument(instruction, 0));
            U8 iB = U8(operation.argument(instruction, 1));
            U16 rA = machine.cpu.registers.words[iA];
            U16 rB = machine.cpu.registers.words[iB];
            machine.cpu.registers.compareFlags = compare<U16>(rA, rB);
        },
        { { SIZE8 }, { SIZE8 } }));

    // 17xx - U16 I/O
    cpu.operations[0x1700] = S<Operation>(new Operation(
        "Write Word", "WINT16", "out Words[A]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U8 register_index = U8(operation.argument(instruction, 0));
            std::stringstream buffer {};
            buffer << std::dec << machine.cpu.registers.words[register_index];
            out(machine.out, buffer, machine.cpu.flag_debug);
        },
        { { SIZE8 } }));
    cpu.operations[0x1701] = S<Operation>(new Operation(
        "Read Word", "RINT16", "in Words[A]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U16 value;
            std::stringstream buffer;
            U8 register_index = U8(operation.argument(instruction, 0));
            buffer << in(machine.out, machine.in, machine.cpu.flag_debug);
            buffer >> std::dec >> value;
            machine.cpu.registers.words[register_index] = value;
        },
        { { SIZE8 } }));

    cpu.operations[0x1710] = S<Operation>(new Operation(
        "Write Utf-16", "WUTF16", "out S16 [A]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            // TODO convert encoding
            U16 address = U16(operation.argument(instruction, 0));
            U16 offset = 0;
            SS8 buffer {};
            U16 character {};
            Converter<CH16, CH8> CH16_CH8;
            do {
                character = CH16(U16(set2word(machine.readData(address + offset, SIZE16))));
                if (character != u'\0') {
                    buffer << CH16_CH8.convert(character);
                }
                offset++;
            } while (character != u'\0');
            out(machine.out, buffer, machine.cpu.flag_debug);
        },
        { { SIZE16 } }));
}