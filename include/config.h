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

#define CONFIG_TSV_CHAR '\t' // Separator char


#define CONFIG_TSVPOS_OPCODESIZE 0 // Size of opcode in bytes
#define CONFIG_TSVPOS_INSTR 1 // Instruction position in TSV
#define CONFIG_TSVPOS_INSTR_MASK 2 // Instruction mask position in TSV
#define CONFIG_TSVPOS_INSTR_TYPE 3 // Instruction type position in TSV
#define CONFIG_TSVPOS_INSTR_MNEMONIC 4 // Instruction mnemonic position in TSV
#define CONFIG_TSVPOS_NUM_ARG 5 // Number of arguments position in TSV
#define CONFIG_TSVPOS_ARG_MASK 6 // Argument mask position in TSV


// Read TSV file to buffer
uint8_t config_ReadFile(DISASM *pDisasm, char *configPath);
// Get opcode list from TSV in buffer
void config_ParseOpcodeList(DISASM *pDisasm);
// Get opcode list size
void config_OpcodeListSize(DISASM *pDisasm);
// Extract data from TSV
char* config_GetValue(DISASM *pDisasm, uint32_t *pIdx);
// Get length of TSV element at index pIdx
uint32_t config_GetLength(DISASM *pDisasm, uint32_t *pIdx);
// Convert string opcode to hex
uint32_t config_OpcodeStr2Hex(char *pHexConfig);


#endif // CONFIG_H_INCLUDED
