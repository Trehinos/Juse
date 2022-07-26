# Juse

Juse is a virtual machine which executes bytecode.

It is inspired by projects like [Chip-8](https://en.wikipedia.org/wiki/CHIP-8) or [Chip-16](https://github.com/chip16/chip16).  
It may be compared to the JVM or the CLR machines.  

**Juse is still in early development.**

&copy; 2022 Sébastien GELDREICH

## Key features

- A **multipurpose virtual machine** written in modern C++
- The **Cpu** can deal with 8, 16, 32 and 64 bits words
- Addresses are 64 bits long, chunked as : `Pool[U16]:Segment[U32]:U8[U16]`
- **One `Segment` is 64KiB**. It is the smallest amount of memory a program takes.
- 

## Dependencies

### Environment

* Only tested on **Windows 11**
* Compiled with **Visual C++ 2022**

## Documentation

TODO

## License

License : [MIT](LICENSE)  
&copy; 2022 Sébastien Geldreich
