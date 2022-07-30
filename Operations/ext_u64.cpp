

#include "operations.h"

template <Juse::IsWord T>
void Juse::setWord<Juse::U64>(Juse::Operation&, Juse::Instruction&, Juse::GeneralRegisters<T>&);

/* 1Cxx-1Fxx */
void Juse::Operations::StandardExtensions::ext_u64(Cpu& cpu)
{
    cpu.operations[0x1C00] = S<Operation>(new Operation(
        "Set Long", "SET64", "Longs[A] = B",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            setWord<U64>(operation, instruction, machine.cpu.registers.longs);
        },
        { { SIZE8 }, { SIZE64 } }));

    cpu.operations[0x1C01] = S<Operation>(new Operation(
        "Copy Long From", "COPYFROM64", "Longs[A] = [B]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U8 register_index = U8(operation.argument(instruction, 0));
            U16 address = U16(operation.argument(instruction, 1));
            U64 value = set2word(machine.readData(address, SIZE64));
            machine.cpu.registers.longs[register_index] = value;
        },
        { { SIZE8 }, { SIZE16 } }));

    cpu.operations[0x1C02] = S<Operation>(new Operation(
        "Copy Long To", "COPYTO64", "[A] = Longs[B]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U16 address = U16(operation.argument(instruction, 0));
            U8 register_index = U8(operation.argument(instruction, 1));
            machine.writeData(address, word2set<U64>(machine.cpu.registers.bytes[register_index]));
        },
        { { SIZE16 }, { SIZE8 } }));
}