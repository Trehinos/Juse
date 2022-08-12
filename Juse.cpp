
#include "Juse/Juse.h"
#include "Juse/Debug/Debug.h"
#include "Juse/Tests/Tests.h"

using namespace std;
using namespace Juse;

const int ERROR_MODE = 0x000F;
const int NOT_IMPLEMENTED_MODE = 0x0001;

int main(int argc, char* argv[])
{
    // Command-line arguments parsing
    string program; // only used for debug ; will be used to load from file
    map<string, string> options;
    tie(program, options) = parseArgs(argc, argv);

    // Options interpretations
    bool debug = options["mode"] == "debug";
    bool execute = debug || options["mode"] == "normal";
    bool compile = options["compile"] == "compile";
    bool help = options["display"] == "help";
    bool usage = options["display"] == "usage";

    // Dump information
    cout << "Trehinos/Juse " << VERSION << "\n"
         << "(c)2022 Trehinos\n " << endl;

    if (usage) {
        cout << "Usage : Juse {-h} | {-u} | {-d program.juse} | {-c program.asm > program.juse}" << endl;
        return 0;
    }
    if (compile) {
        // compilation
        return NOT_IMPLEMENTED_MODE;
    }

    // Init machine
    auto machine = Machine::loadFile(program);
    init(machine.cpus[0]);
    if (help) {
        Debug::dumpOperations(machine.cpus[0]);
        return 0;
    }
    if (execute) {
        if (debug) {
            Debug::dumpProgram(machine);
        }

        // Run program
        if (debug) {
            cout << endl
                 << "Debug program " << program << endl;
        }
        machine.run(debug);
        return 0;
    }

    return ERROR_MODE;
}
