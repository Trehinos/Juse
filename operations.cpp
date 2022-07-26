
#include "operations.h"

using namespace Juse;

void Juse::init(Cpu& cpu)
{
	cpu.initOperations();
	// Creation of all operations
	createControlOperations(cpu);
	createRegistersOperations(cpu);
	createMemoryOperations(cpu);
}