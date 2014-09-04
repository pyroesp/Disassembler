/**
 *  Disassembler
 *      by pyroesp, aka Nicolas Lopez Fernandez
 *
 *  Date: 1/09/2014
 *
 *  License:
 *      This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
**/

#ifndef PROGRAM_H_INCLUDED
#define PROGRAM_H_INCLUDED

#include "disasm.h"

#define PROGRAM_INSTR_TYPE_NONBRANCH 0
#define PROGRAM_INSTR_TYPE_JUMP 1
#define PROGRAM_INSTR_TYPE_CALL 2
#define PROGRAM_INSTR_TYPE_RET 3

uint8_t program_ReadFile(DISASM *pDisasm, char *programPath);
uint8_t program_ToHex(DISASM *pDisasm);
void program_GetJumpList(DISASM *pDisasm);
void program_GetCallList(DISASM *pDisasm);

#endif // PROGRAM_H_INCLUDED
