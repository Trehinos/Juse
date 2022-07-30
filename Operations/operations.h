#pragma once

#include "../Machine/Machine.h"
#include "../Machine/Operation.h"

namespace Juse {

std::ostream& operator<<(std::ostream&, const U8);

namespace Debug {
    void dumpState(Cpu&);
    void dumpOperations(Cpu&);
    void dumpProgram(Machine&, U16 = 0, U16 = 256, U32 = 0, U16 = 0);
} // namespace Debug

template <IsChar From, IsChar To>
struct Converter {
    static To convert(From from) { return To(from); }
};

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

template <Juse::IsWord T>
void setWord(Juse::Operation& operation, Juse::Instruction& instruction, Juse::GeneralRegisters<T>& registers)
{
    Juse::U8 register_index = Juse::U8(operation.argument(instruction, 0));
    T value = T(operation.argument(instruction, 1));
    registers[register_index] = value;
}

/*
 * Create operations
 */
void init(Cpu& cpu);

} // namespace Juse
