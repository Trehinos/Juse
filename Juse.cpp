
#include "Juse.h"

using namespace std;
using namespace Juse;

array<U8, 256> testProgram()
{
    return { /*000*/ 0x01, 0x03, 0x00, 0x10, 'H', 'e', 'l', 'l',
        /*008*/ 'o', ' ', 'w', 'o', 'r', 'l', 'd', 0x00,
        /*010*/ 0x00, 0x09, 0x13, 0x10, 0x00, 0x04, 0x00, 0x00,
        /*018*/ 0x13, 0x01, 0x00, 0x13, 0x00, 0x00, 0x00, 0x0F };
}

array<U8, 256> testRandom()
{
    return { 0x14, 0x00, 0, 0, 0, 0x14, 0x00, 1, 0, 100, 0x15, 0x06, 2, 0, 1, 0x17, 0, 2, 0x15, 0x06, 2, 0, 1, 0x17, 0, 2, 0x15, 0x06, 2, 0, 1, 0x17, 0, 2, 0x15, 0x06, 2, 0, 1, 0x17, 0, 2, 0, 0x0F };
}

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
    auto source = testRandom();
    copy(source.begin(), source.end(), segment->begin());

    // Init machine
    auto machine = Machine::fromData(*segment); // TODO : run program from command line
    init(machine.cpu);

    // Dump information
    cout << "Trehinos/Juse " << VERSION << "\n(c) 2022 Trehinos\n"
         << endl;
    if (debug) {
        Debug::dumpOperations(machine.cpu);
        Debug::dumpProgram(machine);
    }

    // Run program
    if (debug) {
        cout << endl
             << "Debug program " << program << endl;
    }
    machine.run(debug);
}
