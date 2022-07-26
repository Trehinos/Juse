#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <stack>
#include <string>
#include <vector>

namespace Juse {

const size_t SEGMENT_SIZE = 1 << 16; // 64 KiB

template <class T> using P = std::unique_ptr<T>;

template <class T> using S = std::shared_ptr<T>;

template <class T> const auto makeP = std::make_unique<T>;

template <class T> const auto makeS = std::make_shared<T>;

const size_t SIZE8 = 1;
const size_t SIZE16 = 2;
const size_t SIZE32 = 4;
const size_t SIZE64 = 8;

using U8 = std::uint8_t;
using U16 = std::uint16_t;
using U32 = std::uint32_t;
using U64 = std::uint64_t;

const U8 MASK_BOTTOM8 = 0xFF;
const U16 MASK_16TOP8 = 0xFF00;
const U16 MASK_BOTTOM16 = 0xFFFF;
const U32 MASK_32TOP16 = 0xFFFF0000;
const U32 MASK_BOTTOM32 = 0xFFFFFFFF;
const U64 MASK_64TOP32 = 0xFFFFFFFF00000000;

using I8 = std::int8_t;
using I16 = std::int16_t;
using I32 = std::int32_t;
using I64 = std::int64_t;

using Segment = std::array<U8, SEGMENT_SIZE>;
using Pool = std::map<U32, S<Segment>>;
using Memory = std::map<U16, S<Pool>>;

using ByteSet = std::vector<U8>;
using Stack = std::stack<U8>;

template <class Type> using GeneralRegisters = std::array<Type, 256>;

class Operation;

using OperationMap = std::map<U16, S<Operation>>;

struct Address {
  U16 pool;
  U32 segment;
  U16 datum;

  U64 compose() { return (U64(pool) << 48) + (U64(segment) << 16) + datum; }

  static U64 with(U16 p, U32 s, U16 d) { return Address{p, s, d}.compose(); }

  static Address from(U64 address) {
    U16 pool = U16(address & 0xFFFF000000000000);
    U32 segment = U32(address & 0x0000FFFFFFFF0000);
    U16 datum = U16(address & 0x000000000000FFFF);

    return Address{pool, segment, datum};
  }
};

struct Instruction {
  ByteSet data;

  U16 identifier() { return U16(data[0] << 8) + (data[1]); }

  U64 argument(size_t offset, size_t size) {
    U64 buffer = 0;
    for (size_t i = SIZE16 + offset; i < SIZE16 + offset + size; i++) {
      buffer <<= 8;
      buffer += data[i];
    }
    return buffer;
  }
};

class Cpu;
class Machine;

using FunctionType = void(Machine &, Instruction &, Operation &);
using OperationFunction = std::function<FunctionType>;

} // namespace Juse