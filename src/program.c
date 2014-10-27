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

void program_GetList(DISASM *pDisasm, List *pList, uint32_t instrType, const char *name, uint32_t nameLen)
{
    uint32_t i, idx;

    for (i = 0; i < pDisasm->hexCodeSize; i++)
    {
        idx = program_GetOpcodeIndex(pDisasm->hexCode[i], pDisasm->totalOpcode, pDisasm->opcodeList);
        if (idx >= 0 && idx < pDisasm->totalOpcode) // if valid index
        {
            if (pDisasm->opcodeList[idx].type == instrType) // if instruction is jump or call type
            {
                if (!program_CheckAddressList(pDisasm->hexCode[i] & pDisasm->opcodeList[idx].argMask[0], pList)) // if jump address is not known
                {
                    // add address to list
                    pList->total++;
                    if (pList->total == 1)
                    {
                        pList->address = (uint32_t*)malloc(sizeof(uint32_t) * pList->total);
                        pList->name = (char**)malloc(sizeof(char*) * pList->total);
                        pList->name[pList->total - 1] = (char*)malloc(sizeof(char) * PROGRAM_LBL_LEN);
                    }
                    else
                    {
                        pList->address = (uint32_t*)realloc(pList->address, sizeof(uint32_t) * pList->total);
                        pList->name = (char**)realloc(pList->name, sizeof(char*) * pList->total);
                        pList->name[pList->total - 1] = (char*)malloc(sizeof(char) * nameLen);
                    }

                    pList->address[pList->total - 1] = pDisasm->hexCode[i] & pDisasm->opcodeList[idx].argMask[0];
                    sprintf(pList->name[pList->total - 1], name, pList->total);
                }
            }
        }
    }
}

uint32_t program_CheckAddressList(uint32_t hexAddress, List *pList)
{
    uint32_t i;

    for (i = 0; i < pList->total; i++)
    {
        if (pList->address[i] == hexAddress)
            return i;
    }

    return (uint32_t)-1; // return -1 if not in list
}
