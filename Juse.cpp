
#include <iostream>
#include <iomanip>

#include "Juse.h"

using namespace std;
using namespace Juse;


void dumpState(Cpu& cpu)
{
    cout << "IP=" << cpu.instruction() << endl;
}

void dumpOperations(Cpu& cpu)
{
    cout << cpu.operations.size() << " operations loaded." << endl;
    cout << "====================================================================================" << endl;
    cout << setfill(' ') << right << setw(4) << "OP  " << " : " 
        << setw(10) << left << "Mnemu code" << " | " 
        << setw(32) << "Ecal code                       Operation name" << endl;
    cout << "====================================================================================" << endl;
    for (pair<U16, S<Operation>> operation : cpu.operations) {
        cout << setfill('0') << right << hex << setw(4) << operation.first << " : "
            << setfill(' ') << setw(10) << left << operation.second->mnemuCode()
            << " | "  << setw(32) << operation.second->ecalCode()
            << operation.second->getName()
            << endl;
    }
    cout << "====================================================================================\n" << endl;
}

void dumpProgram(Machine& m)
{
    for (U8 data : *m.getSegment(0, 0)) {
        cout << hex << data;
    }
}

void test(Machine& m)
{
    U16 opid = 0x0103;
    Operation current = *(m.cpu.operations.find(opid))->second;

    cout << "== START ==" << endl;
    dumpState(m.cpu);
    current(m, Instruction({ {0x01, 0x03, 0x00, 0xA4} }));
    dumpState(m.cpu);
    cout << "== END ==" << endl;
}

array<U8, 256> testProgram()
{
    return {
        /*000*/		0x01,	0x03,	0x00,	0x10,
        /*004*/		'H',	'e',	'l',	'l',
        /*008*/		'o',	' ',	'w',	'o',
        /*00B*/		'r',	'd',	0x00,	0x00,
        /*010*/		0x10,	0x10,	0x00,	0x04,
        /*014*/		0x00,	0x00,	0x00,	0x00,
        /*018*/		0x00,	0x00,	0x00,	0x00,
        /*01B*/		0x00,	0x00,	0x00,	0x00,
        /*020*/		0x00,	0x00,	0x01,	0xFF,
    };
}

int main()
{
    S<Segment> segment = makeS<Segment>();
    auto source = testProgram();
    copy(source.begin(), source.end(), segment->begin());
    Machine machine = Machine::fromData(*segment);

    init(machine.cpu);

    cout << "Trehinos/Juse " << VERSION << "\n(c) 2022 Trehinos\n" << endl;
    dumpOperations(machine.cpu);

    machine.run(true);
    //cout << "Execute a test :" << endl;
    //test(machine);
}
