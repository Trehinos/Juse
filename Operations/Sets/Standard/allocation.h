#pragma once

#include "../../operations.h"

/* 03xx */
namespace Juse::Operations::Standard::Allocation {
void init();
void add(OperationMap&);
S<Operation> AllocatePool;
S<Operation> AllocateSegment;
}