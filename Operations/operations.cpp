#include <iomanip>
#include <iostream>
#include <ranges>
#include <string_view>

#include "Standard/branching.h"
#include "Standard/move.h"
#include "Standard/allocation.h"
#include "Standard/thread.h"
#include "Extension/ext_u8.h"
#include "Extension/ext_u16.h"
#include "Extension/ext_u32.h"
#include "Extension/ext_u64.h"

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
    std::string str{};
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
    loadOperationsSets(cpu.operations, type);
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

void Juse::loadOperationsSets(OperationMap& operations, const std::string types)
{
    static bool juseInitiated = false;
    if (!juseInitiated) {
        juseInitiated = true;
        Operations::Standard::Branching::init();
        Operations::Standard::Move::init();
        Operations::Standard::Allocation::init();
        Operations::Standard::Thread::init();
        Operations::ExtU8::init();
        Operations::ExtU16::init();
        Operations::ExtU32::init();
        Operations::ExtU64::init();
    }
    for (const auto& c : split(types, "/")) {
        if (c == "std") {
            Operations::Standard::Branching::add(operations);
            Operations::Standard::Move::add(operations);
            Operations::Standard::Allocation::add(operations);
            Operations::Standard::Thread::add(operations);
            continue;
        }
        if (c == "u8" || c == "std_ext") {
            Operations::ExtU8::add(operations);
        }
        if (c == "u16" || c == "std_ext") {
            Operations::ExtU16::add(operations);
        }
        if (c == "u32" || c == "std_ext") {
            Operations::ExtU32::add(operations);
        }
        if (c == "u64" || c == "std_ext") {
            Operations::ExtU64::add(operations);
        }
        if (c == "str") {
            //Operations::StandardExtensions::ext_string(operations);
        }
        if (c == "float") {
            //Operations::StandardExtensions::ext_float(operations);
        }
    }
}