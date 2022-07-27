# Juse

Juse is a virtual machine which executes bytecode.

It is inspired by projects like [Chip-8](https://en.wikipedia.org/wiki/CHIP-8) or [Chip-16](https://github.com/chip16/chip16).  
It may be compared to the JVM or the CLR machines.  

**Juse is still in early development.**
- For now, there are only 22 operations implemented.  
  Nothing to play with... But if you are curious ;)
- An assembler language is planned.

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

- The stack can only push/pop `IP` for now.
- System registers :
    - `CP` code pool, `CS` code segment, `IP` instruction pointer,
    - `DP` data pool, `DS` data segment,
    - `exit_flag` (triggered by the `END` OpCode),
    - `flag_debug` (always `true` for now).
- Addresses are 64 bits long (`U64`), the memory is chunked as : `Pool[U16]:Segment[U32]:U8[U16]`
- **One `Segment` is 64KiB**. It is the smallest amount of memory a program takes.
- **256 general purpose registers** of each type : `Byte (U8)`, `Word (U16)`, `Quad (U32)`, `Long (U64)`
- Operations 
    - OpCodes are variable length.
    - They are identified by their OpKey, which is 16 bits long (`U16`).
    - Implemented OpCodes :
    ```
    NOP			0000

    SJUMP		0100 A(U16)                 : goto CP:CS:A
    JUMP		0101 A(U32) B(U16)          : goto CP:A:B
    LJUMP		0102 A(U64)                 : goto A
    CALL		0103 A(U16)                 : call A (push IP; SJUMP A)
    RET         0104                        : return (pop IP)
    LCALL		0105 A(U64)                 : call! A (push CP:CS:IP; LJUMP A)
    LRET		0106                        : return (CP:CS:IP = pop)
    DATAPOOL	0110 A(U16)                 : DP = A
    DATASEG		0111 A(U32)                 : DS = A
    END         01FF                        : flag_exit = true

    SET8		0200 A(U8) B(U8)            : Byte[A] = B
    SET16		0201 A(U8) B(U16)           : Word[A] = B
    SET32		0202 A(U8) B(U32)           : Quad[A] = B
    SET64		0203 A(U8) B(U64)           : Long[A] = B
    COPYFROM8	0210 A(U8) B(U16)           : Byte[A] = [DP:DS:B]
    COPYFROM16	0211 A(U8) B(U16)           : Word[A] = [DP:DS:B]
    COPYFROM32	0212 A(U8) B(U16)           : Quad[A] = [DP:DS:B]
    COPYFROM64	0213 A(U8) B(U16)           : Long[A] = [DP:DS:B]

    WASCII		10F0 A(U16)                 : out S8 [DP:DS:A]

    ALLOCPOOL	F000 A(U16)                 : alloc A
    ALLOCSEG	F001 A(U16) B(U32)          : alloc B in A
    ```

## License

License : [MIT](LICENSE)  
&copy; 2022 Sébastien Geldreich
