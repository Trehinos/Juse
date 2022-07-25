#include "Operation.h"

#include <iostream>

Juse::Operation::Operation(
	std::string mnemu,
	std::string ecal,
	OperationFunction code,
	std::vector<Argument> arguments
) : mnemu(mnemu), ecal(ecal), code(code), arguments(arguments)
{ }

Juse::U64 Juse::Operation::argument(Instruction instruction, size_t index)
{
	size_t offset = 0;
	for (size_t i = 0; i < index; i++) {
		offset += arguments[i].size;
	}

	return instruction.argument(offset, arguments[index].size);
}

size_t Juse::Operation::length()
{
	size_t size = 2;
	for (Argument argument : arguments) {
		size += argument.size;
	}
	return size;
}

void Juse::Operation::operator()(Juse::Machine& machine, Instruction instruction)
{
	code(machine, instruction, *this);
}

