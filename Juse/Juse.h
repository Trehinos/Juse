#pragma once

#include <fstream>

#include "operations.h"
#include "../compilers/Jumne.h"
#include "utility.h"
#include "Debug/Debug.h"

namespace Juse {
    const S8 VERSION = "0.3.5";

    std::tuple<S8, Map<S8, S8>> parseArgs(int argc, char* argv[]);

    struct Control {
        static const int ERROR_MODE = 0x000F;
        static const int NOT_IMPLEMENTED_MODE = 0x0001;

        bool debug;
        Control(bool d = false) : debug{ d } {}
        static Machine createMachine(S8 cpuSet, U32 frequency = Cpu::BASE_FREQUENCY)
        {
            Machine m{ std::cin, std::cout };
            Operations::initCpu(m.cpus.at(0), cpuSet, frequency);
            return m;
        }
        static void loadRom(Machine& machine, S8 fileName)
        {
            std::ifstream stream(fileName, std::ios::binary);
            Utility::MachineMemory::copyStreamInMemory(machine, stream);
        }
        void runMachine(Machine& machine)
        {
            machine.run(debug);
        }
        static void createCompiler()
        {
        }

        static int juseMain(S8 program, Map<S8, S8> options)
        {
            using namespace std;
            
            // Options interpretations
            bool debug = options["mode"] == "debug";
            bool execute = debug || options["mode"] == "normal";
            bool compile = options["compile"] == "compile";
            bool help = options["display"] == "help";
            bool usage = options["display"] == "usage";

            // Dump information
            cout << "Trehinos/Juse " << VERSION << "\n"
                << "(c)2022 Trehinos\n " << endl;

            Control juse{ debug };

            if (usage) {
                cout << "Usage : Juse {-h} | {-u} | {-d program.juse} | {-c program.asm > program.juse}" << endl;
                return 0;
            }
            if (compile) {
                createCompiler();
                return NOT_IMPLEMENTED_MODE;
            }

            // Init machine
            auto machine = createMachine("std/std_ext", 1);
            loadRom(machine, program);
            if (help) {
                Debug::dumpOperations(machine.cpus.at(0));
            }
            if (execute) {
                if (debug) {
                    Juse::Debug::dumpProgram(machine);
                }
                // Run program
                if (debug) {
                    cout << endl
                        << "Debug program " << program << endl;
                }
                juse.runMachine(machine);
                return 0;
            }
            if (help) {
                return 0;
            }

            return ERROR_MODE;
        }
    };

    std::tuple<S8, Map<S8, S8>>
        parseArgs(int argc, char* argv[])
    {
        if (argc < 2) {
            return {};
        }

        S8 program{};
        Map<S8, S8> options;
        options["mode"] = "normal";
        options["compile"] = "none";
        options["display"] = "none";

        bool in_flag = false;
        char flag_buffer = 0;
        for (size_t i = 1; i < argc; i++) {
            S8 current{ argv[i] };
            if (!in_flag) {
                if (current[0] == '-') {
                    in_flag = true;
                    flag_buffer = current[1];
                    switch (flag_buffer) {
                    case 'h':
                        options["display"] = "help";
                        in_flag = false;
                        flag_buffer = 0;
                        break;
                    case 'd':
                        options["mode"] = "debug";
                        in_flag = false;
                        flag_buffer = 0;
                        break;
                    case 'u':
                        options["display"] = "usage";
                        in_flag = false;
                        flag_buffer = 0;
                        break;
                    case 'c':
                        options["compile"] = "compile";
                        in_flag = false;
                        flag_buffer = 0;
                        break;

                    }
                }
                else {
                    if (program.size() > 0) {
                        throw std::invalid_argument("parseArgs, invalid argument error : Cannot specify multiple program names.");
                    }
                    program = current;
                }
            }
            else {
            }
        }

        return std::make_tuple(program, options);
    }

} // namespace Juse
