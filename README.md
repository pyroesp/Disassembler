Disassembler
============

Disassembler
  by pyroesp

This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.

-------

The idea behind Disassembler is to make a universal disassembler, for whatever program you want to disassemble.

The program will work using a config file. It will contain all instruction set of the CPU for which the program has been compiled. It will contain an instruction mask, arg counter, arg mask, and finaly the instruction in ASM (or whatever), where $ is used as a placeholder for the arg in the opcode.

The resource.h file contains some info about the cpu, like the size of opcode and the base address of program code in the ROM.

In the future I'll add an intel hex format parser, which will "decode" the file by checking the extension of the program to be disassembled.

--------

How to use:

In cmd type - Disassembler.exe instruction_set.csv program.hex

--------

Log:
====

2/09/2014
- Added files to repo
- current config file is for CHIP-8 cpu

1/09/2014
- Disassembler project started
