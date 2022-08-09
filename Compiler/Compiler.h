#pragma once

#include "../Models/types.h"

#include "../Machine/Operation.h"

namespace Juse {

namespace Jumne {

    struct Label {
        Address address;
        std::string label;
        bool defined = true;
    };

    struct Instruction {
        std::string operationKey;
        std::vector<std::string> arguments;
    };

    Jumne::Instruction parse(std::string line);
    ByteSet linkOperation(U16, Operation&, std::vector<std::string>);

    std::optional<Operation> findOperation(OperationMap, std::string, U16&);

    class Compiler {
        OperationMap operations;
        std::vector<Label> labels;

        ByteSet compileLine(Address, std::string);

    public:
        Compiler(OperationMap&);
        Memory compile(std::vector<std::string>);
    };

}

}
