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
                Declaration declaration{ addressPointer, 0 };
                Declaration& d = declaration;
                WordDeclaration<U8> wordU8{ addressPointer, 0 };
                WordDeclaration<U16> wordU16(addressPointer, 0);
                WordDeclaration<U32> wordU32(addressPointer, 0);
                WordDeclaration<U64> wordU64(addressPointer, 0);
                switch (defType) {
                case 'B':
                    d = wordU8;
                    break;

                case 'W':
                    d = wordU16;
                    break;

                case 'Q':
                    d = wordU32;
                    break;

                case 'L':
                    d = wordU64;
                    break;
                default:
                    throw ParseError{ currentLine, addressPointer, "Invalid declaration type" };
                }

                // TODO --> value

                declarations.push_back(d);
            }
            else {
                throw ParseError{ currentLine, addressPointer, "Preprocessor directive keyword invalid" };
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