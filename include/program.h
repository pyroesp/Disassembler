#ifndef PROGRAM_H_INCLUDED
#define PROGRAM_H_INCLUDED

#include "disasm.h"

uint8_t program_ReadFile(DISASM *pDisasm, char *programPath);
uint8_t program_ToHex(DISASM *pDisasm);
void program_GetJumpList(DISASM *pDisasm);
void program_GetCallList(DISASM *pDisasm);

#endif // PROGRAM_H_INCLUDED
