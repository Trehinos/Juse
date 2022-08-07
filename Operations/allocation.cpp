

#include "operations.h"

/* 04xx */
void Juse::Operations::Standard::allocation(Cpu& cpu)
{
    cpu.operations[0x0400] = S<Operation>(new Operation(
        "Allocate Pool", "ALLOCPOOL", "alloc A",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U16 pool_index = U16(arguments[0].value);
            machine.createPool(pool_index);
        },
        { { SIZE16 } }));
    cpu.operations[0x0401] = S<Operation>(new Operation(
        "Allocate Segment", "ALLOCSEG", "alloc A on B",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U32 segment_index = U32(arguments[0].value);
            U16 pool_index = U16(arguments[1].value);
            machine.createSegment(pool_index, segment_index);
        },
        { { SIZE32 }, { SIZE16 } }));
}