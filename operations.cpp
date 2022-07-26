
#include <iomanip>

#include "operations.h"

using namespace Juse;

void Juse::dumpState(Cpu &cpu) {
  using namespace std;
  cout << "IP=" << cpu.instruction() << endl;
}

void Juse::dumpOperations(Cpu &cpu) {
  using namespace std;
  cout << cpu.operations.size() << " operations loaded." << endl;
  cout << "===================================================================="
          "================"
       << endl;
  cout << setfill(' ') << right << setw(4) << "OP  "
       << " : " << setw(10) << left << "Mnemu code"
       << " | " << setw(32) << "Ecal code                       Operation name"
       << endl;
  cout << "===================================================================="
          "================"
       << endl;
  for (pair<U16, S<Operation>> operation : cpu.operations) {
    cout << setfill('0') << right << hex << setw(4) << operation.first << " : "
         << setfill(' ') << setw(10) << left << operation.second->mnemuCode()
         << " | " << setw(32) << operation.second->ecalCode()
         << operation.second->getName() << endl;
  }
  cout << "===================================================================="
          "================\n"
       << endl;
}

void Juse::dumpProgram(Machine &m, size_t max) {
  const size_t align = 32;

  using namespace std;
  S<Segment> segment = m.getSegment(0, 0);
  cout << "Program\n       ";
  for (size_t addr = 0; addr < align; addr++) {
    cout << setfill('0') << right << hex << setw(2) << addr << " ";
  }
  cout << endl;

  size_t addr = 0;
  for (U8 data : *segment) {
    if (addr % align == 0) {
      cout << endl << setfill('0') << right << hex << setw(4) << addr << " | ";
    }
    cout << setfill('0') << right << hex << setw(2) << (int)(unsigned char)data
         << " ";
    addr++;
    if (addr >= max) {
      cout << endl;
      return;
    }
  }
}

void Juse::init(Cpu &cpu) {
  cpu.initOperations();
  // Creation of all operations
  createControlOperations(cpu);
  createRegistersOperations(cpu);
  createIoOperations(cpu);
  createMemoryOperations(cpu);
}