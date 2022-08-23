
#include "ext_u8.h"

namespace Juse::Operations::ExtU8
{

    void add(OperationMap& operations) {
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

    void init() {
        Set8 = Unsigned::set<U8>("Byte");

        CopyFrom8 = SPtr<Operation>(new Operation(
            "Copy Byte From", "COPYFROM8", "Bytes[A] = [B]",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            U16 address = U16(arguments[1].value);
            U8 value = U8(set2word(machine.readData(cpu, address, SIZE8)));
            cpu.registers.bytes[register_index] = value;
        }, { { SIZE8 }, { SIZE16 } }));

        CopyTo8 = Unsigned::copyTo<U8>("Byte");

        Copy8 = SPtr<Operation>(new Operation(
            "Copy Byte", "COPY8", "Bytes[A] = Bytes[B]",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::copy(cpu.registers.bytes, arguments);
        }, { { SIZE8 }, { SIZE8 } }));

        Push8 = SPtr<Operation>(new Operation(
            "Push Byte", "PUSH8", "push Bytes[A]",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::push(cpu, cpu.registers.bytes, arguments);
        }, { { SIZE8 } }));

        Pop8 = SPtr<Operation>(new Operation(
            "Pop Byte", "POP8", "Bytes[A] = {pop}",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::pop(cpu, cpu.registers.bytes, arguments);
        }, { { SIZE8 } }));

        Copy8If = SPtr<Operation>(new Operation(
            "Copy Byte If", "COPY8IF", "?A : Bytes[B]",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            CompareFlag flag = CompareFlag(U8(arguments[0].value));
            if (cpu.registers.compareFlags[flag]) {
                U8 register_index = U8(arguments[1].value);
                machine.writeData(cpu, cpu.offseted(), word2set(cpu.registers.bytes[register_index]));
            }
        }, { { SIZE8 }, { SIZE8 } }));

        Add8 = SPtr<Operation>(new Operation(
            "Add", "ADD8", "Bytes[A] = Bytes[B] + Bytes[C] CR Bytes[D]",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::add<U8, U16>(cpu.registers.bytes, cpu.registers.compareFlags, arguments);
        }, { { SIZE8 }, { SIZE8 }, { SIZE8 }, { SIZE8 } }));

        Substract8 = SPtr<Operation>(new Operation(
            "Substract", "SUB8", "Bytes[A] = Bytes[B] - Bytes[C]",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::substract<U8, U16>(cpu.registers.bytes, cpu.registers.compareFlags, arguments);
        }, { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

        Multiply8 = SPtr<Operation>(new Operation(
            "Multiply", "MUL8", "Bytes[A] = Bytes[B] * Bytes[C] CR Bytes[D]",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::multiply<U8, U16>(cpu.registers.bytes, cpu.registers.compareFlags, arguments);
        }, { { SIZE8 }, { SIZE8 }, { SIZE8 }, { SIZE8 } }));

        Divide8 = SPtr<Operation>(new Operation(
            "Divide", "DIV8", "Bytes[A] = Bytes[B] / Bytes[C]",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::divide<U8, U16>(cpu.registers.bytes, cpu.registers.compareFlags, arguments);
        }, { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

        Modulo8 = SPtr<Operation>(new Operation(
            "Modulo", "MOD8", "Bytes[A] = Bytes[B] % Bytes[C]",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::modulo<U8, U16>(cpu.registers.bytes, cpu.registers.compareFlags, arguments);
        }, { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

        Random8 = SPtr<Operation>(new Operation(
            "Random", "RND16", "Bytes[A] = {rnd Bytes[B] Bytes[C]}",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 index = U8(arguments[0].value);
            U8 min = cpu.registers.bytes[U8(arguments[1].value)];
            U8 max = cpu.registers.bytes[U8(arguments[2].value)];
            cpu.registers.bytes[index] = U8(random<U16>(min, max));
        }, { { SIZE8 }, { SIZE8 }, { SIZE8 } }));

        Compare8 = SPtr<Operation>(new Operation(
            "Compare", "CMP8", "Bytes[A] ? Bytes[B]",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            Operations::Unsigned::compare(cpu.registers.bytes, cpu.registers.compareFlags, arguments);
        }, { { SIZE8 }, { SIZE8 } }));

        Write8 = SPtr<Operation>(new Operation(
            "Write Byte", "WINT8", "out Bytes[A]",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            SS8 buffer {};
            buffer << std::dec << +cpu.registers.bytes[register_index];
            out(machine.out, buffer, cpu.flag_debug);
        }, { { SIZE8 } }));

        Read8 = SPtr<Operation>(new Operation(
            "Read Byte", "RINT8", "in Bytes[A]",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U16 value;
            SS8 buffer;
            U8 register_index = U8(arguments[0].value);
            buffer << in(machine.out, machine.in, cpu.flag_debug);
            buffer >> std::dec >> value;
            cpu.registers.bytes[register_index] = U8(value);
        }, { { SIZE8 } }));

        WriteAscii = SPtr<Operation>(new Operation(
            "Write Ascii", "WASCII", "out S8 A",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U16 address = U16(arguments[0].value);
            U16 offset = 0;
            SS8 buffer {};
            CH8 character {};
            do {
                character = CH8(U8(set2word(machine.readData(cpu, address + offset, SIZE8))));
                if (character != '\0') {
                    buffer << character;
                }
                offset++;
            } while (character != '\0');
            out(machine.out, buffer, cpu.flag_debug);
        }, { { SIZE16 } }));

    }
}