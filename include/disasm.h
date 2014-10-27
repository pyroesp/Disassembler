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

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "resource.h"

/* Defines */
#define DISASM_ARG_HEX 0 // write arg in hex
#define DISASM_ARG_DEC 1 // write arg in dec

#define DISASM_ADD_ADDR 0x01 // 1 = add, 0 = don't add
#define DISASM_ADD_OPCODE 0x02 // 1 = add, 0 = don't add


/* Structures */
typedef struct{
    uint32_t hexVal; // opcode hex value from config
    uint32_t type; // type of opcode, either jump or call or non-jump
    uint32_t hexMask; // opcode hex mask from config
    uint32_t argc; // number of argument in opcode
    uint32_t *argMask; // argument mask

    char *hexConfig; // hex opcode string from config file
    char *mnemonic; // string of asm instruction
}OPCODE;

typedef struct{
    uint32_t total;
    uint32_t *address;
    char **name;
}List;

typedef List CALL; // CALL = List struct
typedef List JMP; // JMP = List struct

typedef struct{
    uint32_t totalOpcode; // total hexCode in config file
    OPCODE *opcodeList; // opcode list

    char *config; // config file
    uint32_t configSize;
    char *program; // program to disassemble
    uint32_t programSize; // program size in bytes
    uint32_t *hexCode; // rearange program to have hexCode
    uint32_t *hexAddress; // address of hexCode
    uint32_t hexCodeSize;

    JMP jumpList; // list of jumps
    CALL callList; // list of calls

    char **outputASM; // output string to save in file
}DISASM;

/* Functions */
void disasm_Init(DISASM *pDisasm);
void disasm_GenerateOutput(DISASM *pDisasm, uint32_t argBase, uint32_t flag);
uint32_t disasm_GetArg(uint32_t hexCode, uint32_t argMask);
void disasm_ArgToString(char *argStr, uint32_t argVal, uint32_t argBase);
void disasm_AddOrigin(char **pOutput, uint32_t hexAddr);
void disasm_AddString(char **pOutput, char *string2Add);
void disasm_StringReplace(char *inputString, char character, char *replaceString);
void disasm_Free(DISASM *pDisasm);


#endif // DISASM_H_INCLUDED
