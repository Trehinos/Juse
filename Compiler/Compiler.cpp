
#include "Compiler.h"
#include <algorithm>
#include <cctype>
#include <locale>

#include "../Machine/utility.h"
#include "../Operations/operations.h"

Juse::ByteSet Juse::Jumne::linkOperation(Juse::U16 opKey, Juse::Operation& operation, std::vector<std::string> arguments)
{
    Juse::ByteSet set { Juse::U8(opKey & Juse::MASK_16TOP8 >> 8), Juse::U8(opKey & Juse::MASK_BOTTOM8) };
    size_t argIndex = 0;
    for (size_t index = 0; index < operation.length(); index++) {
        size_t argSize = operation.getArgumentDefs()[index].size;
        Juse::U64 argument = stoull(arguments[argIndex++]);
        Juse::ByteSet argSet;
        if (argSize == Juse::SIZE8) {
            argSet = Juse::word2set(Juse::U8(argument));
        } else if (argSize == Juse::SIZE16) {
            argSet = Juse::word2set(Juse::U16(argument));
        } else if (argSize == Juse::SIZE32) {
            argSet = Juse::word2set(Juse::U32(argument));
        } else if (argSize == Juse::SIZE64) {
            argSet = Juse::word2set(Juse::U64(argument));
        }
        for (Juse::U8 byte : argSet) {
            set.push_back(byte);
        }
    }
}

std::optional<Juse::Operation> Juse::Jumne::findOperation(Juse::OperationMap operations, std::string jumne, Juse::U16& opKey)
{
    for (const auto& [opkey, operation] : operations) {
        if (operation->mnemuCode() == jumne) {
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

Juse::ByteSet Juse::Jumne::Compiler::compileLine(std::string line)
{
    U16 opKey;
    line = Juse::Utility::trim(line);
    if (line.starts_with("#")) {
        if (line.starts_with("#DEFINE")) {
            // DEFINE
        }
        if (line.starts_with("#OFFSET")) {
            // OFFSET
        }
        if (line.starts_with("#DB")) {
            // DB
        }
        if (line.ends_with(":")) {
            // label:
        }
        if (line.starts_with(";")) {
            // ; comment
        }
    }

    Jumne::Instruction instr = parse(line);
    std::optional<Juse::Operation> operation = findOperation(operations, instr.operationKey, opKey);
    if (operation == std::nullopt) {
        return Juse::ByteSet {};
    }
    Operation op = operation.value();
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
        Juse::ByteSet set = compileLine(line);
        Utility::Memory::write(*memory, set, Address { pool, segment, addr });
        Utility::Memory::forward(pool, segment, addr, set.size());
    }
}
