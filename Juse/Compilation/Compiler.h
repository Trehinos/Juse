#pragma once

#include "BuildChain.h"

#include "Parser.h"

namespace Juse::Compilation
{
    using VariableU8 = Variable<U8>;
    using VariableU16 = Variable<U16>;
    using VariableU32 = Variable<U32>;
    using VariableU64 = Variable<U64>;
    using VariableS8 = Variable<S8>;

    using SymbolMap = M<Symbol, Type&>;

    class Preprocessor : public Transformer {
        RefHeap<std::string, Symbol> defines;
        SymbolMap allocations;
        Parser& directiveParser;

    protected:
        Preprocessor(Parser&);
        virtual void define(std::string);
        template <class T&> void d(Variable<T>&);

    public:
        virtual SourceCode transform(SourceCode&);
    };

    class Assembler : public Transformer {
        Parser& instructionParser;
        OperationMap& operations;
        Collection<Label> labels;

    public:
        virtual SourceCode transform(SourceCode&);
    };

    class Compiler : public BuildChain
    {
        Compiler(Preprocessor& p, Assembler& a): BuildChain{}
        {
            chainWith(p).chainWith(a);
        }
    };
}