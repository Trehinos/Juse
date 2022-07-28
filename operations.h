#pragma once

#include "Machine.h"
#include "Operation.h"

namespace Juse {

void dumpState(Cpu &);
void dumpOperations(Cpu &);
void dumpProgram(Machine &, size_t);

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
