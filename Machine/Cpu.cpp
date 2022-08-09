
#include <functional>
#include <iomanip>

#include "Cpu.h"
#include "Machine.h"
#include "Operation.h"

using namespace Juse;

void Juse::debugInstruction(Machine& machine, Cpu& cpu, Operation& operation, Instruction& instruction)
{
    if (cpu.flag_debug) {
        for (U8 byte : instruction.data) {
            machine.out << std::setw(2) << std::hex << (int)(unsigned char)byte;
        }
        machine.out << " : " << operation.getName() << " ";

        size_t index = 0;
        for (Argument argument : operation.getArgumentDefs()) {
            machine.out << std::left << std::setfill('0') << std::setw(argument.size) << std::right << std::hex << operation.argument(instruction, index++) << " ";
        }
    }
}

Instruction Juse::getInstructionFromId(Machine& machine, Cpu& cpu, Operation& operation, U16 identifier)
{
    Instruction instruction = { ByteSet { U8((MASK_16TOP8 & identifier) >> 8), U8(MASK_BOTTOM8 & identifier) } };
    ByteSet toAdd = machine.read(cpu, operation.length() - 2);
    for (U8 add : toAdd) {
        instruction.data.push_back(add);
    }

    debugInstruction(machine, cpu, operation, instruction);

    return instruction;
}

Duration Cpu::duration(U32 frequency)
{
    I64 duration = 1000000000 / frequency;
    return Duration { duration };
}

bool Cpu::tick(U32 frequency, TimePoint time, TimePoint last)
{
    Duration d = time - last;
    return d >= duration(frequency);
}

Cpu::Cpu()
    : registers {}
    , pool_pointer(0)
    , segment_pointer(0)
    , instruction_pointer(0)
    , stack()
    , data_pool { 0 }
    , data_segment { 0 }
    , address_pointer { 0 }
    , address_offset { 0 }
    , address_increment { 0 }
    , flag_exit(false)
    , flag_debug(false)
    , flag_skip(false)
    , config_frequency(BASE_FREQUENCY)
{
    registers.compareFlags[CompareFlag::EQ] = false;
    registers.compareFlags[CompareFlag::GT] = false;
    registers.compareFlags[CompareFlag::LW] = false;
    registers.compareFlags[CompareFlag::GE] = false;
    registers.compareFlags[CompareFlag::LE] = false;
    registers.compareFlags[CompareFlag::NE] = false;
    registers.compareFlags[CompareFlag::Z0] = false;
    registers.compareFlags[CompareFlag::CR] = false;
    registers.compareFlags[CompareFlag::OF] = false;
    registers.compareFlags[CompareFlag::ERR] = false;
}

void Cpu::forward(Memory& memory)
{
    if (instruction_pointer == UINT16_MAX) {
        instruction_pointer = 0;
        Pool p = *memory[pool()];
        if (segment_pointer >= p.size()) {
            segment_pointer = 0;
            if (pool_pointer >= memory.size())
                pool_pointer = 0;
            else
                pool_pointer++;
        } else
            segment_pointer++;
    } else
        instruction_pointer++;
}

U64 Cpu::instructionPointer()
{
    return Address::with(pool_pointer, segment_pointer, instruction_pointer);
}

U64 Cpu::addressPointer()
{
    return Address::with(data_pool, data_segment, offseted());
}

U16 Cpu::offseted()
{
    return address_pointer + address_offset;
}

void Cpu::jump(U16 pool, U32 segment, U16 instruction)
{
    pool_pointer = pool;
    segment_pointer = segment;
    instruction_pointer = instruction;
}
void Cpu::longjump(U64 address)
{
    Address a = Address::from(address);
    pool_pointer = a.pool;
    segment_pointer = a.segment;
    instruction_pointer = a.datum;
}

bool Cpu::shouldExit() { return flag_exit; }

void Cpu::cycle(Machine& machine, bool debug)
{
    static TimePoint time, last = Clock::now();
    time = Clock::now();

    if (tick(config_frequency, time, last)) {
        flag_debug = debug;

        U16 identifier = 0;
        S<Operation> current = getOperation(machine, identifier);
        if (current == NoOp) {
            return;
        }

        if (debug) {
            machine.out << "@" << std::hex << std::right << std::setfill('0') << std::setw(4);
            machine.out << (int)(instruction_pointer - 2) << " : ";
        }

        Instruction instruction = getInstructionFromId(machine, *this, *current, identifier);

        if (flag_skip) {
            if (debug) {
                machine.out << " <- SKIP" << std::endl;
            }
            flag_skip = false;
            return;
        }
        if (debug) {
            machine.out << std::endl;
        }
        (*current)(machine, *this, instruction);
        TimePoint next = last;
        Duration ellapsed = last - Clock::now();
        next += duration(config_frequency) - ellapsed;
        last = time;
        std::this_thread::sleep_until(next);
    }
}

void Cpu::run(Machine& machine, bool debug)
{
    while (!shouldExit()) {
        cycle(machine, debug);
    }
}

std::thread Cpu::start(Machine& machine, bool debug)
{
    std::thread thr(&Cpu::run, *this, std::ref(machine), debug);
    return thr;
}

void Cpu::push(U8 byte) { stack.push(byte); }

U8 Cpu::pop()
{
    U8 byte = stack.top();
    stack.pop();
    return byte;
}

void Cpu::multiPush(ByteSet set)
{
    for (U8 byte : set) {
        push(byte);
    }
}

ByteSet Cpu::multiPop(size_t nb_bytes)
{
    ByteSet bytes {};
    for (size_t i = 0; i < nb_bytes; i++) {
        bytes.insert(bytes.begin(), pop());
    }
    return bytes;
}

U8 Cpu::dataAt(Memory& memory, U64 address)
{
    Address a = Address::from(address);
    return (*(*memory[a.pool])[a.segment])[a.datum];
}

U8 Cpu::data(Memory& memory) { return dataAt(memory, instructionPointer()); }

void Juse::Cpu::set(Memory& memory, U64 address, U8 datum)
{
    Address a = Address::from(address);
    (*(*memory[a.pool])[a.segment])[a.datum] = datum;
}

U16 Cpu::pool() { return pool_pointer; }

U16 Cpu::segment() { return segment_pointer; }

U16 Cpu::instruction() { return instruction_pointer; }

S<Operation> Cpu::getOperation(Machine& machine, U16& id)
{
    ByteSet identifier = machine.read(*this, 2);
    id = U16(set2word(identifier));

    if (!operations.contains(id)) {
        return Cpu::NoOp;
    }

    return operations[id];
}

S<Operation> Cpu::NoOp = S<Operation>(new Operation(
    "Nothing", "NOP", "",
    [](Machine&, Cpu&, OperationArguments) {},
    {}));

void Cpu::initOperations() { operations[0x0000] = NoOp; }