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

#define PROGRAM_INSTR_TYPE_NONBRANCH 0 // Instruction type non branch
#define PROGRAM_INSTR_TYPE_JUMP_ABS 1 // Jump absolute type instruction
#define PROGRAM_INSTR_TYPE_JUMP_REL 2 // Jump relative type instruction
#define PROGRAM_INSTR_TYPE_CALL 3 // Call type instruction

#define PROGRAM_LBL_LEN 16 // Max label string length
#define PROGRAM_FUNC_LEN 16 // Max function string length

// Read program file to buffer
uint8_t program_ReadFile(DISASM *pDisasm, char *programPath);
// Convert program file to hex opcodes
uint8_t program_ToHex(DISASM *pDisasm);
// Get opcode index
uint32_t program_GetOpcodeIndex(uint32_t hexCode, uint32_t opcodeListSize, OPCODE *pOpcodeList);
// Make list of functions and goto labels
void program_GetList(DISASM *pDisasm, List *pList, uint32_t instrType, const char *name, uint32_t nameLen);
// Check the address in the list
uint32_t program_CheckAddressList(uint32_t hexAddress, List *pList);

#endif // PROGRAM_H_INCLUDED
