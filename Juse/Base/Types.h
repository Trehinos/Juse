#pragma once

#include "Structures.h"
#include "Integer.h"
#include "String.h"

#include <chrono>

namespace Juse
{

    using Segment = ByteArray<SEGMENT_SIZE>;
    using Pool = HeapMap<SegmentId, Segment>;
    using Memory = HeapMap<PoolId, Pool>;

    template <IsWordOrAddr Type>
    using GeneralRegisters = Array<Type, 256>;

    class Operation;
    using OperationId = U16;
    using OperationMap = HeapMap<OperationId, Operation>;

    struct Instruction
    {
        ByteSet data;

        OperationId identifier() { return OperationId(data[0] << 8) + (data[1]); }

        U64 argument(size_t offset, size_t size) {
            U64 buffer = 0;
            size_t start = sizeof(OperationId) * 8 + offset;
            for (size_t i = start; i < start + size; i++) {
                buffer <<= 8;
                buffer += data[i];
            }
            return buffer;
        }
    };

    class Cpu;

    // Comparaison flags for arithmetic and logic Cpu operations.
    enum class CompareFlag
    {
        EQ = 0,
        GT = 1,
        LW = 2,
        GE = 3,
        LE = 4,
        NE = 5,
        Z0 = 6,
        CR = 7,
        OF = 8,
        SN = 9, // signe negative
        ERR = 255
    };
    using CompareFlags = Map<CompareFlag, bool>;

    class Machine;

    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    using Duration = TimePoint::duration;

    struct OperationArgument;
    using OperationArguments = std::vector<OperationArgument>;
    using FunctionType = void(Machine&, Cpu&, OperationArguments);
    using OperationFunction = std::function<FunctionType>;

}
