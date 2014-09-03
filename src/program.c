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
        printf("Couldn't open file.\n");
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

void program_GetJumpList(DISASM *pDisasm)
{

}

void program_GetCallList(DISASM *pDisasm)
{

}
