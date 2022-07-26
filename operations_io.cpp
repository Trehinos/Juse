
#include "operations.h"

namespace Juse {

/* 10xx */
void createIoOperations(Cpu &cpu) {
  cpu.operations[0x10F0] = S<Operation>(new Operation(
      "Write Ascii", "WASCII", "out S8 *$1",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U16 address = U16(operation.argument(instruction, 0));
        U16 offset = 0;
        std::string buffer{};
        U8 character{};
        do {
          character = U8(set2word(machine.readData(address + offset, 1)));
          if (character != '\0') {
            buffer += character;
          }
          offset++;
        } while (character != '\0');
        if (machine.cpu.flag_debug) {
          machine.out << "  > ";
        }
        machine.out << buffer;
        if (machine.cpu.flag_debug) {
          machine.out << std::endl;
        }
      },
      {{SIZE16}}));
}
} // namespace Juse