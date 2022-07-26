
#include <iomanip>

#include "Operation.h"
#include "Machine.h"
#include "Cpu.h"

using namespace Juse;

Cpu::Cpu() :
	pool_pointer(0),
	segment_pointer(0),
	instruction_pointer(0),
	flag_exit(false)
{ }

void Cpu::forward()
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

U64 Cpu::addressPointer()
{
	return Address::with(pool_pointer, segment_pointer, instruction_pointer);
}

void Cpu::jump(U16 pool, U32 segment, U16 instruction)
{
	pool_pointer = pool;
	segment_pointer = segment;
	instruction_pointer = instruction;
}
void Cpu::longjump(U64 address)
{
	Address a = Address::from(address);
	pool_pointer = a.pool;
	segment_pointer = a.segment;
	instruction_pointer = a.datum;
}

bool Cpu::shouldExit()
{
	return flag_exit;
}

void Cpu::cycle(Machine& machine, bool debug)
{
	U16 identifier = 0;
	if (debug) {
		machine.out << "@" << std::hex << std::right << std::setfill('0') << std::setw(4);
		machine.out << (int)instruction_pointer << " : ";
	}

	S<Operation> current = machine.getOperation(identifier);
	Instruction instruction = {ByteSet{U8((MASK_16TOP8 & identifier) >> 8), U8(MASK_BOTTOM8 & identifier)}};
	ByteSet toAdd = machine.read(current->length() - 2);
	for (U8 add : toAdd) {
		instruction.data.push_back(add);
	}
	if (debug) {
		for (U8 byte : instruction.data) {
			machine.out << std::setw(2) << std::hex << (int)(unsigned char)byte;
		}
		machine.out << " : " << current->getName() << " ";
		size_t index = 0;
		for (Argument argument : current->getArgumentDefs()) {
			machine.out << std::left << std::setfill('0') << std::setw(argument.size) << std::hex << current->argument(instruction, index++);
		}
		machine.out << std::endl;
	}
	(*current)(machine, instruction);
}

U8 Cpu::dataAt(Memory& memory, U64 address)
{
	Address a = Address::from(address);
	return (*(*memory[a.pool])[a.segment])[a.datum];
}

U8 Cpu::data(Memory& memory)
{
	return dataAt(memory, addressPointer());
}

U16 Cpu::pool()
{
	return pool_pointer;
}

U16 Cpu::segment()
{
	return segment_pointer;
}

U16 Cpu::instruction()
{
	return instruction_pointer;
}

S<Operation> Cpu::NoOp = S<Operation>(new Operation(
	"Nothing",
	"NOP",
	"",
	[] (Machine& machine, Instruction& instruction, Operation& operation) { },
	{}
));

void Cpu::initOperations()
{
	operations[0x0000] = NoOp;
}