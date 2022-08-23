#pragma once

#include "../../Juse/operations.h"

/* 00xx */
namespace Juse::Operations::Standard::Branching
{
    void init();
    void add(OperationMap&);
    static SPtr<Operation> ShortJump;
    static SPtr<Operation> Jump;
    static SPtr<Operation> LongJump;
    static SPtr<Operation> Call;
    static SPtr<Operation> Return;
    static SPtr<Operation> LongCall;
    static SPtr<Operation> LongReturn;
    static SPtr<Operation> If;
    static SPtr<Operation> Skip;
    static SPtr<Operation> Next;
    static SPtr<Operation> NextDirect;
    static SPtr<Operation> End;
}