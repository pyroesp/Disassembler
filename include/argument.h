/**
 *  Disassembler
 *      by pyroesp, aka Nicolas Lopez Fernandez
 *
 *  Date: 16/11/2014
 *
 *  License:
 *      This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
**/

#ifndef ARGUMENT_H_INCLUDED
#define ARGUMENT_H_INCLUDED

#include <stdint.h>

#define ARGUMENT_DEF_SIZE 4
#define ARGUMENT_DEFPOS_PROGRAM_BASE 0
#define ARGUMENT_DEFPOS_OPCODE_SIZE 1
#define ARGUMENT_DEFPOS_ENDIANNESS 2
#define ARGUMENT_DEFPOS_VARCHAR 3

typedef struct{
    uint32_t programBase; // Base address of program in memory
    uint32_t opcodeSize; // Opcode size in bytes
    uint32_t endianness; // big endian, little endian
    uint32_t varChar; // Placeholder char for instruction vars
}CMDLINE;

uint32_t argument_ParseCmd(CMDLINE *arg, int argc, char **argv);

#endif
