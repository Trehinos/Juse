#pragma once

#include "types.h"

namespace Juse {

enum class CompareFlag {
  EQ = 0,
  GT = 1,
  LW = 2,
  GE = 3,
  LE = 4,
  NE = 5,
  Z0 = 6,
  CR = 7,

  ERR = 255
};

class Cpu {

  /*
   * Registers
   */
  U16 pool_pointer;
  U32 segment_pointer;
  U16 instruction_pointer;

public:
  std::map<CompareFlag, bool> compareFlags;
  bool flag_exit;
  bool flag_debug;
  bool flag_skip;

  static S<Operation> NoOp;
  OperationMap operations;

  Cpu();
  void initOperations();
  void forward();
  U64 addressPointer();
  void jump(U16, U32, U16);
  void longjump(U64);
  bool shouldExit();

  void cycle(Machine &, bool = false);

  U8 dataAt(Memory &, U64);
  U8 data(Memory &);

  void set(Memory &, U64, U8);

  U16 pool();
  U16 segment();
  U16 instruction();
};
} // namespace Juse
