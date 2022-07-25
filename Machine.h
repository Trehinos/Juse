#pragma once

#include "types.h"
#include "Cpu.h"

#include <cstdint>
#include <utility>
#include <stack>
#include <stdexcept>

namespace Juse
{

	U64 set2word(ByteSet bytes);

	class Machine
	{

	public:
		Cpu cpu;
		P<Memory> memory;
		Stack stack;

		/* Move operations location */
		U16 dataPool;
		U32 dataSegment;

		GeneralRegisters<U8> bytes;
		GeneralRegisters<U16> words;
		GeneralRegisters<U32> quads;
		GeneralRegisters<U64> octs;


		Machine();
		static Machine loadFile(std::string);

		void createPool(U16);
		void createSegment(U16, U32);

		void push(U8);
		U8 pop();

		ByteSet read(size_t);

		ByteSet readAt(U64, size_t);

		S<Operation> getOperation(U16&);

		void run();

	};
}
