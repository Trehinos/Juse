#pragma once

#include "../../Juse/operations.h"

/* 03xx */
namespace Juse::Operations::Standard::Allocation
{
    void init();
    void add(OperationMap&);
    static SPtr<Operation> AllocatePool;
    static SPtr<Operation> AllocateSegment;
}