
#include "Juse.h"

using namespace std;
using namespace Juse;

array<U8, 256> testProgram() {
  return {/*000*/ 0x01, 0x03, 0x00, 0x10, 'H',  'e',  'l',  'l',
          /*008*/ 'o',  ' ',  'w',  'o',  'r',  'l',  'd',  0x00,
          /*010*/ 0x10, 0xF0, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00,
          /*018*/ 0x10, 0x10, 0x00, 0x10, 0x00, 0x00, 0x01, 0xFF};
}

int main(int argc, char *argv[]) {
  // Command-line arguments parsing
  std::string program;
  std::map<std::string, std::string> options;
  std::tie(program, options) = parseArgs(argc, argv);

  // Options interpretations
  bool debug = options["mode"] == "debug";

  // Load program
  auto segment = makeS<Segment>();
  auto source = testProgram();
  copy(source.begin(), source.end(), segment->begin());

  // Init machine
  auto machine = Machine::fromData(*segment); // TODO : run program from command line
  init(machine.cpu);

  // Dump information
  cout << "Trehinos/Juse " << VERSION << "\n(c) 2022 Trehinos\n" << endl;
  if (debug) {
    dumpOperations(machine.cpu);
    dumpProgram(machine, 256);
  }

  // Run program
  if (debug) {
    cout << endl << "Debug program " << program << endl;
  }
  machine.run(debug);
}
