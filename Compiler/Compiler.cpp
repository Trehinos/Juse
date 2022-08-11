
#include "Compiler.h"
#include <algorithm>
#include <cctype>
#include <locale>

#include "../Models/utility.h"
#include "../Operations/operations.h"

Juse::ByteSet Juse::Jumne::linkOperation(U16 opKey, Operation& operation, std::vector<std::string> arguments)
{
    ByteSet set { U8(opKey & MASK_16TOP8 >> 8), U8(opKey & MASK_BOTTOM8) };
    size_t argIndex = 0;
    for (size_t index = 0; index < operation.length(); index++) {
        size_t argSize = operation.getArgumentDefs()[index].size;
        U64 argument = stoull(arguments[argIndex++]);
        ByteSet argSet;
        if (argSize == SIZE8) {
            argSet = word2set(U8(argument));
        } else if (argSize == SIZE16) {
            argSet = word2set(U16(argument));
        } else if (argSize == SIZE32) {
            argSet = word2set(U32(argument));
        } else if (argSize == SIZE64) {
            argSet = word2set(U64(argument));
        }
        for (U8 byte : argSet) {
            set.push_back(byte);
        }
    }
}

std::optional<Juse::Operation> Juse::Jumne::findOperation(OperationMap operations, std::string jumne, U16& opKey)
{
    for (const auto& [opkey, operation] : operations) {
        if (operation->jumne() == jumne) {
            opKey = opkey;
            return *operation;
        }
    }
    return std::nullopt;
}

Juse::Jumne::Instruction Juse::Jumne::parse(std::string line)
{
    std::string buffer {};
    std::string operationKey {};
    std::vector<std::string> arguments {};
    bool operationOk = false;

    for (char c : line) {
        if (!operationOk) {
            if (c == ' ') {
                operationOk = true;
                continue;
            }
            operationKey += c;
            continue;
        }
        if (c == ' ') {
            arguments.push_back(buffer);
            buffer = "";
        } else {
            buffer += c;
        }
    }
    if (buffer.size() > 0) {
        arguments.push_back(buffer);
    }

    return Instruction { operationKey, arguments };
}

Juse::ByteSet Juse::Jumne::Compiler::compileLine(Address addr, std::string line)
{
    U16 opKey;
    line = Utility::Strings::trim(line);
    if (line.size() == 0) { // Empty line
        return ByteSet {};
    }
    if (line.starts_with("#")) { // Directives
        if (line.starts_with("#OFFSET ")) { // #OFFSET n
            ByteSet ret;
            line = line.substr(8);
            size_t size = stoull(line);
            ret.assign(size, 0x00);
        }
        if (line.starts_with("#D")) {
            // #Dx{~n} (x € [B,W,Q,L] ; n € [0, 256])
        }
        return ByteSet {};
    }
    if (line.ends_with(":")) {
        labels.push_back(Label { addr, Utility::Strings::trim(line, " :") });
        return ByteSet {};
    }
    if (line.starts_with(";")) {
        // ; comment
        return ByteSet {};
    }

    Jumne::Instruction instr = parse(line);
    std::optional<Operation> operation = findOperation(operations, instr.operationKey, opKey);
    if (operation == std::nullopt) {
        return Juse::ByteSet {};
    }
    Operation op = operation.value();
    size_t argIndex = 0;
    for (std::string& argument : instr.arguments) {
        if (argument.starts_with("&")) {
            std::string lName = Utility::Strings::trim(argument, " &");
            for (Label l : labels) {
                if (l.defined and lName == l.label) {
                    argument = std::to_string(l.address.datum);
                    break;
                }
            }
        }
    }
    return linkOperation(opKey, op, instr.arguments);
}

Juse::Jumne::Compiler::Compiler(OperationMap& op)
    : operations(op)
    , labels {}
{
}

Juse::Memory Juse::Jumne::Compiler::compile(std::vector<std::string> lines)
{
    U16 pool = 0;
    U32 segment = 0;
    U16 addr = 0;
    S<Memory> memory = makeS<Memory>();

    for (std::string line : lines) {
        Address address { pool, segment, addr };
        Juse::ByteSet set = compileLine(address, line);
        Utility::MachineMemory::write(*memory, set, address);
        Utility::MachineMemory::forward(pool, segment, addr, set.size());
    }
}

void test()
{
    using namespace Juse;
    OperationMap operations {};
    loadOperationsSets(operations);
    Jumne::Compiler jasm { operations };
    jasm.compile({ "goto 4",
        "Bytes[0] = 1000",
        "out Bytes[0]" });
}
