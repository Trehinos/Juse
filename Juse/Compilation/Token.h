#pragma once

#include <regex>

#include "../types.h"

namespace Juse::Compilation {

    namespace Token {
        struct Token;

        struct Token {
            std::string type;
            Token(std::string t = "")
                : type{ "token:" + t }
            {
            }

            virtual bool matches(std::string)
            {
                return false;
            }
        };

        struct Terminal : Token {
            std::string value;
            Terminal(std::string t, std::string v)
                : Token{ "terminal:" + t }
                , value{ v }

            {
            }
            virtual bool matches(std::string str)
            {
                return str == value;
            }
        };
        struct Operator : Terminal {
            Operator(std::string v)
                : Terminal{ "operator:" + v, v }

            {
            }
        }; 
        struct Optional : Token {
            Token& token;
            Optional(std::string n, Token& t)
                : Token{ "optional:" + n }, token(t)
            {
            }

            virtual bool matches(std::string s)
            {
                if (s.size() == 0) {
                    return true;
                }

                return token.matches(s);
            }
        };
        struct AllOf : Token {
            Collection<Token> values;
            AllOf(std::string t, Collection<Token> values = {})
                : Token{ "allOf:" + t }
                , values(values)
            {
            }
            AllOf(std::string t, std::vector<std::string> strs)
                : Token{ "allOf:" + t }
                , values({})
            {
                size_t index = 0;
                for (std::string str : strs) {
                    Terminal token{ t + ":" + std::to_string(index++), str };
                    values.push_back(wrap(token));
                }
            }

            virtual bool matches(std::string str)
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
            OneOf(std::string t, Collection<Token> values = {})
                : Token{ "oneOf:" + t }
                , values(values)
            {
            }
            OneOf(std::string t, std::vector<std::string> strs)
                : Token{ "oneOf:" + t }
                , values({})
            {
                size_t index = 0;
                for (std::string str : strs) {
                    Terminal token{ t + ":" + std::to_string(index++), str };
                    values.push_back(wrap(token));
                }
            }

            virtual bool matches(std::string str)
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
            Sequence(std::string t, Collection<Token> values = {})
                : Token{ "sequence:" + t }
                , values(values)
            {
            }
            Sequence(std::string t, std::vector<std::string> strs)
                : Token{ "sequence:" + t }
                , values({})
            {
                size_t index = 0;
                for (std::string str : strs) {
                    Terminal token{ t + ":" + std::to_string(index++), str };
                    values.push_back(wrap(token));
                }
            }

            virtual bool matches(std::string str)
            {
                std::string::iterator strBegin = str.begin();
                std::string::iterator strCursor = strBegin;
                for (auto& token : values) {
                    std::string cursorString;
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
            std::string expr;
            std::regex regExpr;
            Regex(std::string t, std::string re)
                : Token("regex:" + t)
                , expr{ re }
                , regExpr{ re }
            {
            }
            virtual bool matches(std::string str)
            {
                return std::regex_match(str, regExpr);

            }
        };
        struct MultiRegex : OneOf {
            MultiRegex(std::string t, V<std::string> exprs = {})
                : OneOf{ "concat:" + t }
            {
                for (std::string expr : exprs) {
                    Regex token{ t, expr };
                    values.push_back(wrap(token));
                }
            }
            MultiRegex(std::string t, Collection<Regex> regexs = {})
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

            bool matches(std::string str)
            {
                return token(str) != std::nullopt;
            }

            O<Token> token(std::string str)
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