#pragma once

#include "JTypes.h"
#include "../Machine/Cpu.h"
#include "../Machine/Operation.h"

namespace Juse::Types {
    struct ObjectModel;
    struct ModelMethod : public virtual Data
    {
        Ref<ObjectModel> model;
        enum class Visibility {
            PRIVATE, PROTECTED, PUBLIC
        } visibility;
        enum class Scope {
            INSTANCE, STATIC
        } scope;
        Ref<Data> returnModel;
        ParameterList parameters;
        Vector<Instruction> instructions;
        ModelMethod(
            ObjectModel& m,
            Visibility v,
            Scope s,
            Ref<Data> r,
            S8 name,
            ParameterList parameters,
            Vector<Instruction> instructions
        ) : Data("method:" + name), model(m), visibility(v), scope(s), returnModel(r)
            , parameters{ parameters }, instructions(instructions)
        {
        }
        ModelMethod(ObjectModel& m, S8 name, Vector<Instruction>& instructions, Wrap<Data> r = std::nullopt, ParameterList p = {})
            : ModelMethod(m, Visibility::PUBLIC, Scope::INSTANCE, r.value(), name, p, instructions)
        {
        }
        void operator()(Machine& m, Cpu& c, Data* thisModel)
        {
            // TODO manage "this"
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
