#include "Operation.h"

Juse::Operation::Operation(OperationFunction code, std::vector<Argument> arguments): code(code), arguments(arguments)
{ }

Juse::U64 Juse::Operation::argument(Instruction instruction, size_t index = 0)
{
	size_t offset = 0;
	for (size_t i = 0; i < index; i++) {
		offset += arguments[i].size;
	}

	return instruction.argument(offset, arguments[index].size);
}

void Juse::Operation::operator()(Juse::Machine& machine, Instruction instruction)
{ 
	code(machine, instruction, *this);
}
