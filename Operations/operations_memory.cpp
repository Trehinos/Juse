
#include "operations.h"

namespace Juse {

/* F0xx */
void createMemoryOperations(Cpu &cpu) {
  cpu.operations[0xF000] = S<Operation>(new Operation(
      "Allocate Pool", "ALLOCPOOL", "alloc A",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U16 pool_index = U16(operation.argument(instruction, 0));
        machine.createPool(pool_index);
      },
      {{SIZE16}}));
  cpu.operations[0xF001] = S<Operation>(new Operation(
      "Allocate Segment", "ALLOCSEG", "alloc A on B",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U32 segment_index = U32(operation.argument(instruction, 0));
        U16 pool_index = U16(operation.argument(instruction, 1));
        machine.createSegment(pool_index, segment_index);
      },
      {{SIZE32}, {SIZE16}}));
}
} // namespace Juse