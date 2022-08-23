#pragma once

#include "Machine/Machine.h"
#include "Machine/Operation.h"

namespace Juse
{
    template <IsChar From, IsChar To>
    struct Converter
    {
        static To convert(From from) { return To(from); }
    };

    void out(std::ostream&, SS8&, bool);
    S8 in(std::ostream&, std::istream&, bool);

    namespace Operations
    {
        /*
         * Create operations
         */
        void initCpu(Cpu&, const S8 = "std/std_ext", U32 = Cpu::BASE_FREQUENCY);
        void initSets();
        void loadSets(OperationMap&, const S8 = "std/std_ext");
    }
} // namespace Juse
