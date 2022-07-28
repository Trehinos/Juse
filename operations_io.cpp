#include <codecvt>

#include "operations.h"

namespace Juse {

template <IsChar From, IsChar To> struct Converter {

  static To convert(From from) { return To(from); }
};

void out(std::ostream &os, SS8 &ss, bool debug) {
  if (debug) {
    os << "  << ";
  }
  os << ss.str() << std::flush;
  if (debug) {
    os << std::endl;
  }
}

std::string in(std::ostream &os, std::istream &is, bool debug) {
  std::string str{};
  if (debug) {
    os << "  >> ";
  }
  std::getline(is >> std::ws, str);
  is.clear();
  return str;
}

/* 10xx */
void createIoOperations(Cpu &cpu) {
  cpu.operations[0x1000] = S<Operation>(new Operation(
      "Write Byte", "WINT8", "out Bytes[A]",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U8 register_index = U8(operation.argument(instruction, 0));
        std::stringstream buffer{};
        buffer << std::dec << +machine.bytes[register_index];
        out(machine.out, buffer, machine.cpu.flag_debug);
      },
      {{SIZE8}}));
  cpu.operations[0x1001] = S<Operation>(new Operation(
      "Write Word", "WINT16", "out Words[A]",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U8 register_index = U8(operation.argument(instruction, 0));
        std::stringstream buffer{};
        buffer << std::dec << machine.words[register_index];
        out(machine.out, buffer, machine.cpu.flag_debug);
      },
      {{SIZE8}}));
  cpu.operations[0x1002] = S<Operation>(new Operation(
      "Write Quad", "WINT32", "out Quads[A]",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U8 register_index = U8(operation.argument(instruction, 0));
        std::stringstream buffer{};
        buffer << std::dec << machine.quads[register_index];
        out(machine.out, buffer, machine.cpu.flag_debug);
      },
      {{SIZE8}}));
  cpu.operations[0x1003] = S<Operation>(new Operation(
      "Write Long", "WINT64", "out Longs[A]",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U8 register_index = U8(operation.argument(instruction, 0));
        std::stringstream buffer{};
        buffer << std::dec << machine.longs[register_index];
        out(machine.out, buffer, machine.cpu.flag_debug);
      },
      {{SIZE8}}));
  cpu.operations[0x1004] = S<Operation>(new Operation(
      "Write Direct", "WINT", "out [DP:DS:A]",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U8 register_index = U8(operation.argument(instruction, 0));
        std::stringstream buffer{};
        buffer << std::dec
               << U16(set2word(machine.readData(register_index, SIZE16)));
        out(machine.out, buffer, machine.cpu.flag_debug);
      },
      {{SIZE16}}));
  cpu.operations[0x1010] = S<Operation>(new Operation(
      "Read Byte", "RINT8", "in Bytes[A]",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U16 value;
        std::stringstream buffer;
        U8 register_index = U8(operation.argument(instruction, 0));
        buffer << in(machine.out, machine.in, machine.cpu.flag_debug);
        buffer >> std::dec >> value;
        machine.bytes[register_index] = U8(value);
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
        machine.words[register_index] = value;
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
        machine.quads[register_index] = value;
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
        machine.longs[register_index] = value;
      },
      {{SIZE8}}));

  cpu.operations[0x10F0] = S<Operation>(new Operation(
      "Write Ascii", "WASCII", "out S8 [A]",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U16 address = U16(operation.argument(instruction, 0));
        U16 offset = 0;
        std::stringstream buffer{};
        U8 character{};
        do {
          character = CH8(U8(set2word(machine.readData(address + offset, SIZE8))));
          if (character != '\0') {
            buffer << character;
          }
          offset++;
        } while (character != '\0');
        out(machine.out, buffer, machine.cpu.flag_debug);
      },
      {{SIZE16}}));

  cpu.operations[0x10F1] = S<Operation>(new Operation(
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

  cpu.operations[0x10F2] = S<Operation>(new Operation(
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