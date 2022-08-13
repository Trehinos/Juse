#pragma once

#include <any>

#include "../types.h"

namespace Juse {

    namespace Compilation {

        struct Symbol
        {
            std::string base;
            std::string prefix;
            std::string suffix;
            Symbol(std::string p, std::string b, std::string s) : base(b), prefix(p), suffix(s) {}
            Symbol(std::string p, std::string b) : Symbol(p, b, "") {}
            Symbol(std::string b) : Symbol("", b) {}
            inline std::string get()
            {
                return prefix + base + suffix;
            }
        };

        struct Label {
            Symbol label;
            bool defined = true;
            virtual Address address() = 0;
            Label(Symbol label, bool defined = true) : label(label), defined(defined) {}
            Label(std::string label, bool defined = true) : Label{ Symbol{label}, defined } {}
        };

        struct Type
        {
            Symbol& name;
        };

        template <class T>
        struct Variable : public Type
        {
            Variable(Symbol& name) : Type(name) {}
            Variable(std::string name) : Variable(Symbol{ name }) {}
            virtual T value()
            {
                return v;
            }

        private:
            T v;
        };

        using SourceCode = V<std::string>;

        struct Transformer
        {
            virtual SourceCode transform(SourceCode&) = 0;
        };

        class BuildChain : public Transformer {
            Collection<Transformer> transformers;
        public:
            BuildChain() : transformers{} {}
            BuildChain& chainWith(Transformer& t)
            {
                transformers.push_back(wrap(t));
                return *this;
            }

            virtual SourceCode transform(SourceCode& code)
            {
                SourceCode output = code;
                for (Transformer& transformer : transformers) {
                    output = transformer.transform(code);
                }
                return output;
            }
        };

    }

}
