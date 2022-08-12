#pragma once

#include "Operations/operations.h"

namespace Juse {
const std::string VERSION = "0.3.4";

std::tuple<std::string, std::map<std::string, std::string>>
parseArgs(int argc, char* argv[])
{
    if (argc < 2) {
        return {};
    }

    std::string program {};
    std::map<std::string, std::string> options;
    options["mode"] = "normal";
    options["compile"] = "none";
    options["display"] = "none";

    bool in_flag = false;
    char flag_buffer = 0;
    for (size_t i = 1; i < argc; i++) {
        std::string current { argv[i] };
        if (!in_flag) {
            if (current[0] == '-') {
                in_flag = true;
                flag_buffer = current[1];
                switch (flag_buffer) {
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
                case 'h':
                    options["display"] = "help";
                    in_flag = false;
                    flag_buffer = 0;
                    break;
                case 'c':
                    options["compile"] = "compile";
                    in_flag = false;
                    flag_buffer = 0;
                    break;

                }
            } else {
                if (program.size() > 0) {
                    throw std::invalid_argument("parseArgs, invalid argument error : Cannot specify multiple program names.");
                }
                program = current;
            }
        } else {
        }
    }

    return std::make_tuple(program, options);
}

} // namespace Juse
