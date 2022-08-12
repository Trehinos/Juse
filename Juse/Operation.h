#pragma once

#include "types.h"

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
    std::string juse_lang;
    std::vector<Argument> arguments;
    OperationFunction code;

public:
    Operation(std::string = {}, std::string = {}, std::string = {},
        OperationFunction = {}, std::vector<Argument> = {});
    std::string getName() { return name; }
    std::string jumne() { return mnemu; }
    std::string juseLang() { return juse_lang; }
    U64 argument(Instruction&, size_t = 0);
    OperationArguments operationArguments(Instruction&);
    std::vector<Argument> getArgumentDefs();
    size_t length();
    void operator()(Machine&, Cpu&, Instruction&);
    void operator()(Machine&, Cpu&, OperationArguments&);
};

} // namespace Juse