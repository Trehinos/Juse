#pragma once

#include <stdexcept>
#include <tuple>

#include "types.h"
#include "operations.h"
#include "utility.h"

namespace Juse
{

    struct ExecutableHeader
    {
        enum class FlagId
        { // TODO : find usage ^^'
            FLAG0 = 1,
            FLAG1 = 2,
            FLAG2 = 4,
            FLAG3 = 8,
            FLAG4 = 0x10,
            FLAG5 = 0x20,
            FLAG6 = 0x40,
            FLAG7 = 0x80,
        };

        static U64 checksum(ByteSet set) {
            static const ByteSet SEED { 0x19,0xa8,0x77,0x9f, 0x39,0x44,0xde,0xf0 };
            ByteSet buffer { 0,0,0,0, 0,0,0,0 };
            size_t i = 0;
            for (auto& byte : set) {
                buffer.at(i) ^= (i > 1)
                    ? SEED.at(i)
                    : SEED.at(i) ^ buffer.at(i - 1);
                buffer.at(i) ^= byte;
                if (++i > 8) i = 0;
            }
            return set2word(buffer);
        }

        U64 start_address;
        struct JuseMeta
        {
            CH8 j;
            CH8 u;
            CH8 s;
            CH8 e;
            U16 version;
        } juse_meta;
        struct ProgramMeta
        {
            ByteArray<16> guid;
            U16 version;
            U64 build_dt;
        } program_meta;
        ByteArray<203> nullBytes;
        U64 check_sum;
        U32 segments_count;
        U8 special_flags;

        ExecutableHeader()
            : start_address {}, juse_meta { 'j', 'u', 's', 'e' , {} },
            program_meta {}, check_sum {}, segments_count {}, nullBytes {},
            special_flags {} { }
        ExecutableHeader(
            U64 start,
            U16 juseVersion,
            ProgramMeta programMeta,
            U64 checkSum,
            U8 specialFlags,
            Memory& memory
        ) : start_address(start), juse_meta(JuseMeta { 'j','u','s','e', juseVersion }),
            program_meta(programMeta), check_sum {}, segments_count {}, nullBytes {},
            special_flags(specialFlags) {
            segments_count = memory.at(0)->size();
            using Utility::MachineMemory;
            U64 sum = 0;
            for (auto&
                pool : memory) {
                for (auto& segment : *pool.second) {
                    sum ^= checksum(MachineMemory::read(
                        share(memory),
                        Address { pool.first, segment.first, 0 },
                        SEGMENT_SIZE
                    ));
                }
            }
        }

    };

    template <typename T>
    concept IsFlagId = IsAnyOf<T, ExecutableHeader::FlagId>;

    template <IsFlagId... FlagId>
    bool areFlagsOn(ExecutableHeader header, FlagId... ids) {
        Vector<FlagId> v_ids { ids... };
        for (ExecutableHeader::FlagId id : v_ids) {
        }
        return (header.special_flags << U8(id));
    }

    template <IsFlagId... FlagId>
    bool areFlagsOn<ExecutableHeader::FlagId>(ExecutableHeader,
        FlagId... ids);

    class Program
    {
        SPtr<Memory> memory;
        Program(Memory& mem) { memory = share(mem); }
    };

    class Executable
    {
        ExecutableHeader header;
        Executable(Program&) { };
    };

    void test_static(ExecutableHeader header) {
        bool flags0and1 = areFlagsOn(
            ExecutableHeader {},
            ExecutableHeader::FlagId::FLAG0,
            ExecutableHeader::FlagId::FLAG1
        );
    }

} // namespace Juse