# Juse

Juse is a virtual machine which executes bytecode.

It is inspired by projects like :

- [Stack machines](https://en.wikipedia.org/wiki/Stack_machine) : [cslarsen implementation](https://github.com/cslarsen/stack-machine)
- [Chip-8](https://fr.wikipedia.org/wiki/CHIP-8) : [JamesGriffin implementation](https://github.com/JamesGriffin/CHIP-8-Emulator) or [cookerlyk implementation](https://github.com/cookerlyk/Chip8),
- [Chip-16](https://github.com/chip16/chip16) : implementation listed on Github page.
- Other virtual chips : [Khepra VM](https://github.com/tykel/khepra-vm) ([implementation](https://github.com/tykel/qpra))

It could be compared to the [JVM](https://en.wikipedia.org/wiki/Java_virtual_machine) or the [CLR](https://en.wikipedia.org/wiki/Common_Language_Runtime) machines.

**Juse is still in early development.**

- For now, there are only 34 operations implemented.  
    You can play with if you are curious ;)
- But there are a lot of features todo :
    - OpKeys are not fixed.
    - 2 assemblers languages are planned.
    - For now, I stay focused on the C++ code structure : one of the goals of this project is to create a virtual machine which is **easy to understand** and **easy to extend**.
    - No base frequency is defined for now. It will.

&copy; 2022 Sébastien GELDREICH

## Key features

- The goal : A **multipurpose virtual machine** written in modern C++
    - The machine has to be easy to understand (16 bits opkey with variable length opcodes)
    - The project can be used as a framework to create a custom virtual machine with custom opcodes.

Implemented features :
- The machine is composed of its **Cpu**, the **Stack** and the **Memory**.
- The **Cpu** can deal with 8, 16, 32 and 64 bits words
- 34 operations, general registers manipulation (read/write), byte IO and strings output

## Dependencies

### Environment

* Only tested on **Windows 11**
* Compiled with **Visual C++ 2022**

## Documentation

IN PROGRESS (will be moved in the wiki)

### Usage

#### (Future) : assembler

Juse will come with 2 assembler languages.

##### Jumne code (simple ASM)

Jumne stands for "JUse MNEmonics".

This language defines **mnemonics** which each of them will correspond to **a unique OpCode**.  
Something like :

```
; Adds 40 and 20
SET8 0 40
SET8 1 20
ADD8 2 0 1      
WINT8 2         ; prints 60
```

##### JuseLang code (advanced ASM)

A second one which simplify reading the code for the developper :

```
/* Adds 40 and 20 */
Bytes[0] = 40; Bytes[1] = 20;
Bytes[2] = Bytes[0] + Bytes[1];
out Bytes[2];   // prints 60
```

The JuseLang language will include `#PREPROCESSING` (define, include, segment, offset), `@labels` and `$references`.


#### Write a program

- For now, the program run by the VM is defined in the function `array<U8, 256> testProgram()` in `Juse.cpp`.
- You can increase the size of this array to a maximum of `SEGMENT_SIZE` (64 Ki = 65536).

#### Run the program

To run the program, use the command :
```
Juse.exe [-d]
```
| Option | Description                                                        |
|--------|--------------------------------------------------------------------|
| -d     | displays the operations list, and each OpCode before its execution |

### Some informations

- The stack can only push/pop `IP` for now.
- System registers :
    - `CP` code pool, `CS` code segment, `IP` instruction pointer,
    - `DP` data pool, `DS` data segment,
    - `flag_exit` (triggered by the `END` OpCode),
    - `flag_debug` (sets by the command invoking Juse).
    - `flag_skip` : used for conditionnal branching. If true : skip the next operation then set flag_skip to false.
    - Arithmetic flags :
        - `EQ` : true if `A ? B => A == B`
        - `GT` : true if `A ? B => A > B`
        - `LW` : true if `A ? B => A < B`
        - `GE` : true if `A ? B => A >= B`
        - `LE` : true if `A ? B => A <= B`
        - `NE` : true if `A ? B => A != B`
        - `Z0` : true if `A ? B => A == B == 0`
        - `CR` : carry flag
        - `OF` : overflow flag
        - `ERR` : error flag
- Addresses are 64 bits long (`U64`), the memory is chunked as : `Pool[U16]:Segment[U32]:U8[U16]`
- **One `Segment` is 64KiB**. It is the smallest amount of memory a program takes.
- **256 general purpose registers** of each type : `Byte (U8)`, `Word (U16)`, `Quad (U32)`, `Long (U64)`

#### Operations 

- OpCodes are variable length.
- They are identified by their OpKey, which is 16 bits long (`U16`).
- Implemented OpCodes :
```
===============================================================================================================
OP   : Jumne code                                       | JuseLang code                    | Operation name
===============================================================================================================
0000 : NOP                                              |                                  | Nothing
0001 : SHJUMP          A(U16)                           | goto DP:DS:A                     | Short Jump
0002 : JUMP            A(U32) B(U16)                    | goto DP:A:B                      | Jump
0003 : LJUMP           A(U64)                           | goto A                           | Long Jump
0004 : CALL            A(U16)                           | call A                           | Call
0005 : RET                                              | return                           | Return
0006 : LCALL           A(U64)                           | call! A                          | Long Call
0007 : LRET                                             | return!                          | Long Return
0008 : IF              A(U8)                            | if A                             | If
0009 : SKIP                                             | skip                             | Skip
000f : END                                              | end                              | End Program
0100 : DATAPOOL        A(U16)                           | datapool A                       | Set Data Pool
0101 : DATASEG         A(U32)                           | dataset A                        | Set Data Segment
0102 : DATAPTR         A(U16)                           | dataptr A                        | Set Address Pointer
0103 : DATAOFF         A(U16)                           | dataoffset A                     | Set Address Offset
01f0 : PUSHDS                                           | push DS                          | Push Data Segment
01f1 : POPDS                                            | pop DS                           | Pop Data Segment
0400 : ALLOCPOOL       A(U16)                           | alloc A                          | Allocate Pool
0401 : ALLOCSEG        A(U32) B(U16)                    | alloc A on B                     | Allocate Segment
1000 : SET8            A(U8) B(U8)                      | Bytes[A] = B                     | Set Byte
1001 : COPYFROM8       A(U8) B(U16)                     | Bytes[A] = [B]                   | Copy Byte From
1002 : COPYTO8         A(U16) B(U8)                     | [A] = Bytes[B]                   | Copy Byte To
1300 : WINT8           A(U8)                            | out Bytes[A]                     | Write Byte
1301 : RINT8           A(U8)                            | in Bytes[A]                      | Read Byte
1310 : WASCII          A(U16)                           | out S8 [A]                       | Write Ascii
1400 : SET16           A(U8) B(U16)                     | Words[A] = B                     | Set Word
1401 : COPYFROM16      A(U8) B(U16)                     | Words[A] = [B]                   | Copy Word From
1402 : COPYTO16        A(U16) B(U8)                     | [A] = Words[B]                   | Copy Word To
1800 : SET32           A(U8) B(U32)                     | Quads[A] = B                     | Set Quad
1801 : COPYFROM32      A(U8) B(U16)                     | Quads[A] = [B]                   | Copy Quad From
1802 : COPYTO32        A(U16) B(U8)                     | [A] = Quads[B]                   | Copy Quad To
1c00 : SET64           A(U8) B(U64)                     | Longs[A] = B                     | Set Long
1c01 : COPYFROM64      A(U8) B(U16)                     | Longs[A] = [B]                   | Copy Long From
1c02 : COPYTO64        A(U16) B(U8)                     | [A] = Longs[B]                   | Copy Long To
===============================================================================================================
```

## License

License : [MIT](LICENSE)  
&copy; 2022 Sébastien Geldreich
