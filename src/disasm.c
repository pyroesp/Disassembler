/**
 *  Disassembler
 *      by pyroesp, aka Nicolas Lopez Fernandez
 *
 *  Date: 1/09/2014
 *
 *  License:
 *      This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
**/

#include "../include/disasm.h"

void disasm_Init(DISASM *pDisasm)
{
    // opcode list
    pDisasm->totalOpcode = 0;
    pDisasm->opcodeList = NULL;

    // program to disassemble
    pDisasm->program = NULL;
    pDisasm->hexAddress = NULL;
    pDisasm->hexCode = NULL;
    pDisasm->programSize = 0;

    // config file
    pDisasm->config = NULL;
    pDisasm->configSize = 0;

    // jump structure
    pDisasm->jumpList.total = 0;
    pDisasm->jumpList.address = NULL;
    pDisasm->jumpList.name = NULL;

    // call structure
    pDisasm->callList.total = 0;
    pDisasm->callList.address = NULL;
    pDisasm->callList.name = NULL;

    // output code
    pDisasm->outputASM = NULL;
}

void disasm_Free(DISASM *pDisasm)
{
    uint32_t i;

    free(pDisasm->config);
    free(pDisasm->hexAddress);
    free(pDisasm->program);

    for (i = 0; i < pDisasm->totalOpcode; i++)
    {
        free(pDisasm->opcodeList[i].argMask);
        free(pDisasm->opcodeList[i].mnemonic);
        free(pDisasm->opcodeList[i].hexConfig);
    }
    free(pDisasm->opcodeList);

    free(pDisasm->jumpList.address);
    free(pDisasm->jumpList.name);
    free(pDisasm->callList.address);
    free(pDisasm->callList.name);

    free(pDisasm->outputASM);
}



