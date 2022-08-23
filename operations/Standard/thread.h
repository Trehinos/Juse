#pragma once

#include "../../Juse/operations.h"

/* 04xx */
namespace Juse::Operations::Standard::Thread
{
    void init();
    void add(OperationMap&);
    static SPtr<Operation> ChangeFrequency;
    static SPtr<Operation> Thread;
    static SPtr<Operation> ShortThread;
    static SPtr<Operation> ThreadAt;
}