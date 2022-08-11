#pragma once

#include "../../operations.h"

/* 03xx */
namespace Juse::Operations::Standard::Allocation {
void init();
void add(OperationMap&);
static S<Operation> AllocatePool;
static S<Operation> AllocateSegment;
}