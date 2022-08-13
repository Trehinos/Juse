#pragma once

#include "Token.h"

namespace Juse::Compilation {

    class Parser {
    protected:
        Collection<Token::Rule> rules;
        Parser(Collection<Token::Rule> r = {}) : rules(r) {}
    public:
        virtual Collection<Token::Token> parse(S8) = 0;
    };


    namespace Tokens {
        using namespace Token;

        Terminal SPACE{ "space", " " };
        Terminal NL{ "newline", "\n " };
        OneOf SEPARATOR{ "separators", { ref(SPACE), ref(NL) } };

        Operator OP_PLUS{ "+" };

        Regex ALPHA{ "alpha", "[A-Za-z]" };
        Regex NUMERIC{ "numeric", "[0-9]" };

        OneOf ALPHANUM{ "alphaOrNum", { ref(ALPHA), ref(NUMERIC) } };
    }

}
