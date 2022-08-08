#pragma once

#include "../Models/types.h"

namespace Juse::Operations::Thread {

void thread(Machine& machine, Cpu& cpu, Address address, U32 freq);

}
