#pragma once

#include "../Juse/Compilation/Compiler.h"

namespace Juse {

    namespace Jumne {

        using namespace Compilation;

        struct JLabel : Label {
            Address addr;
            JLabel(S8, Address = {});
        };

        using namespace Juse::Compilation::Token;

        using JumneArgument = S8;
        using JumneArguments = Vector<JumneArgument>;

        struct JumneInstruction {
            Symbol operation;
            JumneArguments arguments;
        };

        JumneInstruction parse(S8);
        Pair<Opt<Ref<Operation>>, OperationArguments> find(OperationMap&, JumneInstruction&);
        ByteSet bytesFrom(Wrap<Operation>, OperationArguments&);
        inline ByteSet compileJumne(OperationMap& map, S8 line)
        {
            auto parsed = parse(line);
            auto found = find(map, parsed);
            return bytesFrom(found.first, found.second);
        }

        class JumnePreprocessor : public virtual Preprocessor {
            Map<S8, S8> defines;

        protected:
            virtual void define(S8);
            virtual void def(S8, S8);

        public:
            JumnePreprocessor() : Preprocessor() {}
            virtual SourceCode transform(SourceCode);
        };

        class JumneAssembler : public virtual Assembler
        {
            OperationMap operations;
            Vector<Label> labels;
        public:
            JumneAssembler(OperationMap& m, Parser& p) : Assembler() {}
            SourceCode transform(SourceCode&);
        };

        class JumneCompiler : public virtual Compiler {

        public:
            JumneCompiler(JumnePreprocessor& p, JumneAssembler& a) : Compiler(p, a) {}
            static JumneCompiler create();
            virtual SourceCode transform(SourceCode);
        };

    }

}
