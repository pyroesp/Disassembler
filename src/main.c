/**
 *  Disassembler
 *      by pyroesp, aka Nicolas Lopez Fernandez
 *
 *  Date: 1/09/2014
 *
 *  License:
 *      This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
**/

#include "disasm.h"
#include "config.h"
#include "program.h"

int main(int argc, char *argv[])
{
    uint32_t i;
    DISASM code;
    disasm_Init(&code);
    if (config_ReadFile(&code, argv[1]))
        return 1;
    if (program_ReadFile(&code, argv[2]))
        return 1;

    if (program_ToHex(&code))
        return 1;

    config_ParseOpcodeList(&code);

    free(code.config);
    free(code.program);
    free(code.hexAddress);
    free(code.hexCode);

    //disasm_Free(&code);
    return 0;
}
