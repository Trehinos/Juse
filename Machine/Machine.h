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

    Cpu cpu;
    P<Memory> memory;
    Stack stack;

    Machine(std::istream&, std::ostream&);
    static Machine loadFile(std::string);
    static Machine fromData(Segment&);

    void createPool(U16);
    void createSegment(U16, U32);

    S<Pool> getPool(U16);
    S<Segment> getSegment(U16, U32);

    void push(U8);
    U8 pop();
    void multiPush(ByteSet);
    ByteSet multiPop(size_t);

    ByteSet read(size_t = 1);

    ByteSet readAt(U64, size_t = 1);
    ByteSet readData(U16, size_t = 1);

    void writeAt(U64, ByteSet);
    void writeData(U16, ByteSet);

    S<Operation> getOperation(U16&);

    void run(bool = false);
};
} // namespace Juse
