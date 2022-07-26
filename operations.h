#pragma once

#include "Machine.h"
#include "Operation.h"

namespace Juse {

void dumpState(Cpu &);
void dumpOperations(Cpu &);
void dumpProgram(Machine &, size_t);

void createControlOperations(Cpu &);
void createRegistersOperations(Cpu &);
void createIoOperations(Cpu &);
void createMemoryOperations(Cpu &);

/*
 * Create operations
 */
void init(Cpu &cpu);

} // namespace Juse
