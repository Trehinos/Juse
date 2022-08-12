#pragma once

#include "../model/Compiler.h"

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

    class Preprocessor {
        std::map<std::string, std::string> defines;
    protected:
        virtual void define(std::string);
        virtual void def(std::string, std::string);

    public:
        virtual std::vector<std::string> process(std::vector<std::string>);
    };

    class Compiler {
        OperationMap operations;
        Parser& parser;
        std::vector<Label> labels;

        ByteSet compileLine(Address, std::string);

    public:
        Compiler(Parser&, OperationMap&);
        std::vector<std::string> preProcess(std::vector<std::string>);
        Memory compile(std::vector<std::string>);
        inline Memory build(std::vector<std::string> v)
        {
            return compile(preProcess(v));
        }
    };

}

}
