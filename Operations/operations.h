#pragma once

#include "../Machine/Machine.h"
#include "../Machine/Operation.h"

namespace Juse {

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

/*
 * Create operations
 */
void init(Cpu&, const std::string = "std/std_ext/str/float");
void loadOperationSet(Juse::Cpu&, const std::string);
} // namespace Juse
