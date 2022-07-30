#pragma once

#include "../Models/types.h"

namespace Juse {

Instruction getInstructionFromId(Machine& machine, Operation& operation, U16 identifier);
void debugInstruction(Machine& machine, Operation& operation, Instruction& instruction);

struct Registers {
    GeneralRegisters<U8> bytes;
    GeneralRegisters<U16> words;
    GeneralRegisters<U32> quads;
    GeneralRegisters<U64> longs;
    CompareFlags compareFlags;
};

class Cpu {

    /*
   * Registers
   */
    U16 pool_pointer;
    U32 segment_pointer;
    U16 instruction_pointer;

public:
    bool flag_exit;
    bool flag_debug;
    bool flag_skip;

    Registers registers;

    static S<Operation> NoOp;
    OperationMap operations;

    Cpu();
    void initOperations();
    void forward();
    U64 addressPointer();
    void jump(U16, U32, U16);
    void longjump(U64);
    bool shouldExit();

    void cycle(Machine&, bool = false);

    U8 dataAt(Memory&, U64);
    U8 data(Memory&);

    void set(Memory&, U64, U8);

    U16 pool();
    U16 segment();
    U16 instruction();
};

} // namespace Juse