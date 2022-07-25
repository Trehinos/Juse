#pragma once

#include "Machine.h"
#include "Operation.h"

namespace Juse
{
	void createControlOperations(Cpu&);
	void createRegistersOperations(Cpu&);

	/* 
	* Create operations
	*/
	void init(Cpu& cpu);

}
