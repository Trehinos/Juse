#pragma once

#include "../model/Machine.h"
#include "../model/Operation.h"

namespace Juse::Debug {
void dumpState(Cpu&);
void dumpOperations(Cpu&);
void dumpProgram(Machine&, U16 = 0, U16 = 256, U32 = 0, U16 = 0);
} // namespace Debug