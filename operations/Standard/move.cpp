
#include "move.h"

namespace Juse::Operations::Standard::Move
{

    void add(OperationMap& operations) {
        operations[0x0100] = SetDataPool;
        operations[0x0101] = SetDataSegment;
        operations[0x0102] = SetAddressPointer;
        operations[0x0103] = SetAddressOffset;
        operations[0x0104] = SetAddressIncrement;
        operations[0x0105] = CopyToDataPool;
        operations[0x0106] = CopyToDataSegment;
        operations[0x0107] = CopyToAddressPointer;
        operations[0x0108] = CopyToAddressOffset;
        operations[0x0109] = CopyToAddressIncrement;
        operations[0x010A] = CopyFromDataPool;
        operations[0x010B] = CopyFromDataSegment;
        operations[0x010C] = CopyFromAddressPointer;
        operations[0x010D] = CopyFromAddressOffset;
        operations[0x010E] = CopyFromAddressIncrement;
        operations[0x0110] = PushDataPool;
        operations[0x0111] = PushDataSegment;
        operations[0x0112] = PushAddressPointer;
        operations[0x0113] = PushAddressOffset;
        operations[0x0114] = PushAddressIncrement;
        operations[0x0115] = PopDataPool;
        operations[0x0116] = PopDataSegment;
        operations[0x0117] = PopAddressPointer;
        operations[0x0118] = PopAddressOffset;
        operations[0x0119] = PopAddressIncrement;
    }

    void init() {
        SetDataPool = SPtr<Operation>(new Operation(
            "Set Data Pool", "DATAPOOL", "DP = A",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.data_addr.pool = PoolId(arguments[0].value);
        }, { { SIZE16 } }));

        SetDataSegment = SPtr<Operation>(new Operation(
            "Set Data Segment", "DATASEG", "DS = A",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.data_addr.segment = SegmentId(arguments[0].value);
        }, { { SIZE24 } }));

        SetAddressPointer = SPtr<Operation>(new Operation(
            "Set Address Pointer", "DATAPTR", "AP = A",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.data_addr.address = DataId(arguments[0].value);
        }, { { SIZE24 } }));

        SetAddressOffset = SPtr<Operation>(new Operation(
            "Set Address Offset", "DATAOFF", "AO = A",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.address_offset = DataId(arguments[0].value);
        }, { { SIZE24 } }));

        SetAddressIncrement = SPtr<Operation>(new Operation(
            "Set Address Increment", "DATAINC", "AI = A",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.address_increment = DataId(arguments[0].value);
        }, { { SIZE24 } }));

        CopyToDataPool = SPtr<Operation>(new Operation(
            "Copy To Data Pool", "COPYTODP", "DP = Words[A]",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            PoolId address = cpu.registers.words[register_index];
            cpu.data_addr.pool = address;
        }, { { SIZE8 } }));

        CopyToDataSegment = SPtr<Operation>(new Operation(
            "Copy To Data Segment", "COPYTODS", "DS = Addrs[A]",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            SegmentId address = cpu.registers.addrs[register_index];
            cpu.data_addr.segment = address;
        }, { { SIZE8 } }));

        CopyToAddressPointer = SPtr<Operation>(new Operation(
            "Copy To Address Pointer", "COPYTOAP", "AP = Addrs[A]",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            DataId address = cpu.registers.addrs[register_index];
            cpu.data_addr.address = address;
        }, { { SIZE8 } }));

        CopyToAddressOffset = SPtr<Operation>(new Operation(
            "Copy To Address Offset", "COPYTOAO", "AO = Addrs[A]",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            DataId address = cpu.registers.addrs[register_index];
            cpu.address_offset = address;
        }, { { SIZE8 } }));

        CopyToAddressIncrement = SPtr<Operation>(new Operation(
            "Copy To Address Increment", "COPYTOAI", "AI = Addrs[A]",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            DataId address = cpu.registers.addrs[register_index];
            cpu.address_increment = address;
        }, { { SIZE8 } }));

        CopyFromDataPool = SPtr<Operation>(new Operation(
            "Copy From Data Pool", "COPYFROMDP", "Words[A] = DP",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            cpu.registers.words[register_index] = cpu.data_addr.pool;
        }, { { SIZE8 } }));

        CopyFromDataSegment = SPtr<Operation>(new Operation(
            "Copy From Data Segment", "COPYFROMDS", "Addrs[A] = DS",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            cpu.registers.addrs[register_index] = cpu.data_addr.segment;
        }, { { SIZE8 } }));

        CopyFromAddressPointer = SPtr<Operation>(new Operation(
            "Copy From Address Pointer", "COPYFROMAP", "Addrs[A] = AP",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            cpu.registers.addrs[register_index] = cpu.data_addr.address;
        }, { { SIZE8 } }));

        CopyFromAddressOffset = SPtr<Operation>(new Operation(
            "Copy From Address Offset", "COPYFROMAO", "Addrs[A] = AO",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            cpu.registers.words[register_index] = cpu.address_offset;
        }, { { SIZE8 } }));

        CopyFromAddressIncrement = SPtr<Operation>(new Operation(
            "Copy From Address Increment", "COPYFROMAI", "Addrs[A] = AI",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            U8 register_index = U8(arguments[0].value);
            cpu.registers.words[register_index] = cpu.address_increment;
        }, { { SIZE8 } }));

        PushDataPool = SPtr<Operation>(new Operation(
            "Push Data Pool", "PUSHDP", "push DP",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.multiPush(word2set(cpu.data_addr.pool));
        }, {}));

        PushDataSegment = SPtr<Operation>(new Operation(
            "Push Data Segment", "PUSHDS", "push DS",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.multiPush(word2set(cpu.data_addr.segment));
        }, {}));

        PushAddressPointer = SPtr<Operation>(new Operation(
            "Push Address Pointer", "PUSHAP", "push AP",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.multiPush(word2set(cpu.data_addr.address));
        }, {}));

        PushAddressOffset = SPtr<Operation>(new Operation(
            "Push Address Offset", "PUSHAO", "push AO",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.multiPush(word2set(cpu.address_offset));
        }, {}));

        PushAddressIncrement = SPtr<Operation>(new Operation(
            "Push Address Increment", "PUSHAI", "push AI",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.multiPush(word2set(cpu.address_increment));
        }, {}));

        PopDataPool = SPtr<Operation>(new Operation(
            "Pop Data Pool", "POPDP", "DP = {pop}",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.data_addr.pool = PoolId(set2word(cpu.multiPop(SIZE16)));
        }, {}));

        PopDataSegment = SPtr<Operation>(new Operation(
            "Pop Data Segment", "POPDS", "DS = {pop}",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.data_addr.segment = SegmentId(set2word(cpu.multiPop(SIZE32)));
        }, {}));

        PopAddressPointer = SPtr<Operation>(new Operation(
            "Pop Address Pointer", "POPAP", "AP = {pop}",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.data_addr.address = DataId(set2word(cpu.multiPop(SIZE16)));
        }, {}));

        PopAddressOffset = SPtr<Operation>(new Operation(
            "Pop Address Offset", "POPAO", "AO = {pop}",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.address_offset = DataId(set2word(cpu.multiPop(SIZE16)));
        }, {}));

        PopAddressIncrement = SPtr<Operation>(new Operation(
            "Pop Address Increment", "POPAI", "AI = {pop}",
            [] (Machine& machine, Cpu& cpu, OperationArguments arguments) {
            cpu.address_increment = DataId(set2word(cpu.multiPop(SIZE16)));
        }, {}));
    }
}
