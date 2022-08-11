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
    }
    namespace StandardExtensions {
        void addExtU8(OperationMap&);
        void addExtU16(OperationMap&);
        void addExtU32(OperationMap&);
        void addExtU64(OperationMap&);
        void addExtFloat(OperationMap&);
        void addExtTypes(OperationMap&);
    }
}

/*
 * Create operations
 */
void init(Cpu&, const std::string = "std/std_ext");
void loadOperationsSets(OperationMap&, const std::string = "std/std_ext");
} // namespace Juse
