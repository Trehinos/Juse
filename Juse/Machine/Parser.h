#pragma once

#include "../types.h"
#include "../utility.h"

namespace Juse {

    using namespace Utility;
    namespace Compilation {

        using SourceCode = Vector<S8>;

        template <class T>
        struct Transformer {
            virtual T transform(T& v)
            {
                return v;
            }
        };

        using S8Transformer = Transformer<S8>;

        struct ParseError : std::exception
        {
            Address errAddress;
            size_t line;
            ParseError(size_t l, Address errAddress, S8 message)
                : std::exception(message.c_str())
                , errAddress(errAddress)
                , line(l)
            {
            }
        };

        struct Parser : public virtual S8Transformer {
            Parser() : S8Transformer{}, currentLine{ 0 } {}
            SourceCode parse(SourceCode& code)
            {
                SourceCode output{};
                for (S8& line : code) {
                    currentLine++;
                    output.push_back(transform(Strings::trim(line)));
                }
                return output;
            }
        protected:
            size_t currentLine;
        };

    }

    namespace Jumne {
        using namespace Juse::Compilation;

        struct Declaration {
            Address address;
            virtual ByteSet toValue()
            {
                ByteSet output{};
                for (U16 i = 0; i < size; i++) {
                    output.push_back(0);
                }
                return output;
            }
            virtual void fromString(std::string s)
            {
            }
            Declaration(Address a, U16 s) : address(a), size(s) {}
        private:
            U16 size;
        };

        template <IsWord T>
        struct WordDeclaration : public Declaration {
            T value;
            ByteSet toValue()
            {
                return word2set<T>(value);
            }
            void fromString(std::string s)
            {
                U64 v = std::stoull(s, 0);
                value = T(v);
            }
            WordDeclaration(Address a, T v) : Declaration(a, sizeof(T)), value(v) {}
        };

        using Definition = Pair<S8, S8>;

        struct Preprocessor : public Parser {
            inline static const S8 KEYWORD_DEFINE = "DEFINE ";
            inline static const S8 KEYWORD_DECLARE = "D";

            SPtr<Address> addressPointer;
            Collection<Declaration> declarations;
            Collection<Definition> definitions;

            S8 transform(S8&);
            Preprocessor() : Parser{}, addressPointer{ nullptr }, declarations{}, definitions{} {}
        };

        struct Assembler : public Parser {
            Assembler() : Parser{} {}
            Vector<Instruction> assemble(SourceCode code)
            {
                // Todo
                return {};
            }
            static SPtr<Memory> toMemory(Vector<Instruction>& v)
            {
                Address a{};
                SPtr<Memory> memory = makeS<Memory>();
                for (Instruction& i : v) {
                    Utility::MachineMemory::write(memory, i.data, a);
                    Utility::MachineMemory::forward(a, U16(i.data.size()));
                }
                return memory;
            }
        };

        struct Compiler {
            Preprocessor& preprocessor;
            Assembler& assembler;
            SourceCode code;
            SPtr<Memory> output;

            Compiler(Preprocessor& p, Assembler& a, SourceCode code = {})
                : preprocessor(p), assembler(a), code{code}, output{ nullptr }
            {
            }

            inline Compiler* preprocess()
            {
                if (preprocessed) return this;
                code = preprocessor.parse(code);
                return this;
            }
            inline Compiler* assemble()
            {
                if (assembled) return this;
                auto instructions = assembler.assemble(code);
                output = Assembler::toMemory(instructions);
                return this;
            }
            inline SPtr<Memory> compile(SourceCode& c)
            {
                code = c;
                assembled = false;
                preprocessed = false;
                return output = preprocess()->assemble()->output;
            }
        private:
            bool preprocessed = false;
            bool assembled = false;
        };
    }
}