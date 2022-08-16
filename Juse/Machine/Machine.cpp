#include <algorithm>
#include <fstream>
#include <istream>

#include "Machine.h"
#include "../utility.h"

using namespace Juse;

U64 Juse::set2word(ByteSet bytes)
{
    U64 word = 0;
    size_t index = 0;
    for (U8 byte : bytes) {
        if (++index > 8) break;
        word <<= 8;
        word += byte;
    }
    return word;
}

template <IsWord T>
ByteSet Juse::word2set(T byte)
{
    ByteSet set{};
    size_t size = sizeof(byte);
    size_t offset = 0;
    U64 mask = 0xFF;
    for (size_t i = 0; i < size; i++) {
        set.push_back(U8((mask & byte) >> offset));
        mask <<= 8;
        offset += 8;
    }
    return set;
}

template ByteSet Juse::word2set<U8>(U8);
template ByteSet Juse::word2set<U16>(U16);
template ByteSet Juse::word2set<U32>(U32);
template ByteSet Juse::word2set<U64>(U64);

Machine::Machine(std::istream& ins, std::ostream& outs)
    : memory{}
    , cpus{}
    , in{ ins }
    , out{ outs }
{
    memory = makeS<Memory>();
    createSegment(0, 0);
    cpus.push_back(Cpu{});
}

Machine Machine::loadFile(S8 filename)
{
    std::ifstream file(filename, std::ios::binary);
    Machine machine(std::cin, std::cout);
    Utility::MachineMemory::copyStreamInMemory(machine, file);

    return machine;
}

Machine Juse::Machine::fromData(Segment& segment)
{
    Machine machine = Machine(std::cin, std::cout);

    SPtr<Segment> current = machine.getSegment(0, 0);
    std::copy(segment.begin(), segment.end(), current->begin());

    return machine;
}

void Machine::createPool(U16 pool_index)
{
    memory->insert({ pool_index, makeS<Pool>() });
}

void Machine::createSegment(U16 pool_index, U32 segment_index)
{
    if (!memory->contains(pool_index)) {
        createPool(pool_index);
    }
    ((*memory)[pool_index])->insert({ segment_index, makeS<Segment>() });
}

SPtr<Pool> Juse::Machine::getPool(U16 pool_index)
{
    return Utility::MachineMemory::pool(memory, pool_index);
}

SPtr<Segment> Juse::Machine::getSegment(U16 pool_index, U32 segment_index)
{
    return Utility::MachineMemory::segment(getPool(pool_index), segment_index);
}

/*
* Read & Forward
*/
ByteSet Machine::readAndForward(Cpu* cpu, U16 nb_bytes)
{
    ByteSet set = readAt(cpu->instructionPointer(), nb_bytes);
    cpu->forward(nb_bytes);
    return set;
}

ByteSet Machine::readAt(U64 address, U16 nb_bytes)
{
    return Utility::MachineMemory::read(memory, Address::from(address), nb_bytes);
}

U8 Machine::data(U64 address)
{
    return U8(set2word(readAt(address, 1)));
}

ByteSet Machine::readData(Cpu& cpu, U16 address, U16 nb_bytes)
{
    return readAt(Address::with(cpu.data_pool, cpu.data_segment, address), nb_bytes);
}

void Juse::Machine::writeAt(U64 address, ByteSet set)
{
    Utility::MachineMemory::write(memory, set, Address::from(address));
}

void Juse::Machine::writeData(Cpu& cpu, U16 address, ByteSet set)
{
    writeAt(Address::with(cpu.data_pool, cpu.data_segment, address), set);
}

void Machine::run(bool debug)
{
    for (Cpu& cpu : cpus) {
        auto thread = cpu.start(*this, debug);
        thread.join();
    }
}
