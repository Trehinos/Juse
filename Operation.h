#pragma once

#include "types.h"

namespace Juse
{

	class Operation
	{
		struct Argument
		{
			size_t size = 0;
		};

		std::vector<Argument> arguments;
		OperationFunction code;

	public:
		Operation(OperationFunction, std::vector<Argument>);
		U64 argument(Instruction, size_t = 0);
		inline size_t length()
		{
			size_t size = 2;
			for (Argument argument : arguments) {
				size += argument.size;
			}
			return size;
		}
		void operator()(Machine&, Instruction);
	};

}