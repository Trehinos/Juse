#pragma once

#include "types.h"

namespace Juse {
namespace Utility {
    // trim from end of string (right)
    namespace Strings {
        inline S8& rtrim(S8& s, const char* t = " \t\n\r\f\v")
        {
            s.erase(s.find_last_not_of(t) + 1);
            return s;
        }

        // trim from beginning of string (left)
        inline S8& ltrim(S8& s, const char* t = " \t\n\r\f\v")
        {
            s.erase(0, s.find_first_not_of(t));
            return s;
        }

        // trim from both ends of string (right then left)
        inline S8& trim(S8& s, const char* t = " \t\n\r\f\v")
        {
            return ltrim(rtrim(s, t), t);
        }
    }

    struct MachineMemory {
        static void forward(U16&, U32&, U16&, size_t);
        static void write(Memory&, ByteSet, Address);
        static SPtr<Pool> pool(Memory&, U16);
        static SPtr<Segment> segment(Pool&, U32);
    };
}
}
