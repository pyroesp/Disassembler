Disassembler
============

Disassembler
  by pyroesp

This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.

-------

The idea behind Disassembler is to make a universal disassembler, for whatever program you want to disassemble.
The program will work using a config.TSV file. This file contains all the instruction set of the CPU for which the program has been compiled.

My disassembler expects at least 2 inputs: The config.TSV filepath and the program.bin filepath.

The program file can be whatever extension you want it to be, but the disassembler expects a binary type file (a succession of bytes, not encoded, no CRC).

The TSV format is as follows:

instruction hex code - instruction hex mask code - instruction type - instruction mnemonic - number of arguments - argument mask 1 (optional) - argument mask 2 (optional)

Example from CHIP-8.TSV:  0x1NNN	0xF000	1	JP $	1	0x0FFF

There are 4 instruction types: non-branch (0), jump absolute (1), jump relative (2), call (3).

The $ sign is a place holder for the instruction argument(s). I'm thinking of maybe changing it to another char.

Note: If the CPU has a return instruction, you have to give it an instruction type of 0 even though it's a jumping type instruction.

The instruction type is used to make a list of call functions and goto labels

The disassembler has 4 optional commands as of now: 

-b or -B for the base address of the program, either decimal or hexadecimal.

-e or -E for the endianness of the binary file, either b(ig) or l(ittle).

-h or -H to display all the commands (this).

-o or -O for the size of an opcode in bytes, only in decimal

-v or -V to define the character used as placeholder for the instructions variables

If none is used parameters will keep their default values: BIG ENDIAN mode, base address at 0x0000 and an opcode size of 1 byte.

--------

Important:

For an isntruction like this "CLRF f	00 0001 1fff ffff", where there's a bit set in the same nibble as a variable it is important you make your instruction hex code like this "0x018X" and NOT "0x01XX".

If you use "0x01XX", the program won't know that the MSbit of the second nibble in this isntruction is supposed to be 1, ie. it will think the opcode is 0x0100, and the disassembler will probably not recognise the opcode in the bin file !

--------

How to use:

In cmd type - Disassembler.exe instruction_set.tsv program.bin [-b=0x200] [-e=BIG] [-h] [-o=2] [-v=$]