#include "Machine.h"

Juse::Machine::Machine() :
	memory(),
	stack(),
	cpu()
{ 
	createSegment(0, 0);
}

Juse::Machine Juse::Machine::loadFile(std::string filename)
{
	
}

void Juse::Machine::run()
{
	while (!cpu.shouldExit()) {
		cpu.cycle(*this);
	}
}
