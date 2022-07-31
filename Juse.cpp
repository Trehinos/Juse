
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

array<U8, 256> testGame()
{
    return {
        0x00, 0x01, 0x00, 93, // goto @main
        // @messagePrompt [4]
        'D', 'e', 'v', 'i', 'n', 'e', 'z', ' ', 'l', 'e', ' ', 'n', 'o', 'm', 'b', 'r', 'e', ' ',
        'a', 'u', 'q', 'u', 'e', 'l', ' ', 'j', 'e', ' ', 'p', 'e', 'n', 's', 'e', ' ', ':', 0, // #DB~36

        // @more [40]
        'C', '\'', 'e', 's', 't', ' ', 'p', 'l', 'u', 's', '\n', 0, // #DB~12

        // @less [52]
        'C', '\'', 'e', 's', 't', ' ', 'm', 'o', 'i', 'n', 's', '\n', 0, // #DB~13

        // @victoryMsg [65]
        'B', 'r', 'a', 'v', 'o', ',', ' ', 'v', 'o', 'u', 's', ' ', 'a', 'v', 'e', 'z', ' ', 't', 'r', 'o', 'u', 'v', 'e', ' ', '!', 0, // #DB~26

        // @number [91]
        0x00, 0x00, // #DW

        // @main [93]
        0x14, 0x00, 0, 0x00, 0x00, // Words[0] = 0
        0x14, 0x00, 1, 0x27, 0x0f, // Words[1] = 9999
        0x15, 0x06, 2, 0, 1, // Words[2] = RND(0,1)
        0x14, 0x02, 0, 91, 2, // [@number] = Words[2]

        // @loop [113]
        0x00, 0x04, 0x00, 151, // call @prompt
        0x14, 0x01, 1, 0, 91, // Words[1] = [@number]
        0x15, 0xf0, 0, 1, // Words[0] ? Words[1]
        0x00, 0x08, 0, // If =
        0x00, 0x01, 0x00, 160, // goto @victory
        0x00, 0x08, 2, // If <
        0x13, 0x10, 0, 40, // out S8 @more
        0x00, 0x08, 1, // If >
        0x13, 0x10, 0, 52, // out S8 @less
        0x00, 0x01, 0x00, 113, // goto @loop

        // @prompt [151]
        0x13, 0x10, 0, 4, // out S8 @messagePrompt
        0x17, 0x01, 0, // in Words[0]
        0x00, 0x05, // return

        // @victory [160]
        0x13, 0x10, 0, 65, // out S8 @victoryMsg
        0x00, 0x0f, // end
    };
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
    auto source = testGame();
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
