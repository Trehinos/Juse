#pragma once

#include "types.h"

namespace Juse
{
	class Cpu
	{

		/*
		* Registers
		*/
		U16 pool_pointer;
		U32 segment_pointer;
		U16 instruction_pointer;

		bool flag_exit;

	public:

		/*
		* map<Operation> of this machine
		*/
		OperationMap operations;

		Cpu();
		void initOperations();
		void forward();
		inline U64 address() { return U64(pool_pointer) << 48 + U64(segment_pointer) << 16 + instruction_pointer; }
		inline void jump(U16 pool, U32 segment, U16 instruction)
		{
			pool_pointer = pool;
			segment_pointer = segment;
			instruction_pointer = instruction;
		}
		inline void longjump(U64 address)
		{
			pool_pointer = address & 0xFFFF000000000000;
			segment_pointer = address & 0x0000FFFFFFFF0000;
			instruction_pointer = address & 0x000000000000FFFF;
		}
		inline bool shouldExit() { return flag_exit; }

		void cycle(Machine&);

		inline U8 data(Memory& memory) { return memory[pool_pointer][segment_pointer][instruction_pointer]; }

		inline U16 pool() { return pool_pointer; }
		inline U16 segment() { return segment_pointer; }
		inline U16 instruction() { return instruction_pointer; }
	};
}
