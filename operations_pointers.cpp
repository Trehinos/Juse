
#include "operations.h"

namespace Juse {

/* 10xx */
void createPointersOperations(Cpu &cpu) {
  cpu.operations[0xF000] = S<Operation>(new Operation(
      "Not implemented", "", "",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        
      },
      {}));
}
} // namespace Juse