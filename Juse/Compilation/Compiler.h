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

    using SymbolMap = Map<Symbol, Type&>;

    class Preprocessor : public virtual Transformer {

    public:
        Preprocessor() : Transformer{} {}
        /*
        * Define a new symbol linked to a string.
        */
        virtual void define(Symbol, S8) = 0;

        /*
        * Define a new space for static allocations.
        */
        virtual void d(Address, Type&) = 0;

    };

    class Assembler : public virtual Transformer {
    public:
        Assembler() : Transformer{} {}
    };

    class Compiler : public virtual BuildChain
    {
    public:
        Compiler(Preprocessor& p, Assembler& a) : BuildChain{}
        {
            chainWith(p).chainWith(a);
        }
    };
}