#pragma once

#include "../operations.h"

namespace Juse::Tests {
std::array<U8, 256> helloWorld();
std::array<U8, 256> higherLowerGame();
std::array<Juse::U8, 256> arithmetic();
void testOperations(Machine&, Cpu&);
}
