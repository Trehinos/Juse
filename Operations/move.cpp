

#include "operations.h"

/* 01xx */
void Juse::Operations::Standard::move(Cpu& cpu)
{
    cpu.operations[0x0100] = S<Operation>(new Operation(
        "Set Data Pool", "DATAPOOL", "datapool A",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            machine.cpu.data_pool = U16(operation.argument(instruction, 0));
        },
        { { SIZE16 } }));
    cpu.operations[0x0101] = S<Operation>(new Operation(
        "Set Data Segment", "DATASEG", "dataset A",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            machine.cpu.data_segment = U32(operation.argument(instruction, 0));
        },
        { { SIZE32 } }));
    cpu.operations[0x0102] = S<Operation>(new Operation(
        "Set Address Pointer", "DATAPTR", "dataptr A",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            machine.cpu.address_pointer = U16(operation.argument(instruction, 0));
        },
        { { SIZE16 } }));
    cpu.operations[0x0103] = S<Operation>(new Operation(
        "Set Address Offset", "DATAOFF", "dataoffset A",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            machine.cpu.address_offset = U16(operation.argument(instruction, 0));
        },
        { { SIZE16 } }));
    cpu.operations[0x01F0] = S<Operation>(new Operation(
        "Push Data Segment", "PUSHDS", "push DS",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            machine.multiPush(word2set(machine.cpu.data_segment));
        },
        {}));

    cpu.operations[0x01F1] = S<Operation>(new Operation(
        "Pop Data Segment", "POPDS", "pop DS",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            machine.cpu.data_segment = U32(set2word(machine.multiPop(SIZE32)));
        },
        {}));
}