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
#define PROGRAM_INSTR_TYPE_JUMP_ABS 1
#define PROGRAM_INSTR_TYPE_JUMP_REL 2
#define PROGRAM_INSTR_TYPE_CALL 3

#define PROGRAM_LBL_LEN 16
#define PROGRAM_FUNC_LEN 16

uint8_t program_ReadFile(DISASM *pDisasm, char *programPath);
uint8_t program_ToHex(DISASM *pDisasm);
uint32_t program_GetOpcodeIndex(uint32_t hexCode, uint32_t opcodeListSize, OPCODE *pOpcodeList);
void program_GetJumpList(DISASM *pDisasm);
void program_GetCallList(DISASM *pDisasm);

#endif // PROGRAM_H_INCLUDED
