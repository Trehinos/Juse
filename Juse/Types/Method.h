#pragma once

#include "JTypes.h"
#include "../Machine/Cpu.h"
#include "../Machine/Operation.h"

namespace Juse::Types {

    struct ModelMethod : public virtual Data
    {
        enum class Visibility {
            PRIVATE, PROTECTED, PUBLIC
        } visibility;
        enum class Scope {
            INSTANCE, STATIC
        } scale;
        Ref<Data> returnModel;
        Collection<Parameter> parameters;
        Vector<Instruction> instructions;
        ModelMethod(
            Visibility v,
            Scope s,
            Ref<Data> r,
            S8 name,
            ParameterList parameters,
            Vector<Instruction> instructions
        );
        ModelMethod(S8 name, Vector<Instruction>& instructions, Wrap<Data> r = std::nullopt, ParameterList p = {})
            : ModelMethod(Visibility::PUBLIC, Scope::INSTANCE, r.value(), name, p, instructions)
        {
        }
        void operator()(Machine& m, Cpu& c, Data* thisModel)
        {

            for (Instruction& i : instructions) {
                executeInstruction(m, c, i);
            }
        }
        void call(Machine& m, Cpu& c)
        {
            this->operator()(m, c, this);
        }

    private:
        static void executeInstruction(Machine& m, Cpu& c, Instruction& i)
        {
            auto operation = c.findOperation(m, i.identifier());
            (*operation)(m, c, i);
        }
    };

    using MethodMap = HeapMap<S8, ModelMethod>;
}
