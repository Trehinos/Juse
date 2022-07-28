# Juse

Juse is a virtual machine which executes bytecode.

It is inspired by projects like :

- [Stack machines](https://en.wikipedia.org/wiki/Stack_machine) : [cslarsen impl.](https://github.com/cslarsen/stack-machine)
- [Chip-8](https://fr.wikipedia.org/wiki/CHIP-8) : [JamesGriffin impl.](https://github.com/JamesGriffin/CHIP-8-Emulator) or [cookerlyk impl.](https://github.com/cookerlyk/Chip8),
- [Chip-16](https://github.com/chip16/chip16) : impl. listed on Github page.
- Other virtual chips : [Khepra VM](https://github.com/tykel/khepra-vm) ([impl.](https://github.com/tykel/qpra))

It could be compared to the [JVM](https://en.wikipedia.org/wiki/Java_virtual_machine) or the [CLR](https://en.wikipedia.org/wiki/Common_Language_Runtime) machines.

**Juse is still in early development.**

- For now, there are only 40 operations implemented.  
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
    ===============================================================================================================
    OP   : Mnemu code                                       | Ecal code                        | Operation name
    ===============================================================================================================
    0000 : NOP                                              |                                  | Nothing
    0100 : SHJUMP          A(U16)                           | goto DP:DS:A                     | Short Jump
    0101 : JUMP            A(U32) B(U16)                    | goto DP:A:B                      | Jump
    0102 : LJUMP           A(U64)                           | goto A                           | Long Jump
    0103 : CALL            A(U16)                           | call A                           | Call
    0104 : RET                                              | return                           | Return
    0105 : LCALL           A(U64)                           | call! A                          | Long Call
    0106 : LRET                                             | return!                          | Long Return
    0110 : DATAPOOL        A(U16)                           | datapool A                       | Set Data Pool
    0111 : DATASEG         A(U32)                           | dataset A                        | Set Data Segment
    01ff : END                                              | end                              | End Program
    0200 : SET8            A(U8) B(U8)                      | Bytes[A] = B                     | Set Byte
    0201 : SET16           A(U8) B(U16)                     | Words[A] = B                     | Set Word
    0202 : SET32           A(U8) B(U32)                     | Quads[A] = B                     | Set Quad
    0203 : SET64           A(U8) B(U64)                     | Longs[A] = B                     | Set Long
    0210 : COPYFROM8       A(U8) B(U16)                     | Bytes[A] = [B]                   | Copy Byte From
    0211 : COPYFROM16      A(U8) B(U16)                     | Words[A] = [B]                   | Copy Word From
    0212 : COPYFROM32      A(U8) B(U16)                     | Quads[A] = [B]                   | Copy Quad From
    0213 : COPYFROM64      A(U8) B(U16)                     | Longs[A] = [B]                   | Copy Long From
    0220 : COPYTO8         A(U16) B(U8)                     | [A] = Bytes[B]                   | Copy Byte To
    0221 : COPYTO16        A(U16) B(U8)                     | [A] = Words[B]                   | Copy Word To
    0222 : COPYTO32        A(U16) B(U8)                     | [A] = Quads[B]                   | Copy Quad To
    0223 : COPYTO64        A(U16) B(U8)                     | [A] = Longs[B]                   | Copy Long To
    1000 : WINT8           A(U8)                            | out Bytes[A]                     | Write Byte
    1001 : WINT16          A(U8)                            | out Words[A]                     | Write Word
    1002 : WINT32          A(U8)                            | out Quads[A]                     | Write Quad
    1003 : WINT64          A(U8)                            | out Longs[A]                     | Write Long
    1004 : WINT            A(U16)                           | out [DP:DS:A]                    | Write Direct
    1010 : RINT8           A(U8)                            | in Bytes[A]                      | Read Byte
    1011 : RINT16          A(U8)                            | in Words[A]                      | Read Word
    1012 : RINT32          A(U8)                            | in Quads[A]                      | Read Quad
    1013 : RINT64          A(U8)                            | in Longs[A]                      | Read Long
    10f0 : WASCII          A(U16)                           | out S8 [A]                       | Write Ascii
    10f1 : WUTF16          A(U16)                           | out S16 [A]                      | Write Utf-16
    10f2 : WUTF32          A(U16)                           | out S32 [A]                      | Write Utf-32
    2000 :                 A(U16)                           |                                  | Not implemented
    f000 : ALLOCPOOL       A(U16)                           | alloc A                          | Allocate Pool
    f001 : ALLOCSEG        A(U32) B(U16)                    | alloc A on B                     | Allocate Segment
    f1f0 : PUSHDS                                           | push DS                          | Push Data Segment
    f1f1 : POPDS                                            | pop DS                           | Pop Data Segment
    ===============================================================================================================
    ```

## License

License : [MIT](LICENSE)  
&copy; 2022 Sébastien Geldreich
