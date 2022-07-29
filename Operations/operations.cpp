
#include <iomanip>
#include <sstream>

#include "operations.h"

using namespace Juse;

std::ostream &Juse::operator<<(std::ostream &os, const U8 c) {
  os << (int)(unsigned char)c;
  return os;
}

void Debug::dumpState(Cpu &cpu) {
  using namespace std;
  cout << "IP=" << cpu.instruction() << endl;
}

void Debug::dumpOperations(Cpu &cpu) {
  using namespace std;
  cout << cpu.operations.size() << " operations loaded." << endl;
  cout << "===================================================================="
          "==========================================="
       << endl;
  cout << setfill(' ') << right << setw(4) << "OP  "
       << " : " << setw(48) << left << "Jumne code"
       << " | " << setw(32) << "JuseLang code"
       << " | Operation name" << endl;
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

void Debug::dumpProgram(Machine &m, const U8 from, const U8 size, U32 segmentId,
                        U16 poolID) {
  const size_t align = 32;

  using namespace std;
  S<Segment> segment = m.getSegment(poolID, segmentId);
  cout << "Program\n       ";
  for (U8 addr = 0; addr < align; addr++) {
    cout << setfill('0') << right << hex << setw(2) << addr << " ";
  }
  cout << endl;

  U8 addr = 0;
  for (U8 data : *segment) {
    if (addr < from) {
      continue;
    }
    if (addr % align == 0) {
      cout << endl << setfill('0') << right << hex << setw(4) << addr << " | ";
    }
    cout << setfill('0') << right << hex << setw(2) << data << " ";
    addr++;
    if (addr >= from + size) {
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
  // createPointersOperations(cpu);
  createMemoryOperations(cpu);
  createStackOperations(cpu);
}