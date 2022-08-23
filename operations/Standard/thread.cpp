
#include "../../Juse/Machine/Thread.h"
#include "thread.h"

/* 04xx */
namespace Juse::Operations::Standard::Thread
{

    void add(OperationMap& operations) {
        operations[0x0400] = ChangeFrequency;
        operations[0x0401] = Thread;
        operations[0x0402] = ShortThread;
        operations[0x0403] = ThreadAt;
    }
    void init() {
        ChangeFrequency = SPtr<Operation>(new Operation(
            "Change Frequency", "FREQ", "conf_freq = A",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U32 freq = U32(arguments[0].value);
            cpu.config_frequency = freq;
        }, { { SIZE32 } }));

        Thread = SPtr<Operation>(new Operation(
            "Thread", "THREAD", "thread A at B",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Juse::Thread::addThread(machine, cpu, Address::from(arguments[0].value), U32(arguments[1].value));
        }, { { SIZE64 }, { SIZE32 } }));

        ShortThread = SPtr<Operation>(new Operation(
            "Short Thread", "STHREAD", "thread CP:CS:A at B",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Juse::Thread::addThread(machine, cpu, Address {
                    cpu.pool(),
                    cpu.segment(),
                    DataId(arguments[0].value)
                },
                U32(arguments[1].value)
            );
        }, { { SIZE24 }, { SIZE32 } }));

        ThreadAt = SPtr<Operation>(new Operation(
            "Thread At", "THREADAT", "thread [A] at B",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U64 data = set2word(machine.readData(cpu, DataId(arguments[0].value)));
            Juse::Thread::addThread(machine, cpu, Address::from(data), U32(arguments[1].value));
        }, { { SIZE24 }, { SIZE32 } }));

    }
}