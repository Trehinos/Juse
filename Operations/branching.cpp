
#include "operations.h"

/* 00xx */
void Juse::Operations::Standard::branching(Cpu& cpu)
{
    cpu.operations[0x0001] = S<Operation>(new Operation(
        "Short Jump", "SHJUMP", "goto DP:DS:A",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U16 target = MASK_BOTTOM8 & arguments[0].value;
            cpu.jump(cpu.pool(), cpu.segment(), target);
        },
        { { SIZE16 } }));
    cpu.operations[0x0002] = S<Operation>(new Operation(
        "Jump", "JUMP", "goto DP:A:B",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U32 segment = MASK_BOTTOM32 & arguments[0].value;
            U16 target = MASK_BOTTOM16 & arguments[1].value;
            cpu.jump(cpu.pool(), segment, target);
        },
        { { SIZE32 }, { SIZE16 } }));
    cpu.operations[0x0003] = S<Operation>(new Operation(
        "Long Jump", "LJUMP", "goto A",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.longjump(arguments[0].value);
        },
        { { SIZE64 } }));
    cpu.operations[0x0004] = S<Operation>(new Operation(
        "Call", "CALL", "call A",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U16 target = MASK_BOTTOM8 & arguments[0].value;
            cpu.multiPush(word2set(cpu.instruction()));
            cpu.jump(cpu.pool(), cpu.segment(), target);
        },
        { { SIZE16 } }));
    cpu.operations[0x0005] = S<Operation>(new Operation(
        "Return", "RET", "return",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U16 target = U16(cpu.pop() << 8) + cpu.pop();
            cpu.jump(cpu.pool(), cpu.segment(), target);
        },
        {}));
    cpu.operations[0x0006] = S<Operation>(new Operation(
        "Long Call", "LCALL", "call! A",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U64 target = arguments[0].value;
            cpu.multiPush(word2set(cpu.instructionPointer()));
            cpu.longjump(target);
        },
        { { SIZE64 } }));
    cpu.operations[0x0007] = S<Operation>(new Operation(
        "Long Return", "LRET", "return!",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U64 target = set2word(cpu.multiPop(SIZE64));
            cpu.longjump(target);
        },
        {}));
    cpu.operations[0x0008] = S<Operation>(new Operation(
        "If", "IF", "if A",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 compareOperator = U8(arguments[0].value);
            if (!cpu.registers.compareFlags.at(CompareFlag(compareOperator))) {
                cpu.flag_skip = true;
            } 
        },
        { { SIZE8 } }));

    cpu.operations[0x0009] = S<Operation>(new Operation(
        "Skip", "SKIP", "skip",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.flag_skip = true;
        },
        {}));

    cpu.operations[0x000A] = S<Operation>(new Operation(
        "Next", "NEXT", "next Words[A]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            U16 target = cpu.registers.words[register_index];
            cpu.address_offset += cpu.address_increment;
            cpu.jump(cpu.pool(), cpu.segment(), target);
        },
        {{SIZE8}}));

    cpu.operations[0x000B] = S<Operation>(new Operation(
        "Next Direct", "NEXTD", "next A",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U16 target = U16(arguments[0].value);
            cpu.address_offset += cpu.address_increment;
            cpu.jump(cpu.pool(), cpu.segment(), target);
        },
        {{SIZE16}}));

    cpu.operations[0x000F] = S<Operation>(new Operation(
        "End Program", "END", "end",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.flag_exit = true;
        },
        {}));
}