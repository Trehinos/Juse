#pragma once

#include <iostream>
#include <stdexcept>
#include <utility>

#include "Cpu.h"

namespace Juse {

/*
 * set2word: returns a U64 from a ByteSet. The ByteSet MUST not be bigger
 * than 8.
 */
U64 set2word(ByteSet);

/*
 *
 */
template <IsWord T>
ByteSet word2set(T);

class Machine {

public:
    std::ostream& out;
    std::istream& in;

    Vector<Cpu> cpus;
    SPtr<Memory> memory;

    Machine(std::istream&, std::ostream&);
    static Machine loadFile(S8);
    static Machine fromData(Segment&);

    void createPool(U16);
    void createSegment(U16, U32);

    SPtr<Pool> getPool(U16);
    SPtr<Segment> getSegment(U16, U32);

    ByteSet read(Cpu&, size_t = 1);
    ByteSet readAt(Cpu&, U64, size_t = 1);
    ByteSet readData(Cpu&, U16, size_t = 1);

    void writeAt(Cpu&, U64, ByteSet);
    void writeData(Cpu&, U16, ByteSet);

    void run(bool = false);
};
} // namespace Juse
