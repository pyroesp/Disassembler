#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include "disasm.h"

uint8_t config_ReadFile(DISASM *pDisasm, char *configPath);
void config_ParseInstrSet(DISASM *pDisasm);

#endif // CONFIG_H_INCLUDED
