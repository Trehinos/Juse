

#include "operations.h"

namespace Juse {

/* 01xx */
void createControlOperations(Cpu &cpu) {
  cpu.operations[0x0100] = S<Operation>(new Operation(
      "Short Jump", "SHJUMP", "goto $1",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U16 target = MASK_BOTTOM8 & operation.argument(instruction, 0);
        machine.cpu.jump(machine.cpu.pool(), machine.cpu.segment(), target);
      },
      {{SIZE16}}));
  cpu.operations[0x0101] = S<Operation>(new Operation(
      "Jump", "JUMP", "goto $1:$2",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U32 segment = MASK_BOTTOM32 & operation.argument(instruction, 0);
        U16 target = MASK_BOTTOM16 & operation.argument(instruction, 1);
        machine.cpu.jump(machine.cpu.pool(), segment, target);
      },
      {{SIZE32}, {SIZE16}}));
  cpu.operations[0x0102] = S<Operation>(new Operation(
      "Long Jump", "LJUMP", "goto! $1",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        machine.cpu.longjump(operation.argument(instruction, 0));
      },
      {{SIZE64}}));
  cpu.operations[0x0103] = S<Operation>(new Operation(
      "Call", "CALL", "call $1",
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
      "Long Call", "LCALL", "call! $1",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U64 target = operation.argument(instruction, 0);
        machine.push(0xFF00 & machine.cpu.instruction());
        machine.push(0x00FF & machine.cpu.instruction());
        // ...
        // machine.cpu.longjump(target);
      },
      {{SIZE64}}));
  cpu.operations[0x0106] = S<Operation>(new Operation(
      "Long Return", "LRET", "return!",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        U64 target = (U64(machine.pop()) << 56) + (U64(machine.pop()) << 48) +
                     (U64(machine.pop()) << 40) + (U64(machine.pop()) << 32) +
                     (U64(machine.pop()) << 24) + (U64(machine.pop()) << 16) +
                     (U64(machine.pop()) << 8) + U64(machine.pop());
        machine.cpu.longjump(target);
      },
      {}));
  cpu.operations[0x0110] = S<Operation>(new Operation(
      "Set Data Pool", "DATAPOOL", "datapool $1",
      [](Machine &machine, Instruction &instruction, Operation &operation) {
        machine.dataPool = U16(operation.argument(instruction, 0));
      },
      {{SIZE16}}));
  cpu.operations[0x0111] = S<Operation>(new Operation(
      "Set Data Segment", "DATASEG", "dataset $1",
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