#pragma once

#include "../Machine/Machine.h"
#include "../Machine/Operation.h"

namespace Juse {

template <IsChar From, IsChar To>
struct Converter {
    static To convert(From from) { return To(from); }
};

template <IsWord T>
void setWord(Juse::Operation& operation, Juse::Instruction& instruction, Juse::GeneralRegisters<T>& registers)
{
    U8 register_index = U8(operation.argument(instruction, 0));
    T value = T(operation.argument(instruction, 1));
    registers[register_index] = value;
}

void out(std::ostream&, SS8&, bool);
std::string in(std::ostream&, std::istream&, bool);

namespace Operations {
    namespace Standard {
        void branching(Cpu&);
        void move(Cpu&);
        void logic(Cpu&);
        void allocation(Cpu&);
        void thread(Cpu&);
    }
    namespace StandardExtensions {
        void ext_u8(Cpu&);
        void ext_u16(Cpu&);
        void ext_u32(Cpu&);
        void ext_u64(Cpu&);
        void ext_string(Cpu&);
        void ext_float(Cpu&);
    }
}

/*
 * Create operations
 */
void init(Cpu& cpu);

} // namespace Juse
