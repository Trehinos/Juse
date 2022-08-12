#pragma once

#include "Symbol.h"

namespace Juse {

    class Parser {
    protected:
        Symbol::Grammar grammar;
        Parser(Symbol::Grammar g = {}) : grammar(g) {}
    public:
        virtual Collection<Symbol::Token> parse(std::string);
    };


    namespace Jumne::Tokens {
        using namespace Symbol;

        Terminal SPACE{ "space", " " };
        Terminal NL{ "newline", "\n " };
        OneOf SEPARATOR{ "separators", { wrap(SPACE), wrap(NL) } };

        Operator OP_PLUS{ "+" };

        Regex ALPHA{ "alpha", "[A-Za-z]" };
        Regex NUMERIC{ "numeric", "[0-9]" };

        OneOf ALPHANUM{ "alphaOrNum", { wrap(ALPHA), wrap(NUMERIC) } };
    }

}
