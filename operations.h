#pragma once

#include "Operation.h"

namespace Juse
{
	namespace Operations
	{
		Operation NoOp, ShortJump, SegmentJump, LongJump;
		Operation Call, LongCall, Return, LongReturn;
	}

}
