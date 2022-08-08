#include <algorithm>

#include "Machine.h"

using namespace Juse;

U64 Juse::set2word(ByteSet bytes)
{
    U64 word = 0;
    for (U8 byte : bytes) {
        word <<= 8;
        word += byte;
    }
    return word;
}

template <IsWord T>
ByteSet Juse::word2set(T byte)
{
    ByteSet set {};
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
    : memory {}
    , cpus {}
    , in { ins }
    , out { outs }
{
    memory = makeS<Memory>();
    createSegment(0, 0);
    cpus.push_back(Cpu {});
}

Machine Machine::loadFile(std::string filename)
{
    return Machine(std::cin, std::cout);
}

Machine Juse::Machine::fromData(Segment& segment)
{
    Machine machine = Machine(std::cin, std::cout);

    S<Segment> current = machine.getSegment(0, 0);
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

S<Pool> Juse::Machine::getPool(U16 pool_index)
{
    return (*memory)[pool_index];
}

S<Segment> Juse::Machine::getSegment(U16 pool_index, U32 segment_index)
{
    return (*getPool(pool_index))[segment_index];
}


ByteSet Machine::read(Cpu& cpu, size_t nb_bytes)
{
    ByteSet bytes {};
    for (size_t i = 0; i < nb_bytes; i++) {
        bytes.push_back(cpu.data(*memory));
        cpu.forward(*memory);
    }
    return bytes;
}

ByteSet Machine::readAt(Cpu& cpu, U64 address, size_t nb_bytes)
{
    ByteSet bytes {};
    for (U64 i = address; i < address + nb_bytes; i++) {
        bytes.push_back(cpu.dataAt(*memory, i));
    }
    return bytes;
}

ByteSet Machine::readData(Cpu& cpu, U16 address, size_t nb_bytes)
{
    return readAt(cpu, Address::with(cpu.data_pool, cpu.data_segment, address), nb_bytes);
}

void Juse::Machine::writeAt(Cpu& cpu, U64 address, ByteSet set)
{
    for (U8 byte : set) {
        cpu.set(*memory, address, byte);
    }
}

void Juse::Machine::writeData(Cpu& cpu, U16 address, ByteSet set)
{
    for (U8 byte : set) {
        cpu.set(*memory, Address::with(cpu.data_pool, cpu.data_segment, address), byte);
    }
}

void Machine::run(bool debug)
{
    for (Cpu& cpu : cpus) {
        auto thread = cpu.start(*this, debug);
        thread.join();
    }
}
