/**
 *  Disassembler
 *      by pyroesp, aka Nicolas Lopez Fernandez
 *
 *  Date: 1/09/2014
 *
 *  License:
 *      This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
**/

// config/CHIP-8.csv program/SpaceInvaders.ch8
// config/CHIP-8.csv program/Maze.ch8

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

    disasm_GenerateOutput(&code, DISASM_ARG_HEX, DISASM_ADD_ADDR | DISASM_ADD_OPCODE);

    FILE *fp;
    fp = fopen("output/code.asm", "w");

    if (fp != NULL)
    {
        fprintf(fp, "%s", code.outputASM[0]);
        fclose(fp);
    }

    //printf("%s", code.outputASM[0]);

    disasm_Free(&code);
    return 0;
}
