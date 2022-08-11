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

/*
 * Create operations
 */
void init(Cpu&, const std::string = "std/std_ext");
void loadOperationsSets(OperationMap&, const std::string = "std/std_ext");
} // namespace Juse
