#include "Tests.h"

#include "../../operations/Standard/move.h"

void Juse::Tests::testOperations(Machine& m, Cpu& c)
{
    Operations::initSets();
    (*Operations::Standard::Move::CopyFromDataPool)(m, c, OperationArguments{});

}