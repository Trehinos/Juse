

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
            machine.writeData(address, word2set<U32>(machine.cpu.registers.bytes[register_index]));
        },
        { { SIZE16 }, { SIZE8 } }));
}