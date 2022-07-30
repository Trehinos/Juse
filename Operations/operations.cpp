
#include "operations.h"

std::ostream& Juse::operator<<(std::ostream& os, const Juse::U8 c)
{
    os << (int)(unsigned char)c;
    return os;
}

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

void Juse::init(Juse::Cpu& cpu)
{
    cpu.initOperations();

    Operations::Standard::branching(cpu);
    Operations::Standard::move(cpu);
    Operations::Standard::logic(cpu);
    Operations::Standard::allocation(cpu);
    Operations::Standard::thread(cpu);

    Operations::StandardExtensions::ext_u8(cpu);
    Operations::StandardExtensions::ext_u16(cpu);
    Operations::StandardExtensions::ext_u32(cpu);
    Operations::StandardExtensions::ext_u64(cpu);
    /*
    Operations::StandardExtensions::ext_string(cpu);
    Operations::StandardExtensions::ext_float(cpu);
    */
}