# Juse

Juse is a virtual machine which executes bytecode.

It is inspired by projects like :

- [Stack machines](https://en.wikipedia.org/wiki/Stack_machine) : [cslarsen implementation](https://github.com/cslarsen/stack-machine)
- [Chip-8](https://fr.wikipedia.org/wiki/CHIP-8) : [JamesGriffin implementation](https://github.com/JamesGriffin/CHIP-8-Emulator) or [cookerlyk implementation](https://github.com/cookerlyk/Chip8),
- [Chip-16](https://github.com/chip16/chip16) : implementation listed on Github page.
- Other virtual chips : [Khepra VM](https://github.com/tykel/khepra-vm) ([implementation](https://github.com/tykel/qpra))

It could be compared to the [JVM](https://en.wikipedia.org/wiki/Java_virtual_machine) or the [CLR](https://en.wikipedia.org/wiki/Common_Language_Runtime) machines.

The instruction space distribution is inspired by the [RISC-V architecture](https://escholarship.org/content/qt7zj0b3m7/qt7zj0b3m7.pdf).

&copy; 2022 Sébastien GELDREICH

## The goal

**Juse is still in early development.**  
But you can play with it if you are curious.

- The goal : A **multipurpose virtual machine** written in modern C++
    - The machine has to be **easy to understand** (variable length opcodes with 16 bits opkey),
    - The project can be used as a framework to create a custom virtual machine with custom opcodes,
- There are a lot of features todo :
    - OpKeys specification is not fixed,
    - 2 assemblers languages are planned,
    - For now, I stay focused on the C++ code structure : one of the goals of this project is to create a virtual machine which is **easy to understand** and **easy to extend**,

*Only using `std::` and custom code*

## Key features

Implemented features :
- The machine is composed of its **Cpu**, the **Stack** and the **Memory**,
- The **Cpu** can deal with 8, 16, 32 and 64 bits words,
- Machine have **threadable Cpus** with **choosen frequency** (fixed from 1 Hz to 1 GHz ; 4 KHz by default),
- **117 operations** : branching, moves, threads, allocations, general registers manipulation (push/pop/read/write/unsigned arithmetic), integers and strings I/O 

## Dependencies

### Environment

* Only tested on **Windows 11**
* Compiled with **Visual C++ 2022**

## Documentation

[Documentation](https://asgard.trehinos.eu:9080/Trehinos/juse/-/wikis/home)

## License

License : [MIT](LICENSE)  
&copy; 2022 Sébastien Geldreich
