#pragma once

#include <stdexcept>
#include <tuple>

#include "types.h"

namespace Juse {

struct ExecutableHeader {
  enum class FlagId {
    FLAG0 = 1,
    FLAG1 = 2,
    FLAG2 = 4,
    FLAG3 = 8,
    FLAG4 = 16,
    FLAG5 = 32,
    FLAG6 = 64,
    FLAG7 = 128,
  };

  U64 start_address;
  struct {
    CH8 j;
    CH8 u;
    CH8 s;
    CH8 e;
    U16 version;
  } juse_meta;
  struct {
    ByteArray<16> guid;
    U16 version;
    U64 build_dt;
  } program_meta;
  U64 crc;
  U32 segments_count;
  U8 special_flags;

  ExecutableHeader()
      : start_address{}, juse_meta{'j', 'u', 's', 'e'}, program_meta{}, crc{} {}
};

template <typename T>
concept IsFlagId = IsAnyOf<T, ExecutableHeader::FlagId>;

template <IsFlagId... FlagId>
bool specialFlagValue(ExecutableHeader header, FlagId... ids) {
  Vector<FlagId> v_ids{ids...};
  for (ExecutableHeader::FlagId id : v_ids) {
  }
  return (header.special_flags << U8(id));
}

template <IsFlagId... FlagId>
bool specialFlagValue<ExecutableHeader::FlagId>(ExecutableHeader,
                                                FlagId... ids);

class Program {
  SPtr<Memory> memory;
  Program(Memory &mem) { memory = share(mem); }
};

class Executable {
  ExecutableHeader header;
  Executable(Program &){

  };
};

void test_static(ExecutableHeader header) {
  U8 id = 1;
  bool result = specialFlagValue(header, ExecutableHeader::FlagId::FLAG0);
}

} // namespace Juse