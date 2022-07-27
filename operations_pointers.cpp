
#include "operations.h"

namespace Juse {

/* 10xx */
void createPointersOperations(Cpu &cpu) {
  cpu.operations[0x1000] = S<Operation>(new Operation(
      "Write Byte", "WINT8", "out B[$1]",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U8 register_index = U8(operation.argument(instruction, 0));
        machine.out << std::dec << machine.bytes[register_index] << std::flush;
      },
      {{SIZE8}}));
}
} // namespace Juse