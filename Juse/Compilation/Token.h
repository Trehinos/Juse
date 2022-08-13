#pragma once

#include <regex>

#include "../types.h"

namespace Juse::Compilation {

    namespace Token {
        struct Token;

        struct Token {
            S8 type;
            Token(S8 t = "")
                : type{ "token:" + t }
            {
            }

            virtual bool matches(S8)
            {
                return false;
            }
        };

        struct Terminal : Token {
            S8 value;
            Terminal(S8 t, S8 v)
                : Token{ "terminal:" + t }
                , value{ v }

            {
            }
            virtual bool matches(S8 str)
            {
                return str == value;
            }
        };
        struct Operator : Terminal {
            Operator(S8 v)
                : Terminal{ "operator:" + v, v }

            {
            }
        }; 
        struct Optional : Token {
            Token& token;
            Optional(S8 n, Token& t)
                : Token{ "optional:" + n }, token(t)
            {
            }

            virtual bool matches(S8 s)
            {
                if (s.size() == 0) {
                    return true;
                }

                return token.matches(s);
            }
        };
        struct AllOf : Token {
            Collection<Token> values;
            AllOf(S8 t, Collection<Token> values = {})
                : Token{ "allOf:" + t }
                , values(values)
            {
            }
            AllOf(S8 t, Vector<S8> strs)
                : Token{ "allOf:" + t }
                , values({})
            {
                size_t index = 0;
                for (S8 str : strs) {
                    Terminal token{ t + ":" + std::to_string(index++), str };
                    values.push_back(ref(token));
                }
            }

            virtual bool matches(S8 str)
            {
                for (auto& token : values) {
                    if (!token.get().matches(str)) {
                        return false;
                    }
                }
                return true;
            }
        };
        struct OneOf : Token {
            Collection<Token> values;
            OneOf(S8 t, Collection<Token> values = {})
                : Token{ "oneOf:" + t }
                , values(values)
            {
            }
            OneOf(S8 t, Vector<S8> strs)
                : Token{ "oneOf:" + t }
                , values({})
            {
                size_t index = 0;
                for (S8 str : strs) {
                    Terminal token{ t + ":" + std::to_string(index++), str };
                    values.push_back(ref(token));
                }
            }

            virtual bool matches(S8 str)
            {
                for (auto& token : values) {
                    if (token.get().matches(str)) {
                        return true;
                    }
                }
                return false;
            }
        };
        struct Sequence : Token {
            Collection<Token> values;
            Sequence(S8 t, Collection<Token> values = {})
                : Token{ "sequence:" + t }
                , values(values)
            {
            }
            Sequence(S8 t, Vector<S8> strs)
                : Token{ "sequence:" + t }
                , values({})
            {
                size_t index = 0;
                for (S8 str : strs) {
                    Terminal token{ t + ":" + std::to_string(index++), str };
                    values.push_back(ref(token));
                }
            }

            virtual bool matches(S8 str)
            {
                S8::iterator strBegin = str.begin();
                S8::iterator strCursor = strBegin;
                for (auto& token : values) {
                    S8 cursorString;
                    copy(strBegin, strCursor++, cursorString.begin());
                    if (!token.get().matches(cursorString)) {
                        if (strCursor == str.end()) {
                            return false;
                        }
                        continue;
                    }
                    strBegin = strCursor;
                }
                return true;
            }
        };
        struct Regex : Token {
            S8 expr;
            std::regex regExpr;
            Regex(S8 t, S8 re)
                : Token("regex:" + t)
                , expr{ re }
                , regExpr{ re }
            {
            }
            virtual bool matches(S8 str)
            {
                return std::regex_match(str, regExpr);

            }
        };
        struct MultiRegex : OneOf {
            MultiRegex(S8 t, Vector<S8> exprs = {})
                : OneOf{ "concat:" + t }
            {
                for (S8 expr : exprs) {
                    Regex token{ t, expr };
                    values.push_back(ref(token));
                }
            }
            MultiRegex(S8 t, Collection<Regex> regexs = {})
                : OneOf{ "multiRegex:" + t }
            {
                for (auto& regex : regexs) {
                    values.push_back(regex);
                }
            }
        };
        struct Rule {
            Collection<Token> tokens;
            Rule(Collection<Token> values = {})
                : tokens(values)
            {
            }

            bool matches(S8 str)
            {
                return token(str) != std::nullopt;
            }

            Opt<Token> token(S8 str)
            {
                for (auto& token : tokens) {
                    if (token.get().matches(str)) {
                        return token;
                    }
                }
                return std::nullopt;
            }
        };
    }

}