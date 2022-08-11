#pragma once

#include "../../operations.h"

/* 01xx */
namespace Juse::Operations::Standard::Move {
void init();
void add(OperationMap&);
S<Operation> SetDataPool;
S<Operation> SetDataSegment;
S<Operation> SetAddressPointer;
S<Operation> SetAddressOffset;
S<Operation> SetAddressIncrement;
S<Operation> CopyToDataPool;
S<Operation> CopyToDataSegment;
S<Operation> CopyToAddressPointer;
S<Operation> CopyToAddressOffset;
S<Operation> CopyToAddressIncrement;
S<Operation> CopyFromDataPool;
S<Operation> CopyFromDataSegment;
S<Operation> CopyFromAddressPointer;
S<Operation> CopyFromAddressOffset;
S<Operation> CopyFromAddressIncrement;
S<Operation> PushDataPool;
S<Operation> PushDataSegment;
S<Operation> PushAddressPointer;
S<Operation> PushAddressOffset;
S<Operation> PushAddressIncrement;
S<Operation> PopDataPool;
S<Operation> PopDataSegment;
S<Operation> PopAddressPointer;
S<Operation> PopAddressOffset;
S<Operation> PopAddressIncrement;
}