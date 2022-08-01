
#include "Tests.h"

std::array<Juse::U8, 256> Juse::Tests::arithmetic()
{
    return {
        0x1C, 0x00, 0, 0, 0, 1, 1, 1, 1, 1, 1, // Longs[0] = 1
        0x1C, 0x00, 1, 0, 0, 1, 1, 1, 1, 1, 1, // Longs[1] = 9999
        0x1D, 0x00, 2, 0, 1, 255, // Longs[2] = Longs[0] + Longs[1] CR Longs[255]
        0x1F, 0x00, 2, // out Longs[2]
        0x1F, 0x00, 255, // out Longs[255]
        0x00, 0x0F // end
    };
}
