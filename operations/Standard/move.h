#pragma once

#include "../../Juse/operations.h"

/* 01xx */
namespace Juse::Operations::Standard::Move {
    void init();
    void add(OperationMap&);
    static S<Operation> SetDataPool;
    static S<Operation> SetDataSegment;
    static S<Operation> SetAddressPointer;
    static S<Operation> SetAddressOffset;
    static S<Operation> SetAddressIncrement;
    static S<Operation> CopyToDataPool;
    static S<Operation> CopyToDataSegment;
    static S<Operation> CopyToAddressPointer;
    static S<Operation> CopyToAddressOffset;
    static S<Operation> CopyToAddressIncrement;
    static S<Operation> CopyFromDataPool;
    static S<Operation> CopyFromDataSegment;
    static S<Operation> CopyFromAddressPointer;
    static S<Operation> CopyFromAddressOffset;
    static S<Operation> CopyFromAddressIncrement;
    static S<Operation> PushDataPool;
    static S<Operation> PushDataSegment;
    static S<Operation> PushAddressPointer;
    static S<Operation> PushAddressOffset;
    static S<Operation> PushAddressIncrement;
    static S<Operation> PopDataPool;
    static S<Operation> PopDataSegment;
    static S<Operation> PopAddressPointer;
    static S<Operation> PopAddressOffset;
    static S<Operation> PopAddressIncrement;
}