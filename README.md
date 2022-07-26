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

- A **multipurpose virtual machine** written in modern C++
- The **Cpu** can deal with 8, 16, 32 and 64 bits words
- Addresses are 64 bits long (`U64`), chunked as : `Pool[U16]:Segment[U32]:U8[U16]`
- **One `Segment` is 64KiB**. It is the smallest amount of memory a program takes.
- Operations 
	- OpCodes are variable length.
	- They are identified by their OpKey, which is 16 bits long (`U16`).
	- OpCodes :

		```
		NOP	0000
		SJUMP	0100
		JUMP	0101
		LJUMP	0102
		CALL	0103
		RET	0104
		LCALL	0105
		LRET	0106
		DATAPOOL	0110
		DATASEG	0111
		END	01FF
		SET8	0200
		SET16	0201
		SET32	0202
		SET64	0203
		COPYFROM8	0210
		COPYFROM16	0211
		COPYFROM32	0212
		COPYFROM64	0213
		WASCII	10F0
		ALLOCPOOL	F000
		ALLOCSEG	F001
		```


## Dependencies

### Environment

* Only tested on **Windows 11**
* Compiled with **Visual C++ 2022**

## Documentation

IN PROGRESS

## License

License : [MIT](LICENSE)  
&copy; 2022 Sébastien Geldreich
