/**
 *  Disassembler
 *      by pyroesp, aka Nicolas Lopez Fernandez
 *
 *  Date: 1/09/2014
 *
 *  License:
 *      This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
**/

#include "../include/program.h"

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
    pDisasm->bin = (char*)malloc(pDisasm->programSize * sizeof(char) + 1);
    if (pDisasm->bin == NULL)
        return 1;

    // Read file into program buffer
    fread(pDisasm->bin, sizeof(char), pDisasm->programSize, file);
    pDisasm->bin[pDisasm->programSize] = 0;

    fclose(file);
    return 0;
}

uint8_t program_ToHex(DISASM *pDisasm)
{
    uint32_t sizeHexCode;
    uint32_t i, j;
    sizeHexCode = pDisasm->programSize / pDisasm->arg.opcodeSize; // Get size of code, #bytes/opcodeSize
    pDisasm->hexCodeSize = sizeHexCode;

    // Allocate memory for hex opcodes
    pDisasm->hexCode = (uint32_t*)malloc(sizeHexCode * sizeof(uint32_t));
    if (pDisasm->hexCode == NULL)
        return 1;

    // Allocate memory for opcodes address
    pDisasm->hexAddress = (uint32_t*)malloc(sizeHexCode * sizeof(uint32_t));
    if (pDisasm->hexAddress == NULL)
        return 1;

    // For all opcodes
    for (i = 0; i < sizeHexCode; i++)
    {
        // Set the address of the opcode, either equals to base address or previous address + 1
        pDisasm->hexAddress[i] = ((i == 0) ? (pDisasm->arg.programBase) : (pDisasm->hexAddress[i - 1] + 1));

        switch (pDisasm->arg.endianness)
        {
            // MSB of opcode stored first
            case DISASM_BIG_ENDIAN:
                pDisasm->hexCode[i] = pDisasm->bin[i * pDisasm->arg.opcodeSize] & 0xFF; // Get MSB from program buffer at i*opcodeSize
                for (j = 1; j < pDisasm->arg.opcodeSize; j++) // For next bytes in opcode
                {
                    pDisasm->hexCode[i] <<= 8; // Shift right 8 bits
                    pDisasm->hexCode[i] |= (pDisasm->bin[(i * pDisasm->arg.opcodeSize) + j] & 0xFF); // Add byte to opcode
                }
                break;
            // LSB of opcode stored first
            case DISASM_LITTLE_ENDIAN:
            default:
                pDisasm->hexCode[i] = pDisasm->bin[i * pDisasm->arg.opcodeSize] & 0xFF; // Get LSB from program buffer at i*opcodeSize
                for (j = 1; j < pDisasm->arg.opcodeSize; j++) // For next bytes in opcode
                    pDisasm->hexCode[i] |= ((pDisasm->bin[(i * pDisasm->arg.opcodeSize) + j] & 0xFF) << (8*j)); // Add byte to opcode
                break;
        }
    }

    return 0;
}

uint32_t program_GetOpcodeIndex(uint32_t hexCode, uint32_t opcodeListSize, OPCODE *pOpcodeList)
{
    uint32_t i;

    // For all elements in the opcode list
    for (i = 0; i < opcodeListSize; i++)
    {
        // Search for the correct opcode
        if (((hexCode & pOpcodeList[i].hexMask) ^ pOpcodeList[i].hexVal) == 0)
            return i; // Return index of opcode
    }

    return (uint32_t)-1;
}

void program_GetList(DISASM *pDisasm, List *pList, uint32_t instrType, const char *name, uint32_t nameLen)
{
    uint32_t i, idx;

    // For all opcodes in the program
    for (i = 0; i < pDisasm->hexCodeSize; i++)
    {
        // Get index of opcode in list of opcodes
        idx = program_GetOpcodeIndex(pDisasm->hexCode[i], pDisasm->totalOpcode, pDisasm->opcodeList);
        if (idx != (uint32_t)-1) // If valid index
        {
            // If instruction is jump or call type
            if (pDisasm->opcodeList[idx].type == instrType)
            {
                // If call/jump address is not known
                if (program_CheckAddressList(pDisasm->hexCode[i] & pDisasm->opcodeList[idx].argMask[0], pList) == (uint32_t)-1)
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
                    pList->address[pList->total - 1] = pDisasm->hexCode[i] & pDisasm->opcodeList[idx].argMask[0];
                    // Add name of the function/label to the list
                    sprintf(pList->name[pList->total - 1], name, pList->total);
                }
            }
        }
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
