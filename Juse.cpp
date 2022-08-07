
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

    // Load program
    auto segment = makeS<Segment>();
    auto source = Tests::higherLowerGame();
    copy(source.begin(), source.end(), segment->begin());

    // Init machine
    auto machine = Machine::fromData(*segment); // TODO : run program from command line
    init(machine.cpus[0]);

    // Dump information
    cout << "Trehinos/Juse " << VERSION << "\n"
         << "(c)2022 Trehinos\n " << endl;
    if (debug) {
        Debug::dumpOperations(machine.cpus[0]);
        Debug::dumpProgram(machine);
    }

    // Run program
    if (debug) {
        cout << endl
             << "Debug program " << program << endl;
    }
    machine.run(debug);
}
