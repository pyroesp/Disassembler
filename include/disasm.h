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
#define DISASM_ARG_HEX 1 // write arg in hex if 1, dec if 0


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
    uint32_t total; // total jumps
    uint32_t *address; // address of jump
    uint32_t **name; // name of jump
}JMP;

typedef JMP CALL; // CALL = JMP struct

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

    char *output; // output string to save in file
    uint32_t outputSize;
}DISASM;

/* Functions */
void disasm_Init(DISASM *pDisasm);
void disasm_Free(DISASM *pDisasm);


#endif // DISASM_H_INCLUDED
