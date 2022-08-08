

#include "operations.h"

/* 01xx */
void Juse::Operations::Standard::addMoves(Cpu& cpu)
{
    cpu.operations[0x0100] = S<Operation>(new Operation(
        "Set Data Pool", "DATAPOOL", "data pool A",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.data_pool = U16(arguments[0].value);
        },
        { { SIZE16 } }));

    cpu.operations[0x0101] = S<Operation>(new Operation(
        "Set Data Segment", "DATASEG", "data set A",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.data_segment = U32(arguments[0].value);
        },
        { { SIZE32 } }));

    cpu.operations[0x0102] = S<Operation>(new Operation(
        "Set Address Pointer", "DATAPTR", "data ptr A",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.address_pointer = U16(arguments[0].value);
        },
        { { SIZE16 } }));

    cpu.operations[0x0103] = S<Operation>(new Operation(
        "Set Address Offset", "DATAOFF", "data off A",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.address_offset = U16(arguments[0].value);
        },
        { { SIZE16 } }));

    cpu.operations[0x0104] = S<Operation>(new Operation(
        "Set Address Increment", "DATAINC", "data inc A",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.address_increment = U16(arguments[0].value);
        },
        { { SIZE16 } }));

    cpu.operations[0x0105] = S<Operation>(new Operation(
        "Copy To Data Pool", "COPYTODP", "DP = Words[A]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            U16 address = cpu.registers.words[register_index];
            cpu.data_pool = address;
        },
        { { SIZE8 } }));

    cpu.operations[0x0106] = S<Operation>(new Operation(
        "Copy To Data Segment", "COPYTODS", "DS = Quads[A]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            U32 address = cpu.registers.quads[register_index];
            cpu.data_segment = address;
        },
        { { SIZE8 } }));

    cpu.operations[0x0107] = S<Operation>(new Operation(
        "Copy To Address Pointer", "COPYTOAP", "AP = Words[A]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            U16 address = cpu.registers.words[register_index];
            cpu.address_pointer = address;
        },
        { { SIZE8 } }));

    cpu.operations[0x0108] = S<Operation>(new Operation(
        "Copy To Address Offset", "COPYTOAO", "AO = Words[A]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            U16 address = cpu.registers.words[register_index];
            cpu.address_offset = address;
        },
        { { SIZE8 } }));

    cpu.operations[0x0109] = S<Operation>(new Operation(
        "Copy To Address Increment", "COPYTOAI", "AI = Words[A]",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            U16 address = cpu.registers.words[register_index];
            cpu.address_increment = address;
        },
        { { SIZE8 } }));

    cpu.operations[0x010A] = S<Operation>(new Operation(
        "Copy From Data Pool", "COPYFROMDP", "Words[A] = DP",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            cpu.registers.words[register_index] = cpu.data_pool;
        },
        { { SIZE8 } }));

    cpu.operations[0x010B] = S<Operation>(new Operation(
        "Copy From Data Segment", "COPYFROMDS", "Quads[A] = DS",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            cpu.registers.quads[register_index] = cpu.data_segment;
        },
        { { SIZE8 } }));

    cpu.operations[0x010C] = S<Operation>(new Operation(
        "Copy From Address Pointer", "COPYFROMAP", "Words[A] = AP",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            cpu.registers.words[register_index] = cpu.address_pointer;
        },
        { { SIZE8 } }));

    cpu.operations[0x010D] = S<Operation>(new Operation(
        "Copy From Address Offset", "COPYFROMAO", "Words[A] = AO",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            cpu.registers.words[register_index] = cpu.address_offset;
        },
        { { SIZE8 } }));

    cpu.operations[0x010E] = S<Operation>(new Operation(
        "Copy From Address Increment", "COPYFROMAI", "Words[A] = AI",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            cpu.registers.words[register_index] = cpu.address_increment;
        },
        { { SIZE8 } }));

    cpu.operations[0x0110] = S<Operation>(new Operation(
        "Push Data Pool", "PUSHDP", "push DP",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.multiPush(word2set(cpu.data_pool));
        },
        {}));
    cpu.operations[0x0111] = S<Operation>(new Operation(
        "Push Data Segment", "PUSHDS", "push DS",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.multiPush(word2set(cpu.data_segment));
        },
        {}));
    cpu.operations[0x0112] = S<Operation>(new Operation(
        "Push Address Pointer", "PUSHAP", "push AP",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.multiPush(word2set(cpu.address_pointer));
        },
        {}));
    cpu.operations[0x0113] = S<Operation>(new Operation(
        "Push Address Offset", "PUSHAO", "push AO",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.multiPush(word2set(cpu.address_offset));
        },
        {}));
    cpu.operations[0x0114] = S<Operation>(new Operation(
        "Push Address Increment", "PUSHAI", "push AI",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.multiPush(word2set(cpu.address_increment));
        },
        {}));

    cpu.operations[0x0115] = S<Operation>(new Operation(
        "Pop Data Pool", "POPDP", "DP = {pop}",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.data_pool = U16(set2word(cpu.multiPop(SIZE16)));
        },
        {}));
    cpu.operations[0x0116] = S<Operation>(new Operation(
        "Pop Data Segment", "POPDS", "DS = {pop}",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.data_segment = U32(set2word(cpu.multiPop(SIZE32)));
        },
        {}));
    cpu.operations[0x0117] = S<Operation>(new Operation(
        "Pop Address Pointer", "POPAP", "AP = {pop}",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.address_pointer = U16(set2word(cpu.multiPop(SIZE16)));
        },
        {}));
    cpu.operations[0x0118] = S<Operation>(new Operation(
        "Pop Address Offset", "POPAO", "AO = {pop}",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.address_offset = U16(set2word(cpu.multiPop(SIZE16)));
        },
        {}));
    cpu.operations[0x0119] = S<Operation>(new Operation(
        "Pop Address Increment", "POPAI", "AI = {pop}",
        [](Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.address_increment = U16(set2word(cpu.multiPop(SIZE16)));
        },
        {}));
}