
#include "branching.h"

namespace Juse::Operations::Standard::Branching {

void add(OperationMap& operations)
{
    operations[0x0001] = ShortJump;
    operations[0x0002] = Jump;
    operations[0x0003] = LongJump;
    operations[0x0004] = Call;
    operations[0x0005] = Return;
    operations[0x0006] = LongCall;
    operations[0x0007] = LongReturn;
    operations[0x0008] = If;
    operations[0x0009] = Skip;
    operations[0x000A] = Next;
    operations[0x000B] = NextDirect;

    operations[0x000F] = End;
}

S<Operation> ShortJump = S<Operation>(new Operation(
    "Short Jump", "SHJUMP", "goto DP:DS:A",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        U16 target = MASK_BOTTOM8 & arguments[0].value;
        cpu.jump(cpu.pool(), cpu.segment(), target);
    },
    { { SIZE16 } }));
S<Operation> Jump = S<Operation>(new Operation(
    "Jump", "JUMP", "goto DP:A:B",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        U32 segment = MASK_BOTTOM32 & arguments[0].value;
        U16 target = MASK_BOTTOM16 & arguments[1].value;
        cpu.jump(cpu.pool(), segment, target);
    },
    { { SIZE32 }, { SIZE16 } }));
S<Operation> LongJump = S<Operation>(new Operation(
    "Long Jump", "LJUMP", "goto A",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        cpu.longjump(arguments[0].value);
    },
    { { SIZE64 } }));
S<Operation> Call = S<Operation>(new Operation(
    "Call", "CALL", "call A",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        U16 target = MASK_BOTTOM8 & arguments[0].value;
        cpu.multiPush(word2set(cpu.instruction()));
        cpu.jump(cpu.pool(), cpu.segment(), target);
    },
    { { SIZE16 } }));
S<Operation> Return = S<Operation>(new Operation(
    "Return", "RET", "return",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        U16 target = U16(cpu.pop() << 8) + cpu.pop();
        cpu.jump(cpu.pool(), cpu.segment(), target);
    },
    {}));
S<Operation> LongCall = S<Operation>(new Operation(
    "Long Call", "LCALL", "call! A",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        U64 target = arguments[0].value;
        cpu.multiPush(word2set(cpu.instructionPointer()));
        cpu.longjump(target);
    },
    { { SIZE64 } }));
S<Operation> LongReturn = S<Operation>(new Operation(
    "Long Return", "LRET", "return!",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        U64 target = set2word(cpu.multiPop(SIZE64));
        cpu.longjump(target);
    },
    {}));
S<Operation> If = S<Operation>(new Operation(
    "If", "IF", "if A",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        U8 compareOperator = U8(arguments[0].value);
        if (!cpu.registers.compareFlags.at(CompareFlag(compareOperator))) {
            cpu.flag_skip = true;
        }
    },
    { { SIZE8 } }));

S<Operation> Skip = S<Operation>(new Operation(
    "Skip", "SKIP", "skip",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        cpu.flag_skip = true;
    },
    {}));

S<Operation> Next = S<Operation>(new Operation(
    "Next", "NEXT", "next Words[A]",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        U8 register_index = U8(arguments[0].value);
        U16 target = cpu.registers.words[register_index];
        cpu.address_offset += cpu.address_increment;
        cpu.jump(cpu.pool(), cpu.segment(), target);
    },
    { { SIZE8 } }));

S<Operation> NextDirect = S<Operation>(new Operation(
    "Next Direct", "NEXTD", "next A",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        U16 target = U16(arguments[0].value);
        cpu.address_offset += cpu.address_increment;
        cpu.jump(cpu.pool(), cpu.segment(), target);
    },
    { { SIZE16 } }));

S<Operation> End = S<Operation>(new Operation(
    "End Program", "END", "end",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        cpu.flag_exit = true;
    },
    {}));
}