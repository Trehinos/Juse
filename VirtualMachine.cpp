
#include <iostream>
#include <iomanip>
#include "Juse.h"

using namespace std;
using namespace Juse;

void dumpState(Cpu& cpu)
{
    cout << cpu.instruction() << endl;
}

void test(Machine& m)
{
    U16 opid = 0x0103;

    for (pair<U16, S<Operation>> operation : m.cpu.operations) 
    {
        cout << setfill('0') << hex << setw(4) << operation.first << " : " << operation.second->mnemuCode() << endl;
    }

    Operation current = *(m.cpu.operations.find(opid))->second;

    dumpState(m.cpu);
    current(m, Instruction({{0x01, 0x03, 0x00, 0xA4}}));
    dumpState(m.cpu); 
}


int main()
{
    Machine machine = Machine();

    init(machine.cpu);
    //machine.run();
    test(machine);
}
