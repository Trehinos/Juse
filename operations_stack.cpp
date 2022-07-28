
#include "operations.h"

namespace Juse {

/* F1xx */
void createStackOperations(Cpu &cpu) {
  cpu.operations[0xF1F0] = S<Operation>(new Operation(
      "Push Data Segment", "PUSHDS", "push DS",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        machine.multiPush(word2set(machine.dataSegment));
      },
      {}));

  cpu.operations[0xF1F1] = S<Operation>(new Operation(
      "Pop Data Segment", "POPDS", "pop DS",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        machine.dataSegment = U32(set2word(machine.multiPop(SIZE32)));
      },
      {}));
}
} // namespace Juse