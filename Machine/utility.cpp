
#include "utility.h"

void Juse::Utility::Memory::forward(U16& pool, U32& segment, U16& addr, size_t steps)
{
    U32 tmpAddr = U32(addr);
    tmpAddr += U32(steps);
    if (addr >= UINT16_MAX) {
        addr = tmpAddr % UINT16_MAX;
        if (segment == UINT32_MAX) {
            segment = 0;
            if (pool == UINT16_MAX) {
                pool = 0;
            } else {
                pool++;
            }
        } else {
            segment++;
        }
    }
}
Juse::S<Juse::Pool> Juse::Utility::Memory::pool(Juse::Memory& memory, U16 poolIndex)
{
    return memory.at(poolIndex);
}
Juse::S<Juse::Segment> Juse::Utility::Memory::segment(Juse::Pool& pool, U32 segmentIndex)
{
    return pool.at(segmentIndex);
}

void Juse::Utility::Memory::write(Juse::Memory& memory, ByteSet set, Address addr)
{
    S<Pool> p = pool(memory, addr.pool);
    for (U8 byte : set) { 
        (*segment(*p, addr.segment))[addr.datum++] = byte;
    }
}
