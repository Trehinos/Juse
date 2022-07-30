
#include "operations.h"

template <Juse::IsWord T>
void Juse::setWord<Juse::U8>(Juse::Operation&, Juse::Instruction&, Juse::GeneralRegisters<T>&);

/* 10xx-13xx */
void Juse::Operations::StandardExtensions::ext_u8(Cpu& cpu)
{
    cpu.operations[0x1000] = S<Operation>(new Operation(
        "Set Byte", "SET8", "Bytes[A] = B",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            setWord<U8>(operation, instruction, machine.cpu.registers.bytes);
        },
        { { SIZE8 }, { SIZE8 } }));

    cpu.operations[0x1001] = S<Operation>(new Operation(
        "Copy Byte From", "COPYFROM8", "Bytes[A] = [B]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U8 register_index = U8(operation.argument(instruction, 0));
            U16 address = U16(operation.argument(instruction, 1));
            U8 value = U8(set2word(machine.readData(address, SIZE8)));
            machine.cpu.registers.bytes[register_index] = value;
        },
        { { SIZE8 }, { SIZE16 } }));

    cpu.operations[0x1002] = S<Operation>(new Operation(
        "Copy Byte To", "COPYTO8", "[A] = Bytes[B]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U16 address = U16(operation.argument(instruction, 0));
            U8 register_index = U8(operation.argument(instruction, 1));
            machine.writeData(address, word2set<U8>(machine.cpu.registers.bytes[register_index]));
        },
        { { SIZE16 }, { SIZE8 } }));

  cpu.operations[0x1300] = S<Operation>(new Operation(
        "Write Byte", "WINT8", "out Bytes[A]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U8 register_index = U8(operation.argument(instruction, 0));
            std::stringstream buffer {};
            buffer << std::dec << +machine.cpu.registers.bytes[register_index];
            out(machine.out, buffer, machine.cpu.flag_debug);
        },
        { { SIZE8 } }));

    cpu.operations[0x1301] = S<Operation>(new Operation(
        "Read Byte", "RINT8", "in Bytes[A]",
        [](Machine& machine, Instruction& instruction, Operation& operation) {
            U16 value;
            std::stringstream buffer;
            U8 register_index = U8(operation.argument(instruction, 0));
            buffer << in(machine.out, machine.in, machine.cpu.flag_debug);
            buffer >> std::dec >> value;
            machine.cpu.registers.bytes[register_index] = U8(value);
        },
        { { SIZE8 } }));

  cpu.operations[0x1310] = S<Operation>(new Operation(
      "Write Ascii", "WASCII", "out S8 [A]",
      [](Machine& machine, Instruction& instruction, Operation& operation) {
          U16 address = U16(operation.argument(instruction, 0));
          U16 offset = 0;
          std::stringstream buffer {};
          CH8 character {};
          do {
              character = CH8(U8(set2word(machine.readData(address + offset, SIZE8))));
              if (character != '\0') {
                  buffer << character;
              }
              offset++;
          } while (character != '\0');
          out(machine.out, buffer, machine.cpu.flag_debug);
      },
      { { SIZE16 } }));

}