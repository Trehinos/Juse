#pragma once

#include "../Juse/Compilation/Compiler.h"

namespace Juse {

    namespace Jumne {

        using namespace Compilation;

        struct JLabel : Label {
            Address addr;
            JLabel(std::string, Address = {});
        };

        using namespace Juse::Compilation::Token;

        using JumneArgument = std::string;
        using JumneArguments = V<JumneArgument>;

        struct JumneInstruction {
            Symbol operation;
            JumneArguments arguments;
        };

        JumneInstruction parse(std::string);
        P<O<R<Operation>>, OperationArguments> find(OperationMap&, JumneInstruction&);
        ByteSet bytesFrom(O<R<Operation>>, OperationArguments&);
        inline ByteSet compileJumne(OperationMap& map, std::string line)
        {
            auto parsed = parse(line);
            auto found = find(map, parsed);
            return bytesFrom(found.first, found.second);
        }

        class JumnePreprocessor : Preprocessor {
            M<std::string, std::string> defines;

        protected:
            virtual void define(std::string);
            virtual void def(std::string, std::string);

        public:
            virtual SourceCode transform(SourceCode);
        };

        class JumneAssembler : Assembler 
        {};

        class JumneCompiler : Compiler {
            OperationMap operations;
            std::vector<Label> labels;

        public:
            JumneCompiler(JumnePreprocessor&, JumneAssembler&);
            virtual SourceCode transform(SourceCode);
        };

    }

}
