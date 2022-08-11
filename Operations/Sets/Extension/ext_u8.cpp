
#include "ext_u8.h"

namespace Juse::Operations::ExtU8 {

void add(OperationMap& operations)
{
    // 10xx : U8 moves, stack operations, casts
    operations[0x1000] = Set8;
    operations[0x1001] = CopyFrom8;
    operations[0x1002] = CopyTo8;
    operations[0x1003] = Copy8;
    operations[0x1004] = Push8;
    operations[0x1005] = Pop8;

    // 1006 - 1007 : casts operations, not applicable to u8

    operations[0x1008] = Copy8If;

    // 11xx - U8 Operations
    operations[0x1100] = Add8;
    operations[0x1101] = Substract8;
    operations[0x1102] = Multiply8;
    operations[0x1103] = Divide8;
    operations[0x1104] = Modulo8;
    operations[0x1106] = Random8;
    operations[0x11F0] = Compare8;

    // 13xx - U8 I/O
    operations[0x1300] = Write8;
    operations[0x1301] = Read8;
    operations[0x1310] = WriteAscii;
}

S<Operation> Set8 = S<Operation>(new Operation(
    "Set Byte", "SET8", "Bytes[A] = B",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        Juse::Operations::Unsigned::set(cpu.registers.bytes, arguments);
    },
    { { SIZE8 }, { SIZE8 } }));

S<Operation> CopyFrom8 = S<Operation>(new Operation(
    "Copy Byte From", "COPYFROM8", "Bytes[A] = [B]",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        U8 register_index = U8(arguments[0].value);
        U16 address = U16(arguments[1].value);
        U8 value = U8(set2word(machine.readData(cpu, address, SIZE8)));
        cpu.registers.bytes[register_index] = value;
    },
    { { SIZE8 }, { SIZE16 } }));

S<Operation> CopyTo8 = S<Operation>(new Operation(
    "Copy Byte To", "COPYTO8", "[A] = Bytes[B]",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        U16 address = U16(arguments[0].value);
        U8 register_index = U8(arguments[1].value);
        machine.writeData(cpu, address, word2set(cpu.registers.bytes[register_index]));
    },
    { { SIZE16 }, { SIZE8 } }));

S<Operation> Copy8 = S<Operation>(new Operation(
    "Copy Byte", "COPY8", "Bytes[A] = Bytes[B]",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        Operations::Unsigned::copy(cpu.registers.bytes, arguments);
    },
    { { SIZE8 }, { SIZE8 } }));

S<Operation> Push8 = S<Operation>(new Operation(
    "Push Byte", "PUSH8", "push Bytes[A]",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        Operations::Unsigned::push(cpu, cpu.registers.bytes, arguments);
    },
    { { SIZE8 } }));

S<Operation> Pop8 = S<Operation>(new Operation(
    "Pop Byte", "POP8", "Bytes[A] = {pop}",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        Operations::Unsigned::pop(cpu, cpu.registers.bytes, arguments);
    },
    { { SIZE8 } }));

S<Operation> Copy8If = S<Operation>(new Operation(
    "Copy Byte If", "COPY8IF", "?A : Bytes[B]",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        CompareFlag flag = CompareFlag(U8(arguments[0].value));
        if (cpu.registers.compareFlags[flag]) {
            U8 register_index = U8(arguments[1].value);
            machine.writeData(cpu, cpu.offseted(), word2set(cpu.registers.bytes[register_index]));
        }
    },
    { { SIZE8 }, { SIZE8 } }));

S<Operation> Add8 = S<Operation>(new Operation(
    "Add", "ADD8", "Bytes[A] = Bytes[B] + Bytes[C] CR Bytes[D]",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        Operations::Unsigned::add<U8, U16>(cpu.registers.bytes, cpu.registers.compareFlags, arguments);
    },
    { { SIZE8 }, { SIZE8 }, { SIZE8 }, { SIZE8 } }));
S<Operation> Substract8 = S<Operation>(new Operation(
    "Substract", "SUB8", "Bytes[A] = Bytes[B] - Bytes[C]",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        Operations::Unsigned::substract<U8, U16>(cpu.registers.bytes, cpu.registers.compareFlags, arguments);
    },
    { { SIZE8 }, { SIZE8 }, { SIZE8 } }));
S<Operation> Multiply8 = S<Operation>(new Operation(
    "Multiply", "MUL8", "Bytes[A] = Bytes[B] * Bytes[C] CR Bytes[D]",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        Operations::Unsigned::multiply<U8, U16>(cpu.registers.bytes, cpu.registers.compareFlags, arguments);
    },
    { { SIZE8 }, { SIZE8 }, { SIZE8 }, { SIZE8 } }));
S<Operation> Divide8 = S<Operation>(new Operation(
    "Divide", "DIV8", "Bytes[A] = Bytes[B] / Bytes[C]",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        Operations::Unsigned::divide<U8, U16>(cpu.registers.bytes, cpu.registers.compareFlags, arguments);
    },
    { { SIZE8 }, { SIZE8 }, { SIZE8 } }));
S<Operation> Modulo8 = S<Operation>(new Operation(
    "Modulo", "MOD8", "Bytes[A] = Bytes[B] % Bytes[C]",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        Operations::Unsigned::modulo<U8, U16>(cpu.registers.bytes, cpu.registers.compareFlags, arguments);
    },
    { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

S<Operation> Random8 = S<Operation>(new Operation(
    "Random", "RND16", "Bytes[A] = {rnd Bytes[B] Bytes[C]}",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        U8 index = U8(arguments[0].value);
        U8 min = cpu.registers.bytes[U8(arguments[1].value)];
        U8 max = cpu.registers.bytes[U8(arguments[2].value)];
        cpu.registers.bytes[index] = U8(random<U16>(min, max));
    },
    { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

S<Operation> Compare8 = S<Operation>(new Operation(
    "Compare", "CMP8", "Bytes[A] ? Bytes[B]",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        Operations::Unsigned::compare(cpu.registers.bytes, cpu.registers.compareFlags, arguments);
    },
    { { SIZE8 }, { SIZE8 } }));

S<Operation> Write8 = S<Operation>(new Operation(
    "Write Byte", "WINT8", "out Bytes[A]",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        U8 register_index = U8(arguments[0].value);
        std::stringstream buffer {};
        buffer << std::dec << +cpu.registers.bytes[register_index];
        out(machine.out, buffer, cpu.flag_debug);
    },
    { { SIZE8 } }));

S<Operation> Read8 = S<Operation>(new Operation(
    "Read Byte", "RINT8", "in Bytes[A]",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        U16 value;
        std::stringstream buffer;
        U8 register_index = U8(arguments[0].value);
        buffer << in(machine.out, machine.in, cpu.flag_debug);
        buffer >> std::dec >> value;
        cpu.registers.bytes[register_index] = U8(value);
    },
    { { SIZE8 } }));

S<Operation> WriteAscii = S<Operation>(new Operation(
    "Write Ascii", "WASCII", "out S8 A",
    [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
        U16 address = U16(arguments[0].value);
        U16 offset = 0;
        std::stringstream buffer {};
        CH8 character {};
        do {
            character = CH8(U8(set2word(machine.readData(cpu, address + offset, SIZE8))));
            if (character != '\0') {
                buffer << character;
            }
            offset++;
        } while (character != '\0');
        out(machine.out, buffer, cpu.flag_debug);
    },
    { { SIZE16 } }));

}