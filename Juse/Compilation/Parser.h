#pragma once

#include "Token.h"

namespace Juse::Compilation {

    class Parser {
    protected:
        Collection<Token::Rule> rules;
    public:
        virtual Collection<Token::Token> parse(S8) { return {}; }
        Parser(Collection<Token::Rule> r = {}) : rules(r) {}
    };


    namespace Tokens {
        using namespace Token;

        static Terminal SPACE{ "space", " " };
        static Terminal NL{ "newline", "\n " };
        static OneOf SEPARATOR{ "separators", { ref(SPACE), ref(NL) } };

        static Operator OP_PLUS{ "+" };

        static Regex ALPHA{ "alpha", "[A-Za-z]" };
        static Regex NUMERIC{ "numeric", "[0-9]" };

        static OneOf ALPHANUM{ "alphaOrNum", { ref(ALPHA), ref(NUMERIC) } };
    }

}
