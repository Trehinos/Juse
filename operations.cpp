#pragma once

#include "Machine.h"
#include "operations.h"

using namespace Juse;

Operation Operations::NoOp = Operation(
	[] (Machine& machine, Instruction& instruction, Operation& operation) { },
	{}
);
Operation Operations::ShortJump = Operation(
	[] (Machine& machine, Instruction& instruction, Operation& operation) {
		U16 target = MASK_BOTTOM8 & operation.argument(instruction, 0);
		machine.cpu.jump(machine.cpu.pool(), machine.cpu.segment(), target);
	},
	{{SIZE16}}
);
Operation Operations::SegmentJump = Operation(
	[] (Machine& machine, Instruction& instruction, Operation& operation) {
		U32 segment = MASK_BOTTOM32 & operation.argument(instruction, 0);
		U16 target = MASK_BOTTOM16 & operation.argument(instruction, 1);
		machine.cpu.jump(machine.cpu.pool(), segment, target);
	},
	{{SIZE32}, {SIZE16}}
);
Operation Operations::LongJump = Operation(
	[] (Machine& machine, Instruction& instruction, Operation& operation) {
		machine.cpu.longjump(operation.argument(instruction, 0));
	},
	{{SIZE64}}
);

Operation Operations::Call = Operation(
	[] (Machine& machine, Instruction& instruction, Operation& operation) {
		U16 target = MASK_BOTTOM8 & operation.argument(instruction, 0);
		machine.push(0xFF00 & machine.cpu.instruction());
		machine.push(0x00FF & machine.cpu.instruction());
		machine.cpu.jump(machine.cpu.pool(), machine.cpu.segment(), target);
	},
	{{SIZE16}}
);

Operation Operations::Return = Operation(
	[] (Machine& machine, Instruction& instruction, Operation& operation) {
		U16 target = machine.pop() << 8 + machine.pop();
		machine.cpu.jump(machine.cpu.pool(), machine.cpu.segment(), target);
	},
	{{SIZE16}}
);

Operation Operations::LongCall = Operation(
	[] (Machine& machine, Instruction& instruction, Operation& operation) {
		U64 target = instruction.argument(0, SIZE64);
		machine.cpu.longjump(target);
	},
	{{SIZE64}}
);

Operation Operations::LongReturn = Operation(
	[] (Machine& machine, Instruction& instruction, Operation& operation) {
		U64 target =
			U64(machine.pop()) << 56 +
			U64(machine.pop()) << 48 +
			U64(machine.pop()) << 40 +
			U64(machine.pop()) << 32 +
			U64(machine.pop()) << 24 +
			U64(machine.pop()) << 16 +
			U64(machine.pop()) << 8 +
			U64(machine.pop())
			;
		machine.cpu.longjump(target);
	},
	{{SIZE64}}
);

