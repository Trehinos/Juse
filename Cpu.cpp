

#include "Machine.h"
#include "Cpu.h"
#include "operations.h"

Juse::Cpu::Cpu() :
	pool_pointer(0),
	segment_pointer(0),
	instruction_pointer(0)
{ }

void Juse::Cpu::forward()
{
	if (instruction_pointer == UINT16_MAX) {
		instruction_pointer = 0;
		if (segment_pointer == UINT32_MAX) {
			segment_pointer = 0;
			if (pool_pointer == UINT16_MAX) pool_pointer = 0;
			else pool_pointer++;
		} else segment_pointer++;
	} else instruction_pointer++;
}

void Juse::Cpu::cycle(Machine& machine)
{
	U16 identifier = 0;
	Juse::Operation current = machine.getOperation(identifier);
	Instruction instruction = {ByteSet{U8((MASK_16TOP8 & identifier) >> 8), U8(MASK_BOTTOM8 & identifier)}};
	ByteSet toAdd = machine.read(current.length() - 2);
	for (U8 add : toAdd) {
		instruction.data.push_back(add);
	}
	current(machine, instruction);
}


void Juse::Cpu::initOperations()
{
	operations[0x0000] = Operations::NoOp;
	operations[0x0100] = Operations::ShortJump;
	operations[0x0101] = Operations::SegmentJump;
	operations[0x0102] = Operations::LongJump;
	operations[0x0103] = Operations::Return;
	operations[0x0104] = Operations::FarReturn;
	operations[0x0105] = Operations::Call;
	operations[0x0106] = Operations::FarCall;

}