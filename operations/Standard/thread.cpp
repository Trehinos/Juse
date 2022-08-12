
#include "thread.h"

/* 04xx */
namespace Juse::Operations::Standard::Thread {

    void add(OperationMap& operations)
    {
        operations[0x0400] = ChangeFrequency;
        operations[0x0401] = Thread;
        operations[0x0402] = ShortThread;
        operations[0x0403] = ThreadAt;
    }

    void thread(Machine& machine, Cpu& cpu, Address address, U32 freq)
    {
        Cpu thread = Cpu{ cpu };
        if (freq != 0)
            thread.config_frequency = freq;

        thread.jump(address.pool, address.segment, address.datum);
        thread.start(machine, cpu.flag_debug);
        machine.cpus.push_back(thread);
    }

    void init()
    {
        ChangeFrequency = S<Operation>(new Operation(
            "Change Frequency", "FREQ", "conf_freq = A",
            [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U32 freq = U32(arguments[0].value);
            cpu.config_frequency = freq;
        },
            { { SIZE32 } }));
        Thread = S<Operation>(new Operation(
            "Thread", "THREAD", "thread A at B",
            [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Thread::thread(machine, cpu, Address::from(arguments[0].value), U32(arguments[1].value));
        },
            { { SIZE64 }, { SIZE32 } }));
        ShortThread = S<Operation>(new Operation(
            "Short Thread", "STHREAD", "thread CP:CS:A at B",
            [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Thread::thread(machine, cpu, Address{ cpu.pool(), cpu.segment(), U16(arguments[0].value) }, U32(arguments[1].value));
        },
            { { SIZE16 }, { SIZE32 } }));
        ThreadAt = S<Operation>(new Operation(
            "Thread At", "THREADAT", "thread [A] at B",
            [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U64 data = set2word(machine.readData(cpu, U16(arguments[0].value)));
            Thread::thread(machine, cpu, Address::from(data), U32(arguments[1].value));
        },
            { { SIZE16 }, { SIZE32 } }));

    }
}