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

#define CONFIG_CSV_CHAR ';' // Separator char
#define CONFIG_VAR_CHAR '$' // Variable char

#define CONFIG_CSVPOS_INSTR 0 // Instruction position in CSV
#define CONFIG_CSVPOS_INSTR_MASK 1 // Instruction mask position in CSV
#define CONFIG_CSVPOS_INSTR_TYPE 2 // Instruction type position in CSV
#define CONFIG_CSVPOS_INSTR_MNEMONIC 3 // Instruction mnemonic position in CSV
#define CONFIG_CSVPOS_NUM_ARG 4 // Number of arguments position in CSV
#define CONFIG_CSVPOS_ARG_MASK 5 // Argument mask position in CSV

// Read CSV file to buffer
uint8_t config_ReadFile(DISASM *pDisasm, char *configPath);
// Get opcode list from csv in buffer
void config_ParseOpcodeList(DISASM *pDisasm);
// Get opcode list size
void config_OpcodeListSize(DISASM *pDisasm);
// Extract data from CSV
char* config_GetValue(DISASM *pDisasm, uint32_t *pIdx);
// Get length of csv element at index pIdx
uint32_t config_GetLength(DISASM *pDisasm, uint32_t *pIdx);
// Convert string opcode to hex
uint32_t config_OpcodeStr2Hex(char *pHexConfig);


#endif // CONFIG_H_INCLUDED
