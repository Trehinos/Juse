#pragma once

#include <iostream>
#include <stdexcept>
#include <utility>

#include "../Models/types.h"
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

    std::vector<Cpu> cpus;
    S<Memory> memory;

    Machine(std::istream&, std::ostream&);
    static Machine loadFile(std::string);
    static Machine fromData(Segment&);

    void createPool(U16);
    void createSegment(U16, U32);

    S<Pool> getPool(U16);
    S<Segment> getSegment(U16, U32);

    ByteSet read(Cpu&, size_t = 1);
    ByteSet readAt(Cpu&, U64, size_t = 1);
    ByteSet readData(Cpu&, U16, size_t = 1);

    void writeAt(Cpu&, U64, ByteSet);
    void writeData(Cpu&, U16, ByteSet);

    void run(bool = false);
};
} // namespace Juse
