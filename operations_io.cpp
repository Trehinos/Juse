
#include "operations.h"

namespace Juse {

/* 10xx */
void createIoOperations(Cpu &cpu) {
  cpu.operations[0x1000] = S<Operation>(new Operation(
      "Write Byte", "WINT8", "out B[$1]",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U8 register_index = U8(operation.argument(instruction, 0));
        machine.out << std::dec << machine.bytes[register_index] << std::flush;
      },
      {{SIZE8}}));
  cpu.operations[0x1001] = S<Operation>(new Operation(
      "Write Word", "WINT16", "out W[$1]",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U8 register_index = U8(operation.argument(instruction, 0));
        machine.out << std::dec << machine.words[register_index] << std::flush;
      },
      {{SIZE8}}));
  cpu.operations[0x1002] = S<Operation>(new Operation(
      "Write Quad", "WINT32", "out Q[$1]",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U8 register_index = U8(operation.argument(instruction, 0));
        machine.out << std::dec << machine.quads[register_index] << std::flush;
      },
      {{SIZE8}}));
  cpu.operations[0x1003] = S<Operation>(new Operation(
      "Write Long", "WINT64", "out L[$1]",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U8 register_index = U8(operation.argument(instruction, 0));
        machine.out << std::dec << machine.longs[register_index] << std::flush;
      },
      {{SIZE8}}));

  cpu.operations[0x10F0] = S<Operation>(new Operation(
      "Write Ascii", "WASCII", "out S8 [$1]",
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
        machine.out << buffer << std::flush;
        if (machine.cpu.flag_debug) {
          machine.out << std::endl;
        }
      },
      {{SIZE16}}));
}
} // namespace Juse