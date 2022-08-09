#pragma once

#include "../Models/types.h"

namespace Juse {
namespace Utility {
    // trim from end of string (right)
    inline std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v")
    {
        s.erase(s.find_last_not_of(t) + 1);
        return s;
    }

    // trim from beginning of string (left)
    inline std::string& ltrim(std::string& s, const char* t = " \t\n\r\f\v")
    {
        s.erase(0, s.find_first_not_of(t));
        return s;
    }

    // trim from both ends of string (right then left)
    inline std::string& trim(std::string& s, const char* t = " \t\n\r\f\v")
    {
        return ltrim(rtrim(s, t), t);
    }

    struct Memory {
        static void forward(U16&, U32&, U16&, size_t);
        static void write(Juse::Memory&, ByteSet, Address);
        static S<Pool> pool(Juse::Memory&, U16);
        static S<Segment> segment(Juse::Pool&, U32);
    };
}
}
