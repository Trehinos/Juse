

#include "Operation.h"
#include "Machine.h"
#include "Cpu.h"

Juse::Cpu::Cpu() :
	pool_pointer(0),
	segment_pointer(0),
	instruction_pointer(0),
	flag_exit(false)
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

Juse::U64 Juse::Cpu::addressPointer()
{
	return Address::with(pool_pointer, segment_pointer, instruction_pointer);
}

void Juse::Cpu::jump(U16 pool, U32 segment, U16 instruction)
{
	pool_pointer = pool;
	segment_pointer = segment;
	instruction_pointer = instruction;
}
void Juse::Cpu::longjump(U64 address)
{
	Address a = Address::from(address);
	pool_pointer = a.pool;
	segment_pointer = a.segment;
	instruction_pointer = a.datum;
}

bool Juse::Cpu::shouldExit()
{
	return flag_exit;
}

void Juse::Cpu::cycle(Machine& machine)
{
	U16 identifier = 0;
	S<Juse::Operation> current = machine.getOperation(identifier);
	Instruction instruction = {ByteSet{U8((MASK_16TOP8 & identifier) >> 8), U8(MASK_BOTTOM8 & identifier)}};
	ByteSet toAdd = machine.read(current->length() - 2);
	for (U8 add : toAdd) {
		instruction.data.push_back(add);
	}
	(*current)(machine, instruction);
}

Juse::U8 Juse::Cpu::dataAt(Memory& memory, U64 address)
{
	Address a = Address::from(address);
	return (*(*memory[a.pool])[a.segment])[a.datum];
}

Juse::U8 Juse::Cpu::data(Memory& memory)
{
	return dataAt(memory, addressPointer());
}

Juse::U16 Juse::Cpu::pool()
{
	return pool_pointer;
}

Juse::U16 Juse::Cpu::segment()
{
	return segment_pointer;
}

Juse::U16 Juse::Cpu::instruction()
{
	return instruction_pointer;
}

Juse::S<Juse::Operation> Juse::Cpu::NoOp = S<Operation>(new Operation(
	"NOP",
	"",
	[] (Machine& machine, Instruction& instruction, Operation& operation) { },
	{}
));

void Juse::Cpu::initOperations()
{
	operations[0x0000] = NoOp;
}