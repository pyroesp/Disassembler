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

    // Get file length
    fseek(file, 0, SEEK_END);
    pDisasm->programSize = ftell(file);
    rewind(file);

    // Allocate memory for program buffer
    pDisasm->program = (char*)malloc(pDisasm->programSize * sizeof(char) + 1);
    if (pDisasm->program == NULL)
        return 1;

    // Read file into program buffer
    fread(pDisasm->program, sizeof(char), pDisasm->programSize, file);
    pDisasm->program[pDisasm->programSize] = 0;

    fclose(file);
    return 0;
}

void program_GetList(DISASM *pDisasm, List *pList, uint32_t instrType, const char *name, uint32_t nameLen)
{
    uint32_t i, idx, opcode;

    // For all bytes in the program
    for (i = 0; i < pDisasm->programSize;)
    {
        // Get index of opcode in list of opcodes
        idx = disasm_GetInstruction(pDisasm, i);
        if (idx != (uint32_t)-1) // If valid index
        {
            // If instruction is jump or call type
            if (pDisasm->opcodeList[idx].type == instrType)
            {
                opcode = disasm_GetOpcode(pDisasm, i, pDisasm->opcodeList[idx].opcodeSize);
                // If call/jump address is not known
                if (program_CheckAddressList(opcode & pDisasm->opcodeList[idx].argMask[0], pList) == (uint32_t)-1)
                {
                    // Add address to list
                    pList->total++;
                    if (pList->total == 1) // If first address is to be added
                    {
                        // Use malloc to allocate memory
                        pList->address = (uint32_t*)malloc(sizeof(uint32_t) * pList->total);
                        pList->name = (char**)malloc(sizeof(char*) * pList->total);
                        pList->name[pList->total - 1] = (char*)malloc(sizeof(char) * PROGRAM_LBL_LEN);
                    }
                    else // If it's not the first address to be added
                    {
                        // Use realloc to increase name buffer size
                        pList->address = (uint32_t*)realloc(pList->address, sizeof(uint32_t) * pList->total);
                        pList->name = (char**)realloc(pList->name, sizeof(char*) * pList->total);
                        pList->name[pList->total - 1] = (char*)malloc(sizeof(char) * nameLen);
                    }

                    // Add the address to the list
                    pList->address[pList->total - 1] = opcode & pDisasm->opcodeList[idx].argMask[0];
                    // Add name of the function/label to the list
                    sprintf(pList->name[pList->total - 1], name, pList->total);
                }
            }
            i += pDisasm->opcodeList[idx].opcodeSize;
        }
        else
            i++;
    }
}

uint32_t program_CheckAddressList(uint32_t hexAddress, List *pList)
{
    uint32_t i;

    // For all elements in list
    for (i = 0; i < pList->total; i++)
    {
        // Check if address exists
        if (hexAddress == pList->address[i])
            return i; // Return index of address in the list
    }

    return (uint32_t)-1; // Return -1 if not in list
}
