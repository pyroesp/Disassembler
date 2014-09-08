/**
 *  Disassembler
 *      by pyroesp, aka Nicolas Lopez Fernandez
 *
 *  Date: 1/09/2014
 *
 *  License:
 *      This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
**/

#include "program.h"

uint8_t program_ReadFile(DISASM *pDisasm, char *programPath)
{
    FILE *file;
    file = fopen(programPath, "rb");

    if (file == NULL)
    {
        printf("Couldn't open file: %s\n", programPath);
        return 1;
    }

    fseek(file, 0, SEEK_END);
    pDisasm->programSize = ftell(file);
    rewind(file);

    pDisasm->program = (char*)malloc(pDisasm->programSize * sizeof(char) + 1);
    if (pDisasm->program == NULL)
        return 1;

    fread(pDisasm->program, sizeof(char), pDisasm->programSize, file);
    pDisasm->program[pDisasm->programSize] = 0;

    fclose(file);
    return 0;
}

uint8_t program_ToHex(DISASM *pDisasm)
{
    uint32_t sizeHexCode;
    uint32_t i, j;
    sizeHexCode = pDisasm->programSize / RES_OPCODE_SIZE;
    pDisasm->hexCodeSize = sizeHexCode;

    pDisasm->hexCode = (uint32_t*)malloc(sizeHexCode * sizeof(uint32_t));
    if (pDisasm->hexCode == NULL)
        return 1;

    pDisasm->hexAddress = (uint32_t*)malloc(sizeHexCode * sizeof(uint32_t));
    if (pDisasm->hexAddress == NULL)
        return 1;

    for (i = 0; i < sizeHexCode; i++)
    {
        pDisasm->hexAddress[i] = ((i == 0) ? (RES_PROGRAM_BASE_ADDRESS) : (pDisasm->hexAddress[i - 1] + RES_OPCODE_SIZE));
        pDisasm->hexCode[i] = pDisasm->program[i * RES_OPCODE_SIZE] & 0xFF;
        for (j = 1; j < RES_OPCODE_SIZE; j++)
        {
            pDisasm->hexCode[i] <<= 8;
            pDisasm->hexCode[i] |= (pDisasm->program[(i * RES_OPCODE_SIZE) + j] & 0xFF);
        }
    }

    return 0;
}

uint32_t program_GetOpcodeIndex(uint32_t hexCode, uint32_t opcodeListSize, OPCODE *pOpcodeList)
{
    uint32_t i;

    for (i = 0; i < opcodeListSize; i++)
    {
        if (((hexCode & pOpcodeList[i].hexMask) ^ pOpcodeList[i].hexVal) == 0)
            return i;
    }

    return (uint32_t)-1;
}

void program_GetJumpList(DISASM *pDisasm)
{
    uint32_t i, idx;

    for (i = 0; i < pDisasm->hexCodeSize; i++)
    {
        idx = program_GetOpcodeIndex(pDisasm->hexCode[i], pDisasm->totalOpcode, pDisasm->opcodeList);
        if (idx >= 0 && idx < pDisasm->totalOpcode)
        {
            if (pDisasm->opcodeList[idx].type == PROGRAM_INSTR_TYPE_JUMP_ABS)
            {
                pDisasm->jumpList.total++;
                if (pDisasm->jumpList.total == 1)
                {
                    pDisasm->jumpList.address = (uint32_t*)malloc(sizeof(uint32_t) * pDisasm->jumpList.total);
                    pDisasm->jumpList.name = (char**)malloc(sizeof(char*) * pDisasm->jumpList.total);
                    pDisasm->jumpList.name[pDisasm->jumpList.total - 1] = (char*)malloc(sizeof(char) * PROGRAM_LBL_LEN);
                }
                else
                {
                    pDisasm->jumpList.address = (uint32_t*)realloc(pDisasm->jumpList.address, sizeof(uint32_t) * pDisasm->jumpList.total);
                    pDisasm->jumpList.name = (char**)realloc(pDisasm->jumpList.name, sizeof(char*) * pDisasm->jumpList.total);
                    pDisasm->jumpList.name[pDisasm->jumpList.total - 1] = (char*)malloc(sizeof(char) * PROGRAM_LBL_LEN);
                }

                pDisasm->jumpList.address[pDisasm->jumpList.total - 1] = pDisasm->hexCode[i] & pDisasm->opcodeList[idx].argMask[0];
                sprintf(pDisasm->jumpList.name[pDisasm->jumpList.total - 1], "label_%03d:", pDisasm->jumpList.total);
            }
        }
    }
}

void program_GetCallList(DISASM *pDisasm)
{
    uint32_t i, idx;

    for (i = 0; i < pDisasm->hexCodeSize; i++)
    {
        idx = program_GetOpcodeIndex(pDisasm->hexCode[i], pDisasm->totalOpcode, pDisasm->opcodeList);
        if (idx >= 0 && idx < pDisasm->totalOpcode)
        {
            if (pDisasm->opcodeList[idx].type == PROGRAM_INSTR_TYPE_CALL)
            {
                pDisasm->callList.total++;
                if (pDisasm->callList.total == 1)
                {
                    pDisasm->callList.address = (uint32_t*)malloc(sizeof(uint32_t) * pDisasm->callList.total);
                    pDisasm->callList.name = (char**)malloc(sizeof(char*) * pDisasm->callList.total);
                    pDisasm->callList.name[pDisasm->callList.total - 1] = (char*)malloc(sizeof(char) * PROGRAM_LBL_LEN);
                }
                else
                {
                    pDisasm->callList.address = (uint32_t*)realloc(pDisasm->callList.address, sizeof(uint32_t) * pDisasm->callList.total);
                    pDisasm->callList.name = (char**)realloc(pDisasm->callList.name, sizeof(char*) * pDisasm->callList.total);
                    pDisasm->callList.name[pDisasm->callList.total - 1] = (char*)malloc(sizeof(char) * PROGRAM_LBL_LEN);
                }

                pDisasm->callList.address[pDisasm->callList.total - 1] = pDisasm->hexCode[i] & pDisasm->opcodeList[idx].argMask[0];
                sprintf(pDisasm->callList.name[pDisasm->callList.total - 1], "function_%03d:", pDisasm->callList.total);
            }
        }
    }
}
