#include "Machine.h"

Juse::U64 Juse::set2word(Juse::ByteSet bytes)
{
	U64 word = 0;
	for (U8 byte : bytes) {
		word <<= 8;
		word += byte;
	}
	return word;
}

Juse::Machine::Machine() :
	memory{},
	stack(),
	cpu(),

	dataPool{0},
	dataSegment{0},

	bytes(),
	words(),
	quads(),
	longs()
{
	memory = makeP<Memory>();
	createSegment(0, 0);
}

Juse::Machine Juse::Machine::loadFile(std::string filename)
{
	// TODO
	return Machine();
}

void Juse::Machine::createPool(U16 pool_index)
{
	memory->insert({pool_index, Juse::makeS<Pool>()});
}

void Juse::Machine::createSegment(U16 pool_index, U32 segment_index)
{
	if (!memory->contains(pool_index)) {
		createPool(pool_index);
	}
	((*memory)[pool_index])->insert({segment_index, Juse::makeS<Segment>()});
}

void Juse::Machine::push(U8 byte)
{
	stack.push(byte);
}

Juse::U8 Juse::Machine::pop()
{
	U8 byte = stack.top(); stack.pop(); return byte;
}

Juse::ByteSet Juse::Machine::read(size_t nb_bytes)
{
	ByteSet bytes{};
	for (size_t i = 0; i < nb_bytes; i++) {
		bytes.push_back(cpu.data(*memory));
		cpu.forward();
	}
	return bytes;
}

Juse::ByteSet Juse::Machine::readAt(U64 address, size_t nb_bytes)
{
	ByteSet bytes{};
	for (U64 i = address; i < address + nb_bytes; i++) {
		bytes.push_back(cpu.dataAt(*memory, i));
	}
	return bytes;
}

Juse::ByteSet Juse::Machine::readData(U16 datum, size_t nb_bytes)
{
	return readAt(Address::with(dataPool, dataSegment, datum), nb_bytes);
}

Juse::S<Juse::Operation> Juse::Machine::getOperation(U16& id)
{
	ByteSet identifier = read(2);

	try {
		return cpu.operations.at(id = U16(set2word(identifier)));
	} catch (std::out_of_range) {
		return Cpu::NoOp;
	}
}

void Juse::Machine::run()
{
	while (!cpu.shouldExit()) {
		cpu.cycle(*this);
	}
}
