
#include "branching.h"

namespace Juse::Operations::Standard::Branching
{

    void add(OperationMap& operations) {
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

    void init() {
        ShortJump = SPtr<Operation>(new Operation(
            "Short Jump", "SHJUMP", "goto DP:DS:A",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            DataId target(arguments[0].value);
            cpu.jump(cpu.pool(), cpu.segment(), target);
        }, { { SIZE24 } }));

        Jump = SPtr<Operation>(new Operation(
            "Jump", "JUMP", "goto DP:A:B",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            SegmentId segment(arguments[0].value);
            DataId target(arguments[1].value);
            cpu.jump(cpu.pool(), segment, target);
        }, { { SIZE24 }, { SIZE24 } }));

        LongJump = SPtr<Operation>(new Operation(
            "Long Jump", "LJUMP", "goto A",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.longjump(arguments[0].value);
        }, { { SIZE64 } }));

        Call = SPtr<Operation>(new Operation(
            "Call", "CALL", "call A",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            DataId target(arguments[0].value);
            cpu.multiPush(word2set(cpu.instruction()));
            cpu.jump(cpu.pool(), cpu.segment(), target);
        }, { { SIZE24 } }));

        Return = SPtr<Operation>(new Operation(
            "Return", "RET", "return",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            DataId target { set2word(cpu.multiPop(sizeof DataId)) };
            cpu.jump(cpu.pool(), cpu.segment(), target);
        }, {}));

        LongCall = SPtr<Operation>(new Operation(
            "Long Call", "LCALL", "call! A",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U64 target = arguments[0].value;
            cpu.multiPush(word2set(cpu.instructionPointer()));
            cpu.longjump(target);
        }, { { SIZE64 } }));

        LongReturn = SPtr<Operation>(new Operation(
            "Long Return", "LRET", "return!",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U64 target { set2word(cpu.multiPop(SIZE64)) };
            cpu.longjump(target);
        }, {}));

        If = SPtr<Operation>(new Operation(
            "If", "IF", "if A",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 compareOperator { arguments[0].value };
            if (!cpu.registers.compareFlags.at(CompareFlag(compareOperator))) {
                cpu.flag_skip = true;
            }
        }, { { SIZE8 } }));

        Skip = SPtr<Operation>(new Operation(
            "Skip", "SKIP", "skip",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.flag_skip = true;
        }, {}));

        Next = SPtr<Operation>(new Operation(
            "Next", "NEXT", "next Words[A]",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {

            // TODO

        }, { { SIZE8 } }));

        NextDirect = SPtr<Operation>(new Operation(
            "Next Direct", "NEXTD", "next A",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            DataId target(arguments[0].value);
            cpu.address_offset += cpu.address_increment;
            cpu.jump(cpu.pool(), cpu.segment(), target);
        }, { { SIZE24 } }));

        End = SPtr<Operation>(new Operation(
            "End Program", "END", "end",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.flag_exit = true;
        }, {}));
    }
}