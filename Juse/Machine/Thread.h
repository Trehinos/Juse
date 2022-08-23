#pragma once

#include "../types.h"
#include "Machine.h"

namespace Juse::Thread
{

    struct MutEx
    {
        std::mutex m;
    };

    void addThread(Machine& machine, Cpu& cpu, Address address, U32 freq) {
        Cpu thread = Cpu { cpu };
        if (freq != 0)
            thread.config_frequency = freq;

        thread.jump(address.pool, address.segment, address.address);
        thread.start(machine, cpu.flag_debug);
        machine.cpus.push_back(thread);
    }

}