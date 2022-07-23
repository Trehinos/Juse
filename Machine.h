#pragma once

#include "types.h"
#include "Cpu.h"
#include "operations.h"

#include <cstdint>
#include <utility>
#include <stack>
#include <stdexcept>

namespace Juse
{

	U64 set2word(ByteSet bytes)
	{
		U64 word = 0;
		for (U8 byte : bytes) {
			word <<= 8;
			word += byte;
		}
		return word;
	}

	class Machine
	{

	public:
		Cpu cpu;
		Memory memory;
		Stack stack;

		Machine();
		static Machine loadFile(std::string);

		void createSegment(U16 pool_index, U32 segment_index)
		{
			memory[pool_index][segment_index] = Segment();
		}

		inline void push(U8 byte) { stack.push(byte); }
		inline U8 pop() { U8 byte = stack.top(); stack.pop(); return byte; }

		inline ByteSet read(size_t nb_bytes)
		{
			ByteSet bytes{};
			for (size_t i = 0; i < nb_bytes; i++) {
				bytes.push_back(cpu.data(memory));
				cpu.forward();
			}
			return bytes;
		}
		
		inline Operation getOperation(U16& id)
		{
			ByteSet identifier = read(2);

			try {
				return cpu.operations.at(id = set2word(identifier));
			} catch (std::out_of_range) {
				return Operations::NoOp;
			}
		}

		void run();

	};
}
