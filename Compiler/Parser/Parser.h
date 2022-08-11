#pragma once

#include <regex>

#include "../../Models/types.h"

namespace Juse {

namespace Parser {

    namespace Symbol {
        struct Token {
            std::string type;
            std::vector<S<Token>> validTokens;
            Token(std::string t, std::vector<S<Token>> vt)
                : type(t)
                , validTokens(vt)
            {
            }

            virtual bool isValid(std::string)
            {
                return false;
            }
            virtual std::string getValue()
            {
                return "";
            }
        };
        struct Terminal : Token {
            std::string value;
            Terminal(std::string t, std::string v, std::vector<S<Token>> vt = {})
                : Token { "terminal:" + t, vt }
                , value { v }

            {
            }
        };
        struct Operator : Terminal {
            Operator(std::string v)
                : Terminal { "operator:" + v, v }

            {
            }
        };
        struct OneOf : Token {
            OneOf(std::string t, std::vector<S<Token>> values = {})
                : Token { "oneOf:" + t, values }
            {
            }
        };
        struct Regex : Token {
            std::regex regExpr;
            Regex(std::string t, std::regex re)
                : Token { "regex:" + t, {} }
                , regExpr(re)
            {
            }
        };
    }

    class Parser {
    public:
        virtual std::vector<Symbol::Token> parse(std::string) = 0;
    };

    class ValueParser : public Parser {
        std::vector<Symbol::Token> parse(std::string);
    };

}

namespace Jumne::Tokens {
    const auto tok = share<Parser::Symbol::Token>;
    Parser::Symbol::Terminal SPACE { "space", " " };
    Parser::Symbol::Terminal NL { "newline", "\n " };
    Parser::Symbol::OneOf SEPARATOR { "separators", { tok(SPACE), tok(NL) } };

    Parser::Symbol::Operator OP_PLUS { "+" };

    Parser::Symbol::Regex ALPHA { "alpha", std::regex { "[A-Za-z]" } };
    Parser::Symbol::Regex NUMERIC { "numeric", std::regex { "[0-9]" } };

    Parser::Symbol::OneOf ALPHANUM { "alphaOrNum", { tok(ALPHA), tok(NUMERIC) } };
}

}
