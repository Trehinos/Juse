
#include "utility.h"
#include "Machine/Machine.h"

/*
    Forward circular memory[pool][segment][addr] += steps;
*/
void Juse::Utility::MachineMemory::copyStreamInMemory(Machine& machine, std::istream& file) {
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
Juse::SPtr<Juse::Pool> Juse::Utility::MachineMemory::pool(SPtr<Memory> memory, PoolId poolIndex) {
    return memory->at(poolIndex);
}
Juse::SPtr<Juse::Segment> Juse::Utility::MachineMemory::segment(SPtr<Pool> pool, SegmentId segmentIndex) {
    return pool->at(segmentIndex);
}

void Juse::Utility::MachineMemory::write(SPtr<Memory> memory, ByteSet set, Address addr) {
    SPtr<Pool> p = pool(memory, addr.pool);
    Address a = addr;
    for (U8 byte : set) {
        segment(pool(memory, a.pool), a.segment)->at(a.address) = byte;
        a++;
    }
}
Juse::ByteSet Juse::Utility::MachineMemory::read(SPtr<Memory> memory, Address address, size_t nb_bytes) {
    ByteSet bytes {};
    Address a = address;
    for (size_t i = 0; i < nb_bytes; i++) {
        U8 byte = segment(pool(memory, a.pool), a.segment)->at(a.address);
        bytes.push_back(byte);
        a++;
    }
    return bytes;
}