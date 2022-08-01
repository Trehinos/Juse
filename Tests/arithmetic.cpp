
#include "Tests.h"

std::array<Juse::U8, 256> Juse::Tests::arithmetic()
{
    return {
        0x14, 0x00, 0, 0, 1, // Words[0] = 1
        0x14, 0x00, 1, 0x27, 0x0f, // Words[1] = 9999
        0x15, 0x00, 2, 0, 1, 255, // Words[2] = Words[0] + Words[1] CR Words[255]
        0x17, 0x00, 2, // out Words[2]
        0x17, 0x00, 255, // out Words[255]
        0x00, 0x0F // end
    };
}
