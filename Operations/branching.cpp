

#include "operations.h"

/* 00xx */
void Juse::Operations::Standard::branching(Cpu& cpu)
{
    cpu.operations[0x0001] = S<Operation>(new Operation(
        "Short Jump", "SHJUMP", "goto DP:DS:A",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U16 target = MASK_BOTTOM8 & operation.argument(instruction, 0);
            machine.cpu.jump(machine.cpu.pool(), machine.cpu.segment(), target);
        },
        { { SIZE16 } }));
    cpu.operations[0x0002] = S<Operation>(new Operation(
        "Jump", "JUMP", "goto DP:A:B",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U32 segment = MASK_BOTTOM32 & operation.argument(instruction, 0);
            U16 target = MASK_BOTTOM16 & operation.argument(instruction, 1);
            machine.cpu.jump(machine.cpu.pool(), segment, target);
        },
        { { SIZE32 }, { SIZE16 } }));
    cpu.operations[0x0003] = S<Operation>(new Operation(
        "Long Jump", "LJUMP", "goto A",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            machine.cpu.longjump(operation.argument(instruction, 0));
        },
        { { SIZE64 } }));
    cpu.operations[0x0004] = S<Operation>(new Operation(
        "Call", "CALL", "call A",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U16 target = MASK_BOTTOM8 & operation.argument(instruction, 0);
            machine.multiPush(word2set(machine.cpu.instruction()));
            machine.cpu.jump(machine.cpu.pool(), machine.cpu.segment(), target);
        },
        { { SIZE16 } }));
    cpu.operations[0x0005] = S<Operation>(new Operation(
        "Return", "RET", "return",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U16 target = U16(machine.pop() << 8) + machine.pop();
            machine.cpu.jump(machine.cpu.pool(), machine.cpu.segment(), target);
        },
        {}));
    cpu.operations[0x0006] = S<Operation>(new Operation(
        "Long Call", "LCALL", "call! A",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U64 target = operation.argument(instruction, 0);
            machine.multiPush(word2set(machine.cpu.addressPointer()));
            machine.cpu.longjump(target);
        },
        { { SIZE64 } }));
    cpu.operations[0x0007] = S<Operation>(new Operation(
        "Long Return", "LRET", "return!",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U64 target = set2word(machine.multiPop(SIZE64));
            machine.cpu.longjump(target);
        },
        {}));
    cpu.operations[0x0008] = S<Operation>(new Operation(
        "If", "IF", "if A",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U8 compareOperator = U8(operation.argument(instruction, 0));
            if (!machine.cpu.registers.compareFlags.at(CompareFlag(compareOperator))) {
                machine.cpu.flag_skip = true;
            } 
        },
        { { SIZE8 } }));
    cpu.operations[0x0009] = S<Operation>(new Operation(
        "Skip", "SKIP", "skip",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            machine.cpu.flag_skip = true;
        },
        {}));

    cpu.operations[0x000F] = S<Operation>(new Operation(
        "End Program", "END", "end",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            machine.cpu.flag_exit = true;
        },
        {}));
}