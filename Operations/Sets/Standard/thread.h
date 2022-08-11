#pragma once

#include "../../operations.h"

/* 04xx */
namespace Juse::Operations::Standard::Thread {
void init();
void add(OperationMap&);
void thread(Machine& machine, Cpu& cpu, Address address, U32 freq);
S<Operation> ChangeFrequency;
S<Operation> Thread;
S<Operation> ShortThread;
S<Operation> ThreadAt;
}