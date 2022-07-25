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
		static S<Operation> NoOp;
		OperationMap operations;

		Cpu();
		void initOperations();
		void forward();
		U64 addressPointer();
		void jump(U16 pool, U32 segment, U16 instruction);
		void longjump(U64 address);
		bool shouldExit();

		void cycle(Machine&);

		U8 dataAt(Memory& memory, U64 address);

		U8 data(Memory& memory);

		U16 pool();
		U16 segment();
		U16 instruction();
	};
}
