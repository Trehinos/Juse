# Juse

Juse is a virtual machine which executes bytecode.

It is inspired by projects like :

- [Stack machines](https://en.wikipedia.org/wiki/Stack_machine) : [clarsen impl.](https://github.com/cslarsen/stack-machine)
- [Chip-8](https://fr.wikipedia.org/wiki/CHIP-8) : [JamesGriffin impl.](https://github.com/JamesGriffin/CHIP-8-Emulator) or [cookerlyk impl.](https://github.com/cookerlyk/Chip8),
- [Chip-16](https://github.com/chip16/chip16) : impl. listed on Github page.
- Other virtual chips : [Khepra VM](https://github.com/tykel/khepra-vm) ([impl.](https://github.com/tykel/qpra))

It could be compared to the JVM or the CLR machines.

**Juse is still in early development.**

- For now, there are only 22 operations implemented.  
    Nothing to play with... But if you are curious ;)
- An assembler language is planned.
- Focus is on the C++ code structure for now.

&copy; 2022 Sébastien GELDREICH

## Key features

- The goal : A **multipurpose virtual machine** written in modern C++
- The machine is composed of its **Cpu**, the **Stack** and the **Memory**.
- The **Cpu** can deal with 8, 16, 32 and 64 bits words

## Dependencies

### Environment

* Only tested on **Windows 11**
* Compiled with **Visual C++ 2022**

## Documentation

IN PROGRESS (will be moved in the wiki)

### Usage

- The program run by the VM is defined in the function `array<U8, 256> testProgram()` in `Juse.cpp`.
    - You can increase the size of this array to a maximum of `SEGMENT_SIZE` (64 Ki = 65536).
- To run the program, use the command :
```
Juse.exe [-d]
```
| Option | Description       |
|--------|-------------------|
| -d     | enable debug mode |

### Some informations

- The stack can only push/pop `IP` for now.
- System registers :
    - `CP` code pool, `CS` code segment, `IP` instruction pointer,
    - `DP` data pool, `DS` data segment,
    - `flag_exit` (triggered by the `END` OpCode),
    - `flag_debug` (always `true` for now).
- Addresses are 64 bits long (`U64`), the memory is chunked as : `PooLongs[U16]:Segment[U32]:U8[U16]`
- **One `Segment` is 64KiB**. It is the smallest amount of memory a program takes.
- **256 general purpose registers** of each type : `Byte (U8)`, `Word (U16)`, `Quad (U32)`, `Long (U64)`
- Operations 
    - OpCodes are variable length.
    - They are identified by their OpKey, which is 16 bits long (`U16`).
    - Implemented OpCodes :
    ```
    NOP         0000

    SJUMP       0100 A(U16)                 : goto CP:CS:A          <=> IP = A
    JUMP        0101 A(U32) B(U16)          : goto CP:A:B           <=> CS = A; IP = B
    LJUMP       0102 A(U64)                 : goto A                <=> CP:CS:IP = A
    CALL        0103 A(U16)                 : call A                <=> Stack[] = IP; SJUMP A
    RET         0104                        : return                <=> IP = Stack(U16)
    LCALL       0105 A(U64)                 : call! A               <=> Stack[] = CP:CS:IP; LJUMP A
    LRET        0106                        : return                <=> CP:CS:IP = Stack(U64)
    DATAPOOL    0110 A(U16)                 : DP = A
    DATASEG     0111 A(U32)                 : DS = A
    END         01FF                        : flag_exit = true

    SET8        0200 A(U8) B(U8)            : Byte[A] = B
    SET16       0201 A(U8) B(U16)           : Word[A] = B
    SET32       0202 A(U8) B(U32)           : Quad[A] = B
    SET64       0203 A(U8) B(U64)           : Long[A] = B
    COPYFROM8   0210 A(U8) B(U16)           : Byte[A] = [DP:DS:B]
    COPYFROM16  0211 A(U8) B(U16)           : Word[A] = [DP:DS:B]
    COPYFROM32  0212 A(U8) B(U16)           : Quad[A] = [DP:DS:B]
    COPYFROM64  0213 A(U8) B(U16)           : Long[A] = [DP:DS:B]

    WASCII      10F0 A(U16)                 : out S8 [DP:DS:A]          ; prints an ASCII '\0' terminated string

    ALLOCPOOL   F000 A(U16)                 : alloc A                   ; allocate a new Pool with no Segment
    ALLOCSEG    F001 A(U16) B(U32)          : alloc B in A              ; allocate a new Segment in an existing Pool
    ```

## License

License : [MIT](LICENSE)  
&copy; 2022 Sébastien Geldreich
