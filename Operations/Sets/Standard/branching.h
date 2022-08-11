#pragma once

#include "../../operations.h"

/* 00xx */
namespace Juse::Operations::Standard::Branching {
void init();
void add(OperationMap&);
 static S<Operation> ShortJump;
 static S<Operation> Jump;
 static S<Operation> LongJump;
 static S<Operation> Call;
 static S<Operation> Return;
 static S<Operation> LongCall;
 static S<Operation> LongReturn;
 static S<Operation> If;
 static S<Operation> Skip;
 static S<Operation> Next;
 static S<Operation> NextDirect;
 static S<Operation> End;
}