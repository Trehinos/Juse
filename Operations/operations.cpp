#include <iomanip>
#include <iostream>
#include <ranges>
#include <string_view>

#include "operations.h"

void Juse::out(std::ostream& os, Juse::SS8& ss, bool debug)
{
    if (debug) {
        os << "  << ";
    }
    os << ss.str() << std::flush;
    if (debug) {
        os << std::endl;
    }
}

std::string Juse::in(std::ostream& os, std::istream& is, bool debug)
{
    std::string str {};
    if (debug) {
        os << "  >> ";
    }
    std::getline(is >> std::ws, str);
    is.clear();
    return str;
}

void Juse::init(Juse::Cpu& cpu, const std::string type)
{
    cpu.initOperations();
    loadOperationSet(cpu, type);
}

std::vector<std::string> split(std::string s, std::string delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

void Juse::loadOperationSet(Juse::Cpu& cpu, const std::string types)
{

    for (const auto& c : split(types, "/")) {
        if (c == "std") {
            Operations::Standard::branching(cpu);
            Operations::Standard::move(cpu);
            Operations::Standard::logic(cpu);
            Operations::Standard::allocation(cpu);
            Operations::Standard::thread(cpu);
            continue;
        }
        if (c == "u8" || c == "std_ext") {
            Operations::StandardExtensions::ext_u8(cpu);
        }
        if (c == "u16" || c == "std_ext") {
            Operations::StandardExtensions::ext_u16(cpu);
        }
        if (c == "u32" || c == "std_ext") {
            Operations::StandardExtensions::ext_u32(cpu);
        }
        if (c == "u64" || c == "std_ext") {
            Operations::StandardExtensions::ext_u64(cpu);
        }
        if (c == "str") {
            //Operations::StandardExtensions::ext_string(cpu);
        }
        if (c == "float") {
            //Operations::StandardExtensions::ext_float(cpu);
        }
    }
}