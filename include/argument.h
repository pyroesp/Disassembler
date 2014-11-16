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
#include "disasm.h"

#define ARGUMENT_DEF_SIZE 3
#define ARGUMENT_DEFPOS_PROGRAM_BASE 0
#define ARGUMENT_DEFPOS_OPCODE_SIZE 1
#define ARGUMENT_DEFPOS_ENDIANNES 2

uint32_t argument_ParseCmd(DISASM *pDisasm, int argc, char **argv);

#endif
