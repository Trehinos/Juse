#include "Parser.h"
#include "Machine.h"

namespace Juse {

    S8 Jumne::Preprocessor::transform(S8& line)
    {
        if (line.starts_with("#")) {
            S8 directive = line;
            Strings::trim(directive, "#");
            if (directive.starts_with(KEYWORD_DEFINE)) {
                directive.erase(KEYWORD_DEFINE.size());
                S8 defName;
                S8 defValue;
                std::tie(defName, defValue) = Strings::splitFirst(directive, " ");
                Definition def(defName, defValue);
                definitions.push_back(def);
            }
            else if (directive.starts_with(KEYWORD_DECLARE)) {
                directive.erase(KEYWORD_DECLARE.size());
                CH8 defType = directive.at(0);
                bool hasArguments = false;
                directive.erase(1);
                if (directive.size() > 0) {
                    hasArguments = true;
                    directive.erase(1);
                    if (directive.at(0) != ' ' || directive.size() < 1) throw ParseError(currentLine, *addressPointer, "Invalid declaration format");
                }

                Declaration declaration{ *addressPointer, 0 };
                Declaration& d = declaration;
                switch (defType) {
                case 'B':
                    d = WordDeclaration<U8>{ *addressPointer, 0 };
                    break;

                case 'W':
                    d = WordDeclaration<U16>{ *addressPointer, 0 };
                    break;

                case 'Q':
                    d = WordDeclaration<U32>{ *addressPointer, 0 };
                    break;

                case 'L':
                    d = WordDeclaration<U64>{ *addressPointer, 0 };
                    break;

                default: throw ParseError{ currentLine, *addressPointer, "Invalid declaration type" };
                }
                if (hasArguments) {
                    d.fromString(directive);
                }

                declarations.push_back(d);
            }
            else {
                throw ParseError{ currentLine, *addressPointer, "Preprocessor directive keyword invalid" };
            }
            return "";
        }

        Vector<S8> output{};
        for (auto& token : Strings::split(line, " ")) {
            for (auto& def : definitions) {
                if (def.get().first == token) {
                    token = def.get().second;
                }
            }
            output.push_back(token);
        }

        return Strings::join(output, " ");
    }
}