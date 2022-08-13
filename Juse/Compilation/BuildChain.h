#pragma once

#include <any>

#include "../types.h"

namespace Juse {

    namespace Compilation {

        struct Symbol
        {
            S8 base;
            S8 prefix;
            S8 suffix;
            Symbol(S8 p, S8 b, S8 s) : base(b), prefix(p), suffix(s) {}
            Symbol(S8 p, S8 b) : Symbol(p, b, "") {}
            Symbol(S8 b) : Symbol("", b) {}
            inline S8 get()
            {
                return prefix + base + suffix;
            }
        };

        struct Label {
            Symbol label;
            bool defined = true;
            virtual Address address() = 0;
            Label(Symbol label, bool defined = true) : label(label), defined(defined) {}
            Label(S8 label, bool defined = true) : Label{ Symbol{label}, defined } {}
        };

        struct Type
        {
            Symbol& name;
        };

        template <class T>
        struct Variable : public Type
        {
            Variable(Symbol& name) : Type(name) {}
            Variable(S8 name) : Variable(Symbol{ name }) {}
            virtual T value()
            {
                return v;
            }

        private:
            T v;
        };

        using SourceCode = Vector<S8>;

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
                transformers.push_back(ref(t));
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
