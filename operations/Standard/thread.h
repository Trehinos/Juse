#pragma once

#include "../../Juse/operations.h"

/* 04xx */
namespace Juse::Operations::Standard::Thread {
    void init();
    void add(OperationMap&);
    void thread(Machine& machine, Cpu& cpu, Address address, U32 freq);
    static SPtr<Operation> ChangeFrequency;
    static SPtr<Operation> Thread;
    static SPtr<Operation> ShortThread;
    static SPtr<Operation> ThreadAt;
}