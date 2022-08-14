
#include <algorithm>
#include <cctype>
#include <locale>

#include "../Juse/operations.h"
#include "../Juse/utility.h"

#include "Jumne.h"

using namespace Juse;

Juse::Jumne::JLabel::JLabel(S8 label, Juse::Address a) : Label{ label, true }, addr{ a } {
    if (a.segment == a.pool && a.pool == a.addr16 && a.pool == 0) {
        defined = false;
    }
}

Juse::ByteSet linkOperation(U16 opKey, Operation& operation, Vector<S8> arguments)
{
    ByteSet set = word2set(opKey);
    size_t argIndex = 0;
    for (size_t index = 0; index < operation.length(); index++) {
        size_t argSize = operation.getArgumentDefs()[index].size;
        U64 argument = stoull(arguments[argIndex++]);
        ByteSet argSet;
        if (argSize == SIZE8) {
            argSet = word2set(U8(argument));
        }
        else if (argSize == SIZE16) {
            argSet = word2set(U16(argument));
        }
        else if (argSize == SIZE32) {
            argSet = word2set(U32(argument));
        }
        else if (argSize == SIZE64) {
            argSet = word2set(U64(argument));
        }
        for (U8 byte : argSet) {
            set.push_back(byte);
        }
    }

    return set;
}

std::optional<Juse::Operation> findOperation(OperationMap operations, S8 jumne, U16& opKey)
{
    for (const auto& [opkey, operation] : operations) {
        if (operation->jumne() == jumne) {
            opKey = opkey;
            return *operation;
        }
    }
    return std::nullopt;
}

Juse::Jumne::JumneInstruction parse(S8 line)
{
    Jumne::JumneArgument buffer{};
    Compilation::Symbol operationKey{""};
    Jumne::JumneArguments arguments{};
    bool operationOk = false;

    for (char c : line) {
        if (!operationOk) {
            if (c == ' ') {
                operationOk = true;
                continue;
            }
            operationKey.base += c;
            continue;
        }
        if (c == ' ') {
            arguments.push_back(buffer);
            buffer = "";
        }
        else {
            buffer += c;
        }
    }
    if (buffer.size() > 0) {
        arguments.push_back(buffer);
    }

    return Jumne::JumneInstruction{ operationKey, arguments };
}
/*
Juse::ByteSet compileLine(Address addr, S8 line)
{
    U16 opKey;
    line = Utility::Strings::trim(line);
    if (line.size() == 0) { // Empty line
        return ByteSet{};
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
        return ByteSet{};
    }
    if (line.ends_with(":")) {
        labels.push_back(Label{ addr, Utility::Strings::trim(line, " :") });
        return ByteSet{};
    }
    if (line.starts_with(";")) {
        // ; comment
        return ByteSet{};
    }

    Jumne::Instruction instr = parse(line);
    std::optional<Operation> operation = findOperation(operations, instr.operationKey, opKey);
    if (operation == std::nullopt) {
        return Juse::ByteSet{};
    }
    Operation op = operation.value();
    size_t argIndex = 0;
    for (S8& argument : instr.arguments) {
        if (argument.starts_with("&")) {
            S8 lName = Utility::Strings::trim(argument, " &");
            for (Label l : labels) {
                if (l.defined and lName == l.label) {
                    argument = std::to_string(l.address.addr16);
                    break;
                }
            }
        }
    }
    return linkOperation(opKey, op, instr.arguments);
}
*/

Juse::Memory compile(Vector<S8> lines)
{
    U16 pool = 0;
    U32 segment = 0;
    U16 addr = 0;
    SPtr<Memory> memory = makeS<Memory>();
    /*
    for (S8 line : lines) {
        Address address{ pool, segment, addr };
        Juse::ByteSet set = compileLine(address, line);
        Utility::MachineMemory::write(*memory, set, address);
        Utility::MachineMemory::forward(pool, segment, addr, set.size());
    }
    */
    return *memory;
}

void testCompiler()
{
    using namespace Juse;
}
