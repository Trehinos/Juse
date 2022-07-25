
#include "operations.h"

namespace Juse
{
	void createRegistersOperations(Cpu& cpu)
	{
		cpu.operations[0x0200] = S<Operation>(new Operation(
			"SET8",
			"Set Byte:$1 = $2",
			[] (Machine& machine, Instruction& instruction, Operation& operation) {
				U8 register_index = U8(operation.argument(instruction, 0));
				U8 value = U8(operation.argument(instruction, 1));
				machine.bytes[register_index] = value;
			},
			{{SIZE8}, {SIZE8}}
			));
		cpu.operations[0x0201] = S<Operation>(new Operation(
			"SET16",
			"Set Word:$1 = $2",
			[] (Machine& machine, Instruction& instruction, Operation& operation) {
				U8 register_index = U8(operation.argument(instruction, 0));
				U16 value = U16(operation.argument(instruction, 1));
				machine.words[register_index] = value;
			},
			{{SIZE8}, {SIZE16}}
			));
		cpu.operations[0x0202] = S<Operation>(new Operation(
			"SET32",
			"Set Quad:$1 = $2",
			[] (Machine& machine, Instruction& instruction, Operation& operation) {
				U8 register_index = U8(operation.argument(instruction, 0));
				U32 value = U32(operation.argument(instruction, 1));
				machine.quads[register_index] = value;
			},
			{{SIZE8}, {SIZE32}}
			));
		cpu.operations[0x0203] = S<Operation>(new Operation(
			"SET64",
			"Set Oct:$1 = $2",
			[] (Machine& machine, Instruction& instruction, Operation& operation) {
				U8 register_index = U8(operation.argument(instruction, 0));
				U64 value = operation.argument(instruction, 1);
				machine.octs[register_index] = value;
			},
			{{SIZE8}, {SIZE64}}
			));

		cpu.operations[0x0204] = S<Operation>(new Operation(
			"MOVE8",
			"Move Byte:$1 = $2",
			[] (Machine& machine, Instruction& instruction, Operation& operation) {
				U8 register_index = U8(operation.argument(instruction, 0));
				U64 address = operation.argument(instruction, 1);
				U8 value = U8(set2word(machine.readAt(SIZE8, address)));
				machine.bytes[register_index] = value;
			},
			{{SIZE8}, {SIZE64}}
			));

		cpu.operations[0x0205] = S<Operation>(new Operation(
			"MOVE16",
			"Move Word:$1 = $2",
			[] (Machine& machine, Instruction& instruction, Operation& operation) {
				U8 register_index = U8(operation.argument(instruction, 0));
				U64 address = operation.argument(instruction, 1);
				U16 value = U16(set2word(machine.readAt(SIZE16, address)));
				machine.words[register_index] = value;
			},
			{{SIZE8}, {SIZE64}}
			));

		cpu.operations[0x0206] = S<Operation>(new Operation(
			"MOVE32",
			"Move Quad:$1 = $2",
			[] (Machine& machine, Instruction& instruction, Operation& operation) {
				U8 register_index = U8(operation.argument(instruction, 0));
				U64 address = operation.argument(instruction, 1);
				U32 value = U32(set2word(machine.readAt(SIZE32, address)));
				machine.quads[register_index] = value;
			},
			{{SIZE8}, {SIZE64}}
			));

		cpu.operations[0x0207] = S<Operation>(new Operation(
			"MOVE64",
			"Move Oct:$1 = $2",
			[] (Machine& machine, Instruction& instruction, Operation& operation) {
				U8 register_index = U8(operation.argument(instruction, 0));
				U64 address = operation.argument(instruction, 1);
				U64 value = set2word(machine.readAt(SIZE64, address));
				machine.octs[register_index] = value;
			},
			{{SIZE8}, {SIZE64}}
			));
	}
}