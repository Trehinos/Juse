#pragma once

#include "types.h"

namespace Juse
{

	template <int length>
	struct word
	{
		std::array<I8, length> bytes;

		word operator+(word);
		word operator-(word);
		word operator/(word);
		word operator*(word);
	};

}
