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

uint8_t config_ReadFile(DISASM *pDisasm, char *configPath);
void config_ParseInstrSet(DISASM *pDisasm);

#endif // CONFIG_H_INCLUDED
