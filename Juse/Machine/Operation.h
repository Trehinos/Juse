#pragma once

#include "../types.h"

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

    S8 name;
    S8 mnemu;
    S8 juse_lang;
    Vector<Argument> arguments;
    OperationFunction code;

public:
    Operation(S8 = {}, S8 = {}, S8 = {},
        OperationFunction = {}, Vector<Argument> = {});
    S8 getName() { return name; }
    S8 jumne() { return mnemu; }
    S8 juseLang() { return juse_lang; }
    U64 argument(Instruction&, size_t = 0);
    OperationArguments operationArguments(Instruction&);
    Vector<Argument> getArgumentDefs();
    size_t length();
    void operator()(Machine&, Cpu&, Instruction&);
    void operator()(Machine&, Cpu&, OperationArguments&);
    void operator()(Machine&, Cpu&, OperationArguments);
};

} // namespace Juse