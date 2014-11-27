/**
 *  Disassembler
 *      by pyroesp, aka Nicolas Lopez Fernandez
 *
 *  Date: 1/09/2014
 *
 *  License:
 *      This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
**/

#ifndef DISASM_H_INCLUDED
#define DISASM_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "argument.h"

#define DISASM_LITTLE_ENDIAN 0
#define DISASM_BIG_ENDIAN 1

#define DISASM_ARG_HEX 0 // Write arg in hex
#define DISASM_ARG_DEC 1 // Write arg in dec

#define DISASM_ADD_ADDR 0x01 // 1 = add, 0 = don't add
#define DISASM_ADD_OPCODE 0x02 // 1 = add, 0 = don't add

typedef struct{
    uint32_t opcodeSize; // Size of opcode in bytes
    uint32_t hexVal; // Opcode hex value from config
    uint32_t type; // Type of opcode, either jump or call or non-jump
    uint32_t hexMask; // Opcode hex mask from config
    uint32_t argc; // Number of argument in opcode
    uint32_t *argMask; // Argument mask

    char *hexConfig; // Hex opcode string from config file
    char *mnemonic; // String of asm instruction
}OPCODE;

typedef struct{
    uint32_t total;
    uint32_t *address;
    char **name;
}List;

typedef List CALL; // CALL = List struct
typedef List JMP; // JMP = List struct

typedef struct{
    CMDLINE arg;

    uint32_t totalOpcode; // Total hexCode in config file
    OPCODE *opcodeList; // Opcode list

    char *config; // Config (CSV) buffer
    uint32_t configSize; // Size og config (CSV)
    char *program; // Program to disassemble
    uint32_t programSize; // Program size in bytes
    uint32_t smallestOpcodeSize; // Size in bytes of smallest opcode

    JMP jumpList; // List of jumps
    CALL callList; // List of calls

    char **outputASM; // Output string to save in file
}DISASM;

// Init DISASM structure
void disasm_Init(DISASM *pDisasm);
// Generate output code
void disasm_GenerateOutput(DISASM *pDisasm, uint32_t argBase);
// Find instruction in opcodeList
uint32_t disasm_GetInstruction(DISASM *pDisasm, uint32_t idx);
// Get Opcode from program buffer
uint32_t disasm_GetOpcode(DISASM *pDisasm, uint32_t idx, uint32_t opcodeSize);
// Get argument in opcode
uint32_t disasm_GetArg(uint32_t hexCode, uint32_t argMask);
// Convert argument to string
void disasm_ArgToString(char *argStr, uint32_t argVal, uint32_t argBase);
// Add ORG 0xAZERTY to output code
void disasm_AddOrigin(char **pOutput, uint32_t hexAddr);
// Add string to output code
void disasm_AddString(char **pOutput, char *string2Add);
// Replace char in string with another string
void disasm_StringReplace(char *inputString, char character, char *replaceString);
// Free DISASM structure
void disasm_Free(DISASM *pDisasm);

#endif // DISASM_H_INCLUDED
