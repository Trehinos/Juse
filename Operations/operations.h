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
        void addBranching(Cpu&);
        void addMoves(Cpu&);
        void addLogic(Cpu&);
        void addAllocations(Cpu&);
        void addThreads(Cpu&);
    }
    namespace StandardExtensions {
        void addExtU8(Cpu&);
        void addExtU16(Cpu&);
        void addExtU32(Cpu&);
        void addExtU64(Cpu&);
        void addExtFloat(Cpu&);
        void addExtTypes(Cpu&);
    }
}

/*
 * Create operations
 */
void init(Cpu&, const std::string = "std/std_ext");
void loadOperationsSets(Juse::Cpu&, const std::string);
} // namespace Juse
