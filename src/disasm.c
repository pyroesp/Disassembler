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

uint8_t disasm_ReadProgram(DISASM *pDisasm, char *programPath, char *extension)
{
    FILE *file;

    if (strcmp(extension, programPath + strlen(programPath) - strlen(extension)) != 0)
    {
        printf("File type is not ch8.\n");
        printf("Exiting...\n");

        return 1;
    }

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

uint8_t disasm_ProgramToOpcode(DISASM *pDisasm)
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

uint8_t disasm_ReadConfig(DISASM *pDisasm, char *configPath, char *extension)
{
    FILE *file;

    if (strcmp(extension, configPath + strlen(configPath) - strlen(extension)) != 0)
    {
        printf("File type is not ch8.\n");
        printf("Exiting...\n");

        return 1;
    }

    file = fopen(configPath, "rb");

    if (file == NULL)
    {
        printf("Couldn't open file.\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    pDisasm->configSize = ftell(file);
    rewind(file);

    pDisasm->config = (char*)malloc(pDisasm->configSize * sizeof(char) + 1);
    if (pDisasm->config == NULL)
        return 1;

    fread(pDisasm->config, sizeof(char), pDisasm->configSize, file);
    pDisasm->config[pDisasm->configSize] = 0;

    fclose(file);
    return 0;
}

void disasm_ParseConfig(DISASM *pDisasm)
{

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
        free(pDisasm->opcodeList[i].opcodeASMConfig);
        free(pDisasm->opcodeList[i].opcodeHexConfig);
    }

    free(pDisasm->opcodeList);

    free(pDisasm->jumpList.address);
    free(pDisasm->jumpList.name);
    free(pDisasm->callList.address);
    free(pDisasm->callList.name);

    free(pDisasm->outputASM);
}



