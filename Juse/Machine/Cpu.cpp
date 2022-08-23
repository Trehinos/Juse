
#include <functional>
#include <iomanip>

#include "Machine.h"
#include "Operation.h"

using namespace Juse;

const Array<U8, 14> Registers::SCALAR_ARGS = Utility::Arrays::range<U8, 14>(0x02);
const Array<U8, 15> Registers::FUNC_ARGS = Utility::Arrays::range<U8, 15>(0x10);
const Array<U8, 16> Registers::VEC_RESULTS = Utility::Arrays::range<U8, 16>(0x20);
const Array<U8, 64> Registers::VEC_ARGS = Utility::Arrays::range<U8, 64>(0x30);

void Juse::debugInstruction(Machine& machine, Cpu* cpu, Operation* operation, Instruction& instruction) {
    if (cpu->flag_debug) {
        for (U8 byte : instruction.data) {
            machine.out << std::setw(2) << std::hex << (int) (unsigned char) byte;
        }
        machine.out << " : " << operation->getName() << " ";

        size_t index = 0;
        for (Argument argument : operation->getArgumentDefs()) {
            machine.out << std::setfill('0') << std::setw(argument.size) << std::right
                << std::hex << operation->argument(instruction, index++) << " ";
        }
    }
}

Instruction Juse::getInstructionFromId(Machine& machine, Cpu* cpu, Operation* operation, OperationId identifier) {
    Instruction instruction = { ByteSet { U8((MASK_16TOP8 & identifier) >> 8), U8(MASK_BOTTOM8 & identifier) } };
    ByteSet toAdd = machine.readAndForward(cpu, U16(operation->length() - 2));
    for (U8 add : toAdd) {
        instruction.data.push_back(add);
    }

    debugInstruction(machine, cpu, operation, instruction);

    return instruction;
}

Duration Cpu::duration(U32 frequency) {
    I64 duration = 1000000000 / frequency;
    return Duration { duration };
}

bool Cpu::tick(U32 frequency, TimePoint time, TimePoint last) {
    Duration d = time - last;
    return d >= duration(frequency);
}

Cpu::Cpu()
    : registers {}, code {}, data_addr {}
    , stack()
    , address_offset { 0 }
    , address_increment { 0 }
    , flag_exit(false)
    , flag_debug(false)
    , flag_skip(false)
    , config_frequency(BASE_FREQUENCY) {
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

void Cpu::forward(size_t s) {
    code += s;
}

U64 Cpu::instructionPointer() {
    return code.compose();
}

U64 Cpu::addressPointer() {
    return Address::with(data_addr.pool, data_addr.segment, offseted());
}

DataId Cpu::offseted() {
    return data_addr.address + address_offset;
}

void Cpu::jump(PoolId pool, SegmentId segment, DataId instruction) {
    code = Address(pool, segment, instruction);
}
void Cpu::longjump(U64 address) {
    Address a = Address::from(address);
    jump(a.pool, a.segment, a.address);
}

bool Cpu::shouldExit() { return flag_exit; }

void Cpu::cycle(Machine& machine, bool debug) {
    static TimePoint time, last = Clock::now();
    time = Clock::now();

    if (tick(config_frequency, time, last)) {
        flag_debug = debug;

        OperationId identifier = 0;
        SPtr<Operation> current = getOperation(machine, identifier);

        if (current == NoOp || identifier == 0) {
            return;
        }

        if (debug) {
            machine.out << "@" << std::hex << std::right << std::setfill('0') << std::setw(4);
            machine.out << (int) (code.address - 2) << " : ";
        }

        Instruction instruction = getInstructionFromId(machine, this, current.get(), identifier);

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

void Cpu::run(Machine& machine, bool debug) {
    while (!shouldExit()) {
        cycle(machine, debug);
    }
}

std::thread Cpu::start(Machine& machine, bool debug) {
    std::thread thr(&Cpu::run, *this, std::ref(machine), debug);
    return thr;
}

void Cpu::push(U8 byte) { stack.push(byte); }

U8 Cpu::pop() {
    U8 byte = stack.top();
    stack.pop();
    return byte;
}

void Cpu::multiPush(ByteSet set) {
    for (U8 byte : set) {
        push(byte);
    }
}

ByteSet Cpu::multiPop(size_t nb_bytes) {
    ByteSet bytes {};
    for (size_t i = 0; i < nb_bytes; i++) {
        bytes.insert(bytes.begin(), pop());
    }
    return bytes;
}

U8 Cpu::dataAt(Machine& m, U64 address) {
    return m.data(address);
}

U8 Cpu::data(Machine& m) { return m.data(this->instructionPointer()); }

void Juse::Cpu::set(Machine& m, U64 address, U8 datum) {
    return m.writeAt(address, word2set(datum));
}

PoolId Cpu::pool() { return code.pool; }

SegmentId Cpu::segment() { return code.segment; }

DataId Cpu::instruction() { return code.address; }

SPtr<Operation> Cpu::getOperation(Machine& machine, OperationId& id) {
    ByteSet identifier = machine.readAndForward(this, 2);
    id = OperationId(set2word(identifier));

    return findOperation(machine, id);
}

SPtr<Operation> Cpu::findOperation(Machine& machine, OperationId id) {
    if (!operations.contains(id)) {
        id = 0;
        return NoOp;
    }

    return operations.at(id);
}

SPtr<Operation> Cpu::NoOp = SPtr<Operation>(new Operation(
    "Nothing", "NOP", "",
    [] (Machine&, Cpu&, OperationArguments) { },
    {}));

void Cpu::initOperations() { operations[0x0000] = NoOp; }