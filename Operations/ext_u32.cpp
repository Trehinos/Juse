

#include "operations.h"

template <Juse::IsWord T>
void Juse::setWord<Juse::U32>(Juse::Operation&, Juse::Instruction&, Juse::GeneralRegisters<T>&);

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
        "Write Utf-32", "WUTF32", "out S32 [A]",
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