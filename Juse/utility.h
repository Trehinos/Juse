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

        inline Vector<S8> split(S8 s, S8 delimiter)
        {
            size_t pos_start = 0, pos_end, delim_len = delimiter.length();
            S8 token;
            Vector<S8> res;

            while ((pos_end = s.find(delimiter, pos_start)) != S8::npos) {
                token = s.substr(pos_start, pos_end - pos_start);
                pos_start = pos_end + delim_len;
                res.push_back(token);
            }

            res.push_back(s.substr(pos_start));
            return res;
        }
    }

    struct MachineMemory {
        static void forward(U16&, U32&, U16&, U16=1);
        static void forward(Address&, U16=1);
        static SPtr<U8> memoryPtr(SPtr<Memory>, Address);
        static void write(SPtr<Memory>, ByteSet, Address);
        static ByteSet read(SPtr<Memory>, Address, U16);
        static SPtr<Pool> pool(SPtr<Memory>, U16);
        static SPtr<Segment> segment(SPtr<Pool>, U32);
        static void copyStreamInMemory(Machine& machine, std::istream& file);
    };
}
}
