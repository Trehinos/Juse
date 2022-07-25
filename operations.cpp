
#include "operations.h"

void Juse::init(Juse::Cpu& cpu)
{
	cpu.initOperations();
	// Creation of all operations
	createControlOperations(cpu);
	createRegistersOperations(cpu);
}