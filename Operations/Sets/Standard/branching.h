#pragma once

#include "../../operations.h"

/* 00xx */
namespace Juse::Operations::Standard::Branching {
void init();
void add(OperationMap&);
 S<Operation> ShortJump;
 S<Operation> Jump;
 S<Operation> LongJump;
 S<Operation> Call;
 S<Operation> Return;
 S<Operation> LongCall;
 S<Operation> LongReturn;
 S<Operation> If;
 S<Operation> Skip;
 S<Operation> Next;
 S<Operation> NextDirect;
 S<Operation> End;
}