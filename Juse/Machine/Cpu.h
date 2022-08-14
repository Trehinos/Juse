#pragma once

#include <thread>

#include "../types.h"

namespace Juse {

    Instruction getInstructionFromId(Machine& machine, Cpu* cpu, Operation* operation, U16 identifier);
    void debugInstruction(Machine& machine, Cpu* cpu, Operation* operation, Instruction& instruction);

    struct Registers {
        GeneralRegisters<U8> bytes;
        GeneralRegisters<U16> words;
        GeneralRegisters<U32> quads;
        GeneralRegisters<U64> longs;
        CompareFlags compareFlags;

        static CompareFlags createFlags()
        {
            CompareFlags flags{};
            flags[CompareFlag::EQ] = false;
            flags[CompareFlag::GT] = false;
            flags[CompareFlag::LW] = false;
            flags[CompareFlag::GE] = false;
            flags[CompareFlag::LE] = false;
            flags[CompareFlag::NE] = false;
            flags[CompareFlag::Z0] = false;
            flags[CompareFlag::CR] = false;
            flags[CompareFlag::OF] = false;
            flags[CompareFlag::SN] = false;
            flags[CompareFlag::ERR] = false;
            return flags;
        }
    };

    class Cpu {

        /*
       * Registers
       */
        U16 pool_pointer;
        U32 segment_pointer;
        U16 instruction_pointer;

    public:
        static const U32 BASE_FREQUENCY = 4000;

        /* Move operations location */
        U16 data_pool;
        U32 data_segment;
        U16 address_pointer;
        U16 address_offset;
        U16 address_increment;

        bool flag_exit;
        bool flag_debug;
        bool flag_skip;

        Registers registers;
        Stack stack;

        static SPtr<Operation> NoOp;
        static Duration duration(U32);
        static bool tick(U32, TimePoint, TimePoint);
        OperationMap operations;

        U32 config_frequency;

        Cpu();
        void initOperations();
        SPtr<Operation> getOperation(Machine&, U16&);

        void forward(U16=1);
        void jump(U16, U32, U16);
        void longjump(U64);
        U16 pool();
        U16 segment();
        U16 instruction();
        U64 instructionPointer();
        U64 addressPointer();
        U16 offseted();

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
