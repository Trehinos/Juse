#pragma once

#include <thread>

#include "Registers.h"
#include "../utility.h"

namespace Juse
{

    Instruction getInstructionFromId(Machine& machine, Cpu* cpu, Operation* operation, OperationId identifier);
    void debugInstruction(Machine& machine, Cpu* cpu, Operation* operation, Instruction& instruction);

    class Cpu
    {

        /*
        * Registers
        */
        Address code;

    public:
        static SPtr<Operation> NoOp;
        static Duration duration(U32);
        static bool tick(U32, TimePoint, TimePoint);

        /* Registers */
        Address data_addr;
        DataId address_offset;
        DataId address_increment;

        static const U32 BASE_FREQUENCY = 4000;
        U32 config_frequency = BASE_FREQUENCY;

        bool flag_exit;
        bool flag_debug;
        bool flag_skip;

        Registers registers;
        Stack stack;

        OperationMap operations;

        Cpu();
        void initOperations();
        SPtr<Operation> getOperation(Machine&, OperationId&);
        SPtr<Operation> findOperation(Machine&, OperationId);

        void forward(size_t = 1);
        void jump(PoolId, SegmentId, DataId);
        void longjump(U64);
        PoolId pool();
        SegmentId segment();
        DataId instruction();
        U64 instructionPointer();
        U64 addressPointer();
        DataId offseted();

        void push(U8);
        void multiPush(ByteSet);
        U8 pop();
        ByteSet multiPop(size_t);

        void set(Machine&, U64, U8);
        U8 dataAt(Machine&, U64);
        U8 data(Machine&);

        void run(Machine&, bool = false);
        void cycle(Machine&, bool = false);
        std::thread start(Machine&, bool);

        bool shouldExit();

    };

} // namespace Juse
