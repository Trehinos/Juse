#include <iostream>

#include "Operation.h"

using namespace Juse;

std::ostream& Juse::operator<<(std::ostream& os, const U8 c)
{
    os << (int)(unsigned char)c;
    return os;
}

Operation::Operation(std::string name, std::string mnemu, std::string ecal, OperationFunction code, std::vector<Argument> arguments)
    : name(name)
    , mnemu(mnemu)
    , ecal(ecal)
    , code(code)
    , arguments(arguments)
{
}

U64 Operation::argument(Instruction& instruction, size_t index)
{
    size_t offset = 0;
    for (size_t i = 0; i < index; i++) {
        offset += arguments[i].size;
    }
    return instruction.argument(offset, arguments[index].size);
}

OperationArguments Operation::operationArguments(Instruction& instruction)
{
    OperationArguments args {};
    for (size_t i = 0; i < arguments.size(); i++) {
        args.push_back(OperationArgument { i, argument(instruction, i) });
    }
    return args;
}

std::vector<Argument> Juse::Operation::getArgumentDefs() { return arguments; }

size_t Operation::length()
{
    size_t size = 2;
    for (Argument argument : arguments) {
        size += argument.size;
    }
    return size;
}

void Operation::operator()(Machine& machine, Cpu& cpu, Instruction& instruction)
{

    code(machine, cpu, operationArguments(instruction));
}
