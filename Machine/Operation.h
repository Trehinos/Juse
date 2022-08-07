#pragma once

#include "../Models/types.h"

namespace Juse {

std::ostream& operator<<(std::ostream&, const U8);

struct Argument {
    size_t size = 0;
};

struct OperationArgument {
    size_t index;
    U64 value;
};

class Operation {

    std::string name;
    std::string mnemu;
    std::string ecal;
    std::vector<Argument> arguments;
    OperationFunction code;

public:
    Operation(std::string = {}, std::string = {}, std::string = {},
        OperationFunction = {}, std::vector<Argument> = {});
    std::string getName() { return name; }
    std::string mnemuCode() { return mnemu; }
    std::string ecalCode() { return ecal; }
    U64 argument(Instruction&, size_t = 0);
    OperationArguments operationArguments(Instruction&);
    std::vector<Argument> getArgumentDefs();
    size_t length();
    void operator()(Machine&, Cpu&, Instruction&);
};

} // namespace Juse