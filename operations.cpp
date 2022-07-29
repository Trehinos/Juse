
#include <iomanip>
#include <sstream>

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
          "==========================================="
       << endl;
  cout << setfill(' ') << right << setw(4) << "OP  "
       << " : " << setw(48) << left << "Mnemu code"
       << " | " << setw(32) << "Ecal code"
       << " | Operation name"
       << endl;
  cout << "===================================================================="
          "==========================================="
       << endl;
  for (pair<U16, S<Operation>> operation : cpu.operations) {
    std::stringstream arguments_str{};

    arguments_str << left << setw(16) << operation.second->mnemuCode();
    char argIndex = 'A';
    for (Argument arg : operation.second->getArgumentDefs()) {
      arguments_str << argIndex++ << "(U" << sizes.at(arg.size) << ") ";
    }

    cout << setfill('0') << right << hex << setw(4) << operation.first << " : "
         << setfill(' ') << setw(48) << left << arguments_str.str() << " | "
         << setw(32) << operation.second->ecalCode();

    cout << " | " << operation.second->getName() << endl;
  }
  cout << "===================================================================="
          "===========================================\n"
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
  //createPointersOperations(cpu);
  createMemoryOperations(cpu);
  createStackOperations(cpu);
}