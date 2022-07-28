
#include "operations.h"

namespace Juse {

/* 20xx */
void createPointersOperations(Cpu &cpu) {
  
  cpu.operations[0x2000] = S<Operation>(new Operation(
      "Not implemented", "", "",
      [](Machine &machine, Instruction &instruction, Operation &operation) {

      },
      {{SIZE16}}));
      
}
} // namespace Juse