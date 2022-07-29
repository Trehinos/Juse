

#include "operations.h"

namespace Juse {

/* 01xx */
void createControlOperations(Cpu &cpu) {
  cpu.operations[0x0100] = S<Operation>(new Operation(
      "Short Jump", "SHJUMP", "goto DP:DS:A",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U16 target = MASK_BOTTOM8 & operation.argument(instruction, 0);
        machine.cpu.jump(machine.cpu.pool(), machine.cpu.segment(), target);
      },
      {{SIZE16}}));
  cpu.operations[0x0101] = S<Operation>(new Operation(
      "Jump", "JUMP", "goto DP:A:B",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U32 segment = MASK_BOTTOM32 & operation.argument(instruction, 0);
        U16 target = MASK_BOTTOM16 & operation.argument(instruction, 1);
        machine.cpu.jump(machine.cpu.pool(), segment, target);
      },
      {{SIZE32}, {SIZE16}}));
  cpu.operations[0x0102] = S<Operation>(new Operation(
      "Long Jump", "LJUMP", "goto A",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        machine.cpu.longjump(operation.argument(instruction, 0));
      },
      {{SIZE64}}));
  cpu.operations[0x0103] = S<Operation>(new Operation(
      "Call", "CALL", "call A",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U16 target = MASK_BOTTOM8 & operation.argument(instruction, 0);
        machine.push(0xFF00 & machine.cpu.instruction());
        machine.push(0x00FF & machine.cpu.instruction());
        machine.cpu.jump(machine.cpu.pool(), machine.cpu.segment(), target);
      },
      {{SIZE16}}));
  cpu.operations[0x0104] = S<Operation>(new Operation(
      "Return", "RET", "return",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U16 target = U16(machine.pop() << 8) + machine.pop();
        machine.cpu.jump(machine.cpu.pool(), machine.cpu.segment(), target);
      },
      {}));
  cpu.operations[0x0105] = S<Operation>(new Operation(
      "Long Call", "LCALL", "call! A",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U64 target = operation.argument(instruction, 0);
        machine.multiPush(word2set(machine.cpu.addressPointer()));
        machine.cpu.longjump(target);
      },
      {{SIZE64}}));
  cpu.operations[0x0106] = S<Operation>(new Operation(
      "Long Return", "LRET", "return!",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U64 target = set2word(machine.multiPop(SIZE64));
        machine.cpu.longjump(target);
      },
      {}));
  cpu.operations[0x0110] = S<Operation>(new Operation(
      "Set Data Pool", "DATAPOOL", "datapool A",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        machine.dataPool = U16(operation.argument(instruction, 0));
      },
      {{SIZE16}}));
  cpu.operations[0x0111] = S<Operation>(new Operation(
      "Set Data Segment", "DATASEG", "dataset A",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        machine.dataSegment = U32(operation.argument(instruction, 0));
      },
      {{SIZE32}}));

  cpu.operations[0x01FF] = S<Operation>(
      new Operation("End Program", "END", "end",
                    [](Machine &machine, Instruction &instruction,
                       Operation &operation) { machine.cpu.flag_exit = true; },
                    {}));
}
} // namespace Juse