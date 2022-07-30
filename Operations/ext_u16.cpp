

#include "operations.h"

template <Juse::IsWord T>
void Juse::setWord<Juse::U16>(Juse::Operation&, Juse::Instruction&, Juse::GeneralRegisters<T>&);

/* 14xx-17xx */
void Juse::Operations::StandardExtensions::ext_u16(Cpu& cpu)
{
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
            machine.writeData(address, word2set<U16>(machine.cpu.registers.bytes[register_index]));
        },
        { { SIZE16 }, { SIZE8 } }));
}