
#include "operations.h"

#include "thread.h"

/* 04xx */
void Juse::Operations::Standard::Thread::thread(Machine& machine, Cpu& cpu, Address address, U16 freq)
{
    Cpu thread = Cpu { cpu };
    if (freq != 0)
        thread.config_frequency = freq;

    thread.jump(address.pool, address.segment, address.datum);
    thread.start(machine, cpu.flag_debug);
    machine.cpus.push_back(thread);
}

void Juse::Operations::Standard::thread(Cpu& cpu)
{
    cpu.operations[0x0400] = S<Operation>(new Operation(
        "Change Frequency", "FREQ", "conf_freq = A",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U16 freq = U16(arguments[0].value);
            cpu.config_frequency = freq;
        },
        { { SIZE16 } }));
    cpu.operations[0x0401] = S<Operation>(new Operation(
        "Thread", "THREAD", "thread A at B",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Thread::thread(machine, cpu, Address::from(arguments[0].value), U16(arguments[1].value));
        },
        { { SIZE64 }, { SIZE16 } }));
    ;
    cpu.operations[0x0402] = S<Operation>(new Operation(
        "Short Thread", "STHREAD", "thread CP:CS:A at B",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Thread::thread(machine, cpu, Address { cpu.pool(), cpu.segment(), U16(arguments[0].value) }, U16(arguments[1].value));
        },
        { { SIZE16 }, { SIZE16 } }));
    cpu.operations[0x0403] = S<Operation>(new Operation(
        "Thread At", "THREADAT", "thread [A] at B",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U64 data = set2word(machine.readData(cpu, U16(arguments[0].value)));
            Thread::thread(machine, cpu, Address::from(data), U16(arguments[1].value));
        },
        { { SIZE16 }, { SIZE16 } }));
}