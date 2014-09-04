/**
 *  Disassembler
 *      by pyroesp, aka Nicolas Lopez Fernandez
 *
 *  Date: 1/09/2014
 *
 *  License:
 *      This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
**/

#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include "disasm.h"

#define CONFIG_CSV_CHAR ';' // separator char
#define CONFIG_VAR_CHAR '$' // variable char

#define CONFIG_CSVPOS_INSTR 0
#define CONFIG_CSVPOS_INSTR_MASK 1
#define CONFIG_CSVPOS_INSTR_TYPE 2
#define CONFIG_CSVPOS_INSTR_MNEMONIC 3
#define CONFIG_CSVPOS_NUM_ARG 4
#define CONFIG_CSVPOS_ARG_MASK 5

#define CONFIG_MAX_INSTR_ARG 3

#define CONFIG_MAX_INSTR_TYPE 4
#define CONFIG_INSTR_TYPE_NONBRANCH 0
#define CONFIG_INSTR_TYPE_JUMP 1
#define CONFIG_INSTR_TYPE_CALL 2
#define CONFIG_INSTR_TYPE_RET 3


uint8_t config_ReadFile(DISASM *pDisasm, char *configPath);
void config_ParseOpcodeList(DISASM *pDisasm);
void config_OpcodeListSize(DISASM *pDisasm);
char* config_GetValue(DISASM *pDisasm, uint32_t *pIdx);
uint32_t config_GetLength(DISASM *pDisasm, uint32_t *pIdx);
uint32_t config_OpcodeStr2Hex(char *pHexConfig);


#endif // CONFIG_H_INCLUDED
