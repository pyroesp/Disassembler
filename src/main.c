#include "../include/disasm.h"

int main(int argc, char *argv[])
{
    DISASM code;
    disasm_Init(&code);
    if (disasm_ReadConfig(&code, argv[1], DISASM_CONFIG_EXT))
        return 1;
    if (disasm_ReadProgram(&code, argv[2], RES_PROGRAM_EXT))
        return 1;

    if (disasm_ProgramToOpcode(&code))
        return 1;



    free(code.config);
    free(code.program);
    free(code.hexAddress);
    free(code.hexCode);

    //disasm_Free(&code);
    return 0;
}
