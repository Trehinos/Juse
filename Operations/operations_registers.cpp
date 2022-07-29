
#include "operations.h"

namespace Juse {

/* 02xx */
void createRegistersOperations(Cpu &cpu) {
  cpu.operations[0x0200] = S<Operation>(new Operation(
      "Set Byte", "SET8", "Bytes[A] = B",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U8 register_index = U8(operation.argument(instruction, 0));
        U8 value = U8(operation.argument(instruction, 1));
        machine.bytes[register_index] = value;
      },
      {{SIZE8}, {SIZE8}}));
  cpu.operations[0x0201] = S<Operation>(new Operation(
      "Set Word", "SET16", "Words[A] = B",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U8 register_index = U8(operation.argument(instruction, 0));
        U16 value = U16(operation.argument(instruction, 1));
        machine.words[register_index] = value;
      },
      {{SIZE8}, {SIZE16}}));
  cpu.operations[0x0202] = S<Operation>(new Operation(
      "Set Quad", "SET32", "Quads[A] = B",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U8 register_index = U8(operation.argument(instruction, 0));
        U32 value = U32(operation.argument(instruction, 1));
        machine.quads[register_index] = value;
      },
      {{SIZE8}, {SIZE32}}));
  cpu.operations[0x0203] = S<Operation>(new Operation(
      "Set Long", "SET64", "Longs[A] = B",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U8 register_index = U8(operation.argument(instruction, 0));
        U64 value = operation.argument(instruction, 1);
        machine.longs[register_index] = value;
      },
      {{SIZE8}, {SIZE64}}));

  cpu.operations[0x0210] = S<Operation>(new Operation(
      "Copy Byte From", "COPYFROM8", "Bytes[A] = [B]",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U8 register_index = U8(operation.argument(instruction, 0));
        U16 address = U16(operation.argument(instruction, 1));
        U8 value = U8(set2word(machine.readData(address, SIZE8)));
        machine.bytes[register_index] = value;
      },
      {{SIZE8}, {SIZE16}}));

  cpu.operations[0x0211] = S<Operation>(new Operation(
      "Copy Word From", "COPYFROM16", "Words[A] = [B]",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U8 register_index = U8(operation.argument(instruction, 0));
        U16 address = U16(operation.argument(instruction, 1));
        U16 value = U16(set2word(machine.readData(address, SIZE16)));
        machine.words[register_index] = value;
      },
      {{SIZE8}, {SIZE16}}));

  cpu.operations[0x0212] = S<Operation>(new Operation(
      "Copy Quad From", "COPYFROM32", "Quads[A] = [B]",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U8 register_index = U8(operation.argument(instruction, 0));
        U16 address = U16(operation.argument(instruction, 1));
        U32 value = U32(set2word(machine.readData(address, SIZE32)));
        machine.quads[register_index] = value;
      },
      {{SIZE8}, {SIZE16}}));

  cpu.operations[0x0213] = S<Operation>(new Operation(
      "Copy Long From", "COPYFROM64", "Longs[A] = [B]",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U8 register_index = U8(operation.argument(instruction, 0));
        U16 address = U16(operation.argument(instruction, 1));
        U64 value = set2word(machine.readData(address, SIZE64));
        machine.longs[register_index] = value;
      },
      {{SIZE8}, {SIZE16}}));

  cpu.operations[0x0220] = S<Operation>(new Operation(
      "Copy Byte To", "COPYTO8", "[A] = Bytes[B]",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U16 address = U16(operation.argument(instruction, 0));
        U8 register_index = U8(operation.argument(instruction, 1));
        machine.writeData(address, word2set<U8>(machine.bytes[register_index]));
      },
      {{SIZE16}, {SIZE8}}));

  cpu.operations[0x0221] = S<Operation>(new Operation(
      "Copy Word To", "COPYTO16", "[A] = Words[B]",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U16 address = U16(operation.argument(instruction, 0));
        U8 register_index = U8(operation.argument(instruction, 1));
        machine.writeData(address, word2set<U16>(machine.bytes[register_index]));
      },
      {{SIZE16}, {SIZE8}}));

  cpu.operations[0x0222] = S<Operation>(new Operation(
      "Copy Quad To", "COPYTO32", "[A] = Quads[B]",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U16 address = U16(operation.argument(instruction, 0));
        U8 register_index = U8(operation.argument(instruction, 1));
        machine.writeData(address, word2set<U32>(machine.bytes[register_index]));
      },
      {{SIZE16}, {SIZE8}}));

  cpu.operations[0x0223] = S<Operation>(new Operation(
      "Copy Long To", "COPYTO64", "[A] = Longs[B]",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U16 address = U16(operation.argument(instruction, 0));
        U8 register_index = U8(operation.argument(instruction, 1));
        machine.writeData(address, word2set<U64>(machine.bytes[register_index]));
      },
      {{SIZE16}, {SIZE8}}));
}
} // namespace Juse