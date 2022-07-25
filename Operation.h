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

		std::string mnemu;
		std::string ecal;
		std::vector<Argument> arguments;
		OperationFunction code;

	public:
		Operation(std::string = {}, std::string = {}, OperationFunction = {}, std::vector<Argument> = {});
		std::string mnemuCode() { return mnemu; }
		U64 argument(Instruction, size_t = 0);
		size_t length();
		void operator()(Machine&, Instruction);
	};

}