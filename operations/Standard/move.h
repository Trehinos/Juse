#pragma once

#include "../../Juse/operations.h"

/* 01xx */
namespace Juse::Operations::Standard::Move {
    void init();
    void add(OperationMap&);
    static SPtr<Operation> SetDataPool;
    static SPtr<Operation> SetDataSegment;
    static SPtr<Operation> SetAddressPointer;
    static SPtr<Operation> SetAddressOffset;
    static SPtr<Operation> SetAddressIncrement;
    static SPtr<Operation> CopyToDataPool;
    static SPtr<Operation> CopyToDataSegment;
    static SPtr<Operation> CopyToAddressPointer;
    static SPtr<Operation> CopyToAddressOffset;
    static SPtr<Operation> CopyToAddressIncrement;
    static SPtr<Operation> CopyFromDataPool;
    static SPtr<Operation> CopyFromDataSegment;
    static SPtr<Operation> CopyFromAddressPointer;
    static SPtr<Operation> CopyFromAddressOffset;
    static SPtr<Operation> CopyFromAddressIncrement;
    static SPtr<Operation> PushDataPool;
    static SPtr<Operation> PushDataSegment;
    static SPtr<Operation> PushAddressPointer;
    static SPtr<Operation> PushAddressOffset;
    static SPtr<Operation> PushAddressIncrement;
    static SPtr<Operation> PopDataPool;
    static SPtr<Operation> PopDataSegment;
    static SPtr<Operation> PopAddressPointer;
    static SPtr<Operation> PopAddressOffset;
    static SPtr<Operation> PopAddressIncrement;
}