
#include "Juse/Juse.h"

int main(int argc, char* argv[])
{
    // Command-line arguments parsing
    Juse::S8 program;
    Juse::Map<Juse::S8, Juse::S8> options;
    std::tie(program, options) = Juse::parseArgs(argc, argv);

    return Juse::Control::juseMain(program, options);   // displays help, or
                                                    // creates a machine and runs it, or
                                                    // creates a compiler (pending implementation), or
                                                    // returns an error code
}
