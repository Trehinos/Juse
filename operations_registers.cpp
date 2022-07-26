
#include "operations.h"

namespace Juse
{
    void createRegistersOperations(Cpu& cpu)
    {
        cpu.operations[0x0200] = S<Operation>(new Operation(
            "Set Byte",
            "SET8",
            "B$1 = $2",
            [](Machine& machine, Instruction& instruction, Operation& operation) {
                U8 register_index = U8(operation.argument(instruction, 0));
                U8 value = U8(operation.argument(instruction, 1));
                machine.bytes[register_index] = value;
            },
            { {SIZE8}, {SIZE8} }
            ));
        cpu.operations[0x0201] = S<Operation>(new Operation(
            "Set Word",
            "SET16",
            "W$1 = $2",
            [](Machine& machine, Instruction& instruction, Operation& operation) {
                U8 register_index = U8(operation.argument(instruction, 0));
                U16 value = U16(operation.argument(instruction, 1));
                machine.words[register_index] = value;
            },
            { {SIZE8}, {SIZE16} }
            ));
        cpu.operations[0x0202] = S<Operation>(new Operation(
            "Set Quad",
            "SET32",
            "Q$1 = $2",
            [](Machine& machine, Instruction& instruction, Operation& operation) {
                U8 register_index = U8(operation.argument(instruction, 0));
                U32 value = U32(operation.argument(instruction, 1));
                machine.quads[register_index] = value;
            },
            { {SIZE8}, {SIZE32} }
            ));
        cpu.operations[0x0203] = S<Operation>(new Operation(
            "Set Long",
            "SET64",
            "L$1 = $2",
            [](Machine& machine, Instruction& instruction, Operation& operation) {
                U8 register_index = U8(operation.argument(instruction, 0));
                U64 value = operation.argument(instruction, 1);
                machine.longs[register_index] = value;
            },
            { {SIZE8}, {SIZE64} }
            ));

        cpu.operations[0x0210] = S<Operation>(new Operation(
            "Copy Byte From",
            "CPYFROM8",
            "B$1 = *$2",
            [](Machine& machine, Instruction& instruction, Operation& operation) {
                U8 register_index = U8(operation.argument(instruction, 0));
                U16 address = U16(operation.argument(instruction, 1));
                U8 value = U8(set2word(machine.readData(address, SIZE8)));
                machine.bytes[register_index] = value;
            },
            { {SIZE8}, {SIZE16} }
            ));

        cpu.operations[0x0211] = S<Operation>(new Operation(
            "Copy Word From",
            "CPYFROM16",
            "W$1 = *$2",
            [](Machine& machine, Instruction& instruction, Operation& operation) {
                U8 register_index = U8(operation.argument(instruction, 0));
                U16 address = U16(operation.argument(instruction, 1));
                U16 value = U16(set2word(machine.readData(address, SIZE16)));
                machine.words[register_index] = value;
            },
            { {SIZE8}, {SIZE16} }
            ));

        cpu.operations[0x0212] = S<Operation>(new Operation(
            "Copy Quad From",
            "CPYFROM32",
            "Q$1 = *$2",
            [](Machine& machine, Instruction& instruction, Operation& operation) {
                U8 register_index = U8(operation.argument(instruction, 0));
                U16 address = U16(operation.argument(instruction, 1));
                U32 value = U32(set2word(machine.readData(address, SIZE32)));
                machine.quads[register_index] = value;
            },
            { {SIZE8}, {SIZE16} }
            ));

        cpu.operations[0x0213] = S<Operation>(new Operation(
            "Copy Long From",
            "CPYFROM64",
            "L$1 = *$2",
            [](Machine& machine, Instruction& instruction, Operation& operation) {
                U8 register_index = U8(operation.argument(instruction, 0));
                U16 address = U16(operation.argument(instruction, 1));
                U64 value = set2word(machine.readData(address, SIZE64));
                machine.longs[register_index] = value;
            },
            { {SIZE8}, {SIZE16} }
            ));
    }
}