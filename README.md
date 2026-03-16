# HexHacker 🐝🍯⬡

A linux command line program for viewing hex, written in C++

**NOTE:** This program is still in development! Please report any bugs!

## Dependencies

- `make` - Used to build project with Makefile
- `g++` - C++ Compiler and linker
- `libncurses-dev` - Library for Terminal Interfaces

## Building 🛠️

After installing dependencies, run `make` to build the software.

Your executable should be in `build/hexhacker` or in a different location that can be specified by `make EXEC=<name>`

## Usage

Open a file with HexHacker and it will look like this.

```
$ make
$ build/hexhacker test.bin

File: test.bin | 38 bytes | 1 block of 4096
Offset (x) | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
-----------+------------------------------------------------
0x00000000 | BA D8 D8 36 6A 0F F1 56 92 EB A7 7C BA 4C 0B 2B 
0x00000010 | 42 03 C0 7D 06 90 BE DA FE DB BC 5B 84 03 1C 21 
0x00000020 | 32 42 54 51 0B 8A 
```

We are currently working on scrolling the file and writing and saving files. Expect them soon!

## Debugging 🪰🚫

If you want to use a debugger on the program, run `make debug` when building it.
Then, you can use a debugger like gdb for debugging.
