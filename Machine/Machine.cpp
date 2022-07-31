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
    , stack()
    , cpu()
    , in { ins }
    , out { outs }
{
    memory = makeP<Memory>();
    createSegment(0, 0);
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

void Machine::push(U8 byte) { stack.push(byte); }

U8 Machine::pop()
{
    U8 byte = stack.top();
    stack.pop();
    return byte;
}

void Juse::Machine::multiPush(ByteSet set)
{
    for (U8 byte : set) {
        push(byte);
    }
}

ByteSet Juse::Machine::multiPop(size_t nb_bytes)
{
    ByteSet bytes {};
    for (size_t i = 0; i < nb_bytes; i++) {
        bytes.insert(bytes.begin(), pop());
    }
    return bytes;
}

ByteSet Machine::read(size_t nb_bytes)
{
    ByteSet bytes {};
    for (size_t i = 0; i < nb_bytes; i++) {
        bytes.push_back(cpu.data(*memory));
        cpu.forward();
    }
    return bytes;
}

ByteSet Machine::readAt(U64 address, size_t nb_bytes)
{
    ByteSet bytes {};
    for (U64 i = address; i < address + nb_bytes; i++) {
        bytes.push_back(cpu.dataAt(*memory, i));
    }
    return bytes;
}

ByteSet Machine::readData(U16 address, size_t nb_bytes)
{
    return readAt(Address::with(cpu.data_pool, cpu.data_segment, address), nb_bytes);
}

void Juse::Machine::writeAt(U64 address, ByteSet set)
{
    for (U8 byte : set) {
        cpu.set(*memory, address, byte);
    }
}

void Juse::Machine::writeData(U16 address, ByteSet set)
{
    for (U8 byte : set) {
        cpu.set(*memory, Address::with(cpu.data_pool, cpu.data_segment, address), byte);
    }
}

S<Operation> Machine::getOperation(U16& id)
{
    ByteSet identifier = read(2);
    id = U16(set2word(identifier));

    if (!cpu.operations.contains(id)) {
        return Cpu::NoOp;
    }

    return cpu.operations[id];
}

void Machine::run(bool debug)
{
    while (!cpu.shouldExit()) {
        cpu.cycle(*this, debug);
    }
}
