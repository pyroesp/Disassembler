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
    DISASM code;
    disasm_Init(&code);
    if (config_ReadFile(&code, argv[1]))
        return 1;
    if (program_ReadFile(&code, argv[2]))
        return 1;

    if (program_ToHex(&code))
        return 1;

    config_ParseOpcodeList(&code);
    program_GetList(&code, &code.jumpList, PROGRAM_INSTR_TYPE_JUMP_ABS, "label_%03d", PROGRAM_LBL_LEN); // jump
    program_GetList(&code, &code.callList, PROGRAM_INSTR_TYPE_CALL, "function_%03d", PROGRAM_FUNC_LEN); // call

    disasm_GenerateOutput(&code, DISASM_ARG_DEC, DISASM_ADD_ADDR | DISASM_ADD_OPCODE);

    printf("%s", code.outputASM[0]);

    disasm_Free(&code);
    return 0;
}
