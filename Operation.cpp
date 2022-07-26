#include <iostream>

#include "Operation.h"

using namespace Juse;

Operation::Operation(
	std::string mnemu,
	std::string ecal,
	OperationFunction code,
	std::vector<Argument> arguments
) : mnemu(mnemu), ecal(ecal), code(code), arguments(arguments)
{ }

U64 Operation::argument(Instruction instruction, size_t index)
{
	size_t offset = 0;
	for (size_t i = 0; i < index; i++) {
		offset += arguments[i].size;
	}
	return instruction.argument(offset, arguments[index].size);
}

size_t Operation::length()
{
	size_t size = 2;
	for (Argument argument : arguments) {
		size += argument.size;
	}
	return size;
}

void Operation::operator()(Machine& machine, Instruction instruction)
{
	code(machine, instruction, *this);
}

