#include "operations.h"

namespace Juse {


/* 10xx */
void createIoOperations(Cpu &cpu) {
  cpu.operations[0x1001] = S<Operation>(new Operation(
      "Write Word", "WINT16", "out Words[A]",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U8 register_index = U8(operation.argument(instruction, 0));
        std::stringstream buffer{};
        buffer << std::dec << machine.cpu.registers.words[register_index];
        out(machine.out, buffer, machine.cpu.flag_debug);
      },
      {{SIZE8}}));
  cpu.operations[0x1002] = S<Operation>(new Operation(
      "Write Quad", "WINT32", "out Quads[A]",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U8 register_index = U8(operation.argument(instruction, 0));
        std::stringstream buffer{};
        buffer << std::dec << machine.cpu.registers.quads[register_index];
        out(machine.out, buffer, machine.cpu.flag_debug);
      },
      {{SIZE8}}));
  cpu.operations[0x1003] = S<Operation>(new Operation(
      "Write Long", "WINT64", "out Longs[A]",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U8 register_index = U8(operation.argument(instruction, 0));
        std::stringstream buffer{};
        buffer << std::dec << machine.cpu.registers.longs[register_index];
        out(machine.out, buffer, machine.cpu.flag_debug);
      },
      {{SIZE8}}));
  cpu.operations[0x1011] = S<Operation>(new Operation(
      "Read Word", "RINT16", "in Words[A]",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U16 value;
        std::stringstream buffer;
        U8 register_index = U8(operation.argument(instruction, 0));
        buffer << in(machine.out, machine.in, machine.cpu.flag_debug);
        buffer >> std::dec >> value;
        machine.cpu.registers.words[register_index] = value;
      },
      {{SIZE8}}));
  cpu.operations[0x1012] = S<Operation>(new Operation(
      "Read Quad", "RINT32", "in Quads[A]",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U32 value;
        std::stringstream buffer;
        U8 register_index = U8(operation.argument(instruction, 0));
        buffer << in(machine.out, machine.in, machine.cpu.flag_debug);
        buffer >> std::dec >> value;
        machine.cpu.registers.quads[register_index] = value;
      },
      {{SIZE8}}));
  cpu.operations[0x1013] = S<Operation>(new Operation(
      "Read Long", "RINT64", "in Longs[A]",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U64 value;
        std::stringstream buffer;
        U8 register_index = U8(operation.argument(instruction, 0));
        buffer << in(machine.out, machine.in, machine.cpu.flag_debug);
        buffer >> std::dec >> value;
        machine.cpu.registers.longs[register_index] = value;
      },
      {{SIZE8}}));

  cpu.operations[0x1081] = S<Operation>(new Operation(
      "Write Utf-16", "WUTF16", "out S16 [A]",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        // TODO convert encoding
        U16 address = U16(operation.argument(instruction, 0));
        U16 offset = 0;
        SS8 buffer{};
        U16 character{};
        Converter<CH16, CH8> CH16_CH8;
        do {
          character =
              CH16(U16(set2word(machine.readData(address + offset, SIZE16))));
          if (character != u'\0') {
            buffer << CH16_CH8.convert(character);
          }
          offset++;
        } while (character != u'\0');
        out(machine.out, buffer, machine.cpu.flag_debug);
      },
      {{SIZE16}}));

  cpu.operations[0x1082] = S<Operation>(new Operation(
      "Write Utf-32", "WUTF32", "out S32 [A]",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        // TODO convert encoding
        U16 address = U16(operation.argument(instruction, 0));
        U16 offset = 0;
        SS8 buffer{};
        U32 character{};
        Converter<CH32, CH8> CH32_CH8;
        do {
          character =
              CH32(U32(set2word(machine.readData(address + offset, SIZE32))));
          if (character != U'\0') {
            buffer << CH32_CH8.convert(character);
          }
          offset++;
        } while (character != U'\0');
        out(machine.out, buffer, machine.cpu.flag_debug);
      },
      {{SIZE16}}));
}
} // namespace Juse