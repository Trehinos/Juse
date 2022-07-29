#pragma once

#include "../Machine/Machine.h"
#include "../Machine/Operation.h"

namespace Juse {

std::ostream& operator<<(std::ostream&, const U8);

namespace Debug {
void dumpState(Cpu &);
void dumpOperations(Cpu &);
void dumpProgram(Machine &, U8 = 0, U8 = 255, U32 = 0, U16 = 0);
} // namespace Debug

/* 01xx */ void createControlOperations(Cpu &);
/* 02xx */ void createRegistersOperations(Cpu &);

/* 10xx */ void createIoOperations(Cpu &);

/* 20xx */ void createPointersOperations(Cpu &);

/* F0xx */ void createStackOperations(Cpu &);
/* F1xx */ void createMemoryOperations(Cpu &);

/*
 * Create operations
 */
void init(Cpu &cpu);

} // namespace Juse
