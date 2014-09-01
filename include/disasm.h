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
#define DISASM_CSV_CHAR ';' // separator char
#define DISASM_VAR_CHAR '$' // variable char

#define DISARM_ARG_HEX 1 // write arg in hex if 1, dec if 0

#define DISASM_CONFIG_EXT ".csv"


/* Structures */
typedef struct{
    uint16_t opcodeHex; // opcode hex value from config
    uint16_t opcodeMask; // opcode hex mask from config
    uint16_t argc; // number of argument in opcode
    uint16_t *argMask; // argument mask

    char *opcodeHexConfig; // hex opcode string from config file
    char *opcodeASMConfig; // string of asm instruction
}OPCODE;

typedef struct{
    uint32_t total; // total jumps
    uint32_t *address; // address of jump
    uint32_t *name; // name of jump
}JMP;

typedef JMP CALL; // CALL = JMP struct

typedef struct{
    uint32_t totalOpcode; // total hexCode in config file
    OPCODE *opcodeList; // opcode list

    char *program; // program to disassemble
    uint32_t *hexCode; // rearange program to have hexCode
    uint32_t *hexAddress; // address of hexCode
    uint32_t programSize; // program size in bytes
    char *config; // config file
    uint32_t configSize;

    JMP jumpList; // list of jumps
    CALL callList; // list of calls

    char *outputASM; // output string to save in file
}DISASM;

/* Functions */
void disasm_Init(DISASM *pDisasm);
uint8_t disasm_ReadProgram(DISASM *pDisasm, char *programPath, char *extension);
uint8_t disasm_ProgramToOpcode(DISASM *pDisasm);
uint8_t disasm_ReadConfig(DISASM *pDisasm, char *configPath, char *extension);
void disasm_ParseConfig(DISASM *pDisasm);
void disasm_Free(DISASM *pDisasm);


#endif // DISASM_H_INCLUDED
