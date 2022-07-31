
#include "Tests.h"

std::array<Juse::U8, 256> Juse::Tests::helloWorld()
{
    return { /*000*/ 0x01, 0x03, 0x00, 0x10, 'H', 'e', 'l', 'l',
        /*008*/ 'o', ' ', 'w', 'o', 'r', 'l', 'd', 0x00,
        /*010*/ 0x13, 0x10, 0x00, 0x04, 0x00, 0x0F };
}