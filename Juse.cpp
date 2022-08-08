
#include "Juse.h"
#include "Debug/Debug.h"
#include "Tests/Tests.h"

using namespace std;
using namespace Juse;

int main(int argc, char* argv[])
{
    // Command-line arguments parsing
    string program; // only used for debug ; will be used to load from file
    map<string, string> options;
    tie(program, options) = parseArgs(argc, argv);

    // Options interpretations
    bool debug = options["mode"] == "debug";
    bool help = options["display"] == "help";
    bool usage = options["display"] == "usage";

    // Init machine
    auto machine = Machine::loadFile(program);
    init(machine.cpus[0]);

    // Dump information
    cout << "Trehinos/Juse " << VERSION << "\n"
         << "(c)2022 Trehinos\n " << endl;
    if (help) {
        Debug::dumpOperations(machine.cpus[0]);
        return 0;
    }
    if (usage) {
        cout << "Usage : Juse [-h|u] | [-d] program.juse" << endl;
        return 0;
    }
    if (debug) {
        Debug::dumpProgram(machine);
    }

    // Run program
    if (debug) {
        cout << endl
             << "Debug program " << program << endl;
    }
    machine.run(debug);
}
