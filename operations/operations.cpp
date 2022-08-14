#include <iomanip>
#include <iostream>
#include <ranges>
#include <string_view>

#include "../Juse/utility.h"

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

Juse::S8 Juse::in(std::ostream& os, std::istream& is, bool debug)
{
    S8 str{};
    if (debug) {
        os << "  >> ";
    }
    std::getline(is >> std::ws, str);
    is.clear();
    return str;
}

void Juse::Operations::initCpu(Cpu& cpu, const S8 type, U32 frequency)
{
    cpu.initOperations();
    loadSets(cpu.operations, type);
    cpu.config_frequency = frequency;
}

void Juse::Operations::initSets()
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
}

void Juse::Operations::loadSets(OperationMap& operations, const S8 types)
{
    initSets();
    for (const auto& c : Utility::Strings::split(types, "/")) {
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