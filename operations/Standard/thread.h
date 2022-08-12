#pragma once

#include "../../Juse/operations.h"

/* 04xx */
namespace Juse::Operations::Standard::Thread {
    void init();
    void add(OperationMap&);
    void thread(Machine& machine, Cpu& cpu, Address address, U32 freq);
    static S<Operation> ChangeFrequency;
    static S<Operation> Thread;
    static S<Operation> ShortThread;
    static S<Operation> ThreadAt;
}