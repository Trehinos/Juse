#pragma once

#include <iostream>
#include <stdexcept>
#include <utility>

#include "Cpu.h"

namespace Juse
{

    /*
     * set2word: returns a U64 from a ByteSet. The ByteSet MUST not be bigger
     * than 8.
     */
    U64 set2word(ByteSet);

    /*
     *
     */
    template <IsWordOrAddr T>
    ByteSet word2set(T);

    class Machine
    {

    public:
        std::ostream& out;
        std::istream& in;

        Vector<Cpu> cpus;
        SPtr<Memory> memory;

        Machine(std::istream&, std::ostream&);
        static Machine loadFile(S8);
        static Machine fromData(Segment&);

        void createCpu();

        void createPool(PoolId);
        void createSegment(PoolId, SegmentId);
        void writeAt(U64, ByteSet);
        void writeData(Cpu&, DataId, ByteSet);
        void run(bool = false);

        SPtr<Pool> getPool(PoolId);
        SPtr<Segment> getSegment(PoolId, SegmentId);
        U8 data(U64);
        ByteSet readAt(U64, size_t = 1);
        ByteSet readData(Cpu&, DataId, size_t = 1);
        ByteSet readAndForward(Cpu*, size_t = 1);
    };
} // namespace Juse
