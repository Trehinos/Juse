#pragma once

#include <iostream>
#include <utility>
#include <stdexcept>

#include "types.h"
#include "Cpu.h"

namespace Juse
{

	U64 set2word(ByteSet bytes);

	class Machine
	{

	public:
		std::ostream& out;
		std::istream& in;

		Cpu cpu;
		P<Memory> memory;
		Stack stack;

		/* Move operations location */
		U16 dataPool;
		U32 dataSegment;

		GeneralRegisters<U8> bytes;
		GeneralRegisters<U16> words;
		GeneralRegisters<U32> quads;
		GeneralRegisters<U64> longs;


		Machine(std::istream&, std::ostream&);
		static Machine loadFile(std::string);
		static Machine fromData(Segment&);

		void createPool(U16);
		void createSegment(U16, U32);

		S<Pool> getPool(U16);
		S<Segment> getSegment(U16, U32);

		void push(U8);
		U8 pop();

		ByteSet read(size_t);

		ByteSet readAt(U64, size_t);
		ByteSet readData(U16, size_t);

		S<Operation> getOperation(U16&);

		void run(bool = false);

	};
}
