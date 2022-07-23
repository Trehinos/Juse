#pragma once

#include <cstdint>
#include <array>
#include <map>
#include <vector>
#include <functional>
#include <string>
#include <stack>

namespace Juse
{

	const size_t SEGMENT_SIZE = 1 << 16; // 64 KiB

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
	using Pool = std::map<U32, Segment&>;
	using Memory = std::map<U16, Pool&>;

	using ByteSet = std::vector<U8>;
	using Stack = std::stack<U8>;

	class Operation;

	struct Instruction
	{
		ByteSet data;

		inline U16 identifier()
		{
			return (data[0] << 8) + (data[1]);
		}

		inline U64 argument(size_t offset, size_t size)
		{
			U64 buffer = 0;
			for (size_t i = SIZE16 + offset; i < SIZE16 + offset + size; i++) {
				buffer <<= 8;
				buffer += data[i];
			}
			return buffer;
		}
	};

	using OperationMap = std::map<U16, Operation&>;

	class Cpu;
	class Machine;

	using FunctionType = void(Machine&, Instruction&, Operation&);
	using OperationFunction = std::function<FunctionType>;

}

