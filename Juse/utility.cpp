
#include "utility.h"
#include "Machine/Machine.h"

/*
    Forward circular memory[pool][segment][addr] += steps;
*/
void Juse::Utility::MachineMemory::forward(U16& pool, U32& segment, U16& addr, U16 steps)
{
    U32 tmpAddr = U32(addr);
    tmpAddr += U32(steps);
    addr = tmpAddr % UINT16_MAX;
    if (tmpAddr >= UINT16_MAX) {
        if (segment == UINT32_MAX) {
            segment = 0;
            if (pool == UINT16_MAX) {
                pool = 0;
            }
            else {
                pool++;
            }
        }
        else {
            segment++;
        }
    }
}
void Juse::Utility::MachineMemory::copyStreamInMemory(Machine& machine, std::istream& file)
{
    U16 pool = 0;
    U32 segment = 0;
    while (!file.eof()) {
        SPtr<Segment> current = machine.getSegment(pool, segment);
        for (U8& byte : *current) {
            if (file.eof())
                break;
            file.read(reinterpret_cast<char*>(&byte), sizeof(U8));
        }
        if (!file.eof()) {
            if (++segment == UINT32_MAX) {
                pool++;
                segment = 0;
            }
            machine.createSegment(pool, segment);
        }
    }
}
void Juse::Utility::MachineMemory::forward(Address& a, U16 steps)
{
    forward(a.pool, a.segment, a.addr16, steps);
}

Juse::SPtr<Juse::Pool> Juse::Utility::MachineMemory::pool(SPtr<Memory> memory, U16 poolIndex)
{
    return memory->at(poolIndex);
}
Juse::SPtr<Juse::Segment> Juse::Utility::MachineMemory::segment(SPtr<Pool> pool, U32 segmentIndex)
{
    return pool->at(segmentIndex);
}

void Juse::Utility::MachineMemory::write(SPtr<Memory> memory, ByteSet set, Address addr)
{
    SPtr<Pool> p = pool(memory, addr.pool);
    Address a = addr;
    for (U8 byte : set) {
        segment(pool(memory, a.pool), a.segment)->at(a.addr16) = byte;
        forward(a);
    }
}
Juse::ByteSet Juse::Utility::MachineMemory::read(SPtr<Memory> memory, Address address, U16 nb_bytes)
{
    ByteSet bytes{};
    Address a = address;
    for (U16 i = 0; i < nb_bytes; i++) {
        U8 byte = segment(pool(memory, a.pool), a.segment)->at(a.addr16);
        bytes.push_back(byte);
        forward(a);
    }
    return bytes;
}