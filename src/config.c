/**
 *  Disassembler
 *      by pyroesp, aka Nicolas Lopez Fernandez
 *
 *  Date: 1/09/2014
 *
 *  License:
 *      This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
**/

#include "config.h"

uint8_t config_ReadFile(DISASM *pDisasm, char *configPath)
{
    FILE *file;
    file = fopen(configPath, "rb");

    if (file == NULL)
    {
        printf("Couldn't open file: %s\n", configPath);
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

void config_ParseOpcodeList(DISASM *pDisasm)
{
    uint32_t i;
    uint32_t instr = 0;
    uint32_t arg;
    uint32_t csvPos = CONFIG_CSVPOS_INSTR;
    char *buff = NULL;

    config_OpcodeListSize(pDisasm);
    pDisasm->opcodeList = (OPCODE*)malloc(pDisasm->totalOpcode * sizeof(OPCODE));

    for (i = 0; i < pDisasm->configSize;)
    {
        buff = config_GetValue(pDisasm, &i);

        switch(csvPos)
        {
            case CONFIG_CSVPOS_INSTR:
                pDisasm->opcodeList[instr].hexConfig = buff;
                pDisasm->opcodeList[instr].hexVal = config_OpcodeStr2Hex(buff);
                csvPos = CONFIG_CSVPOS_INSTR_MASK;
                break;
            case CONFIG_CSVPOS_INSTR_MASK:
                sscanf(buff, "0x%X", &pDisasm->opcodeList[instr].hexMask);
                csvPos = CONFIG_CSVPOS_INSTR_TYPE;
                break;
            case CONFIG_CSVPOS_INSTR_TYPE:
                sscanf(buff, "%d", &pDisasm->opcodeList[instr].type);
                csvPos = CONFIG_CSVPOS_INSTR_MNEMONIC;
                break;
            case CONFIG_CSVPOS_INSTR_MNEMONIC:
                pDisasm->opcodeList[instr].mnemonic = buff;
                csvPos = CONFIG_CSVPOS_NUM_ARG;
                break;
            case CONFIG_CSVPOS_NUM_ARG:
                sscanf(buff, "%d", &pDisasm->opcodeList[instr].argc);
                if (pDisasm->opcodeList[instr].argc)
                {
                    pDisasm->opcodeList[instr].argMask = (uint32_t*)malloc(pDisasm->opcodeList[instr].argc * sizeof(uint32_t));
                    csvPos = CONFIG_CSVPOS_ARG_MASK;
                    arg = 0;
                }
                else
                {
                    csvPos = CONFIG_CSVPOS_INSTR;
                    instr++;
                }
                break;
            case CONFIG_CSVPOS_ARG_MASK:
                sscanf(buff, "0x%X", &pDisasm->opcodeList[instr].argMask[arg]);
                arg++;
                if (arg >= pDisasm->opcodeList[instr].argc)
                {
                    instr++;
                    csvPos = CONFIG_CSVPOS_INSTR;
                }
                break;
        }

        if (pDisasm->config[i] == ';')
            i++;
        else if (pDisasm->config[i] == '\n')
            i++;
        else if (pDisasm->config[i] == '\r' && pDisasm->config[i + 1] == '\n')
            i += 2;
    }
}

void config_OpcodeListSize(DISASM *pDisasm)
{
    uint32_t i;
    uint32_t isOpcode = 0;

    for (i = 0; i < pDisasm->configSize; i++)
    {
        // check if line starts with 0x
        if (pDisasm->config[i] == '0' && pDisasm->config[i + 1] == 'x' && isOpcode == 0)
            isOpcode = 1;

        // wait end of line
        if ((pDisasm->config[i] == '\n' || pDisasm->config[i] == 0) && isOpcode)
        {
            pDisasm->totalOpcode++;
            isOpcode = 0;
        }
    }

    pDisasm->totalOpcode++;
}

char* config_GetValue(DISASM *pDisasm, uint32_t *pIdx)
{
    uint32_t startIdx;
    uint32_t length;
    char *buff;

    length = config_GetLength(pDisasm, pIdx);
    buff = (char*)malloc(length + 1);

    startIdx = *pIdx;
    for (; *pIdx < (startIdx + length); (*pIdx)++)
    {
        buff[*pIdx - startIdx] = pDisasm->config[*pIdx];
    }

    buff[length] = 0;
    return buff;
}

uint32_t config_GetLength(DISASM *pDisasm, uint32_t *pIdx)
{
    uint32_t i;

    for(i = 0;  pDisasm->config[*pIdx + i] != CONFIG_CSV_CHAR &&    \
                pDisasm->config[*pIdx + i] != '\n' &&               \
                pDisasm->config[*pIdx + i] != '\r' &&               \
                pDisasm->config[*pIdx + i] != 0                     \
                ; i++);

    return i;
}

uint32_t config_OpcodeStr2Hex(char *pHexConfig)
{
    uint32_t i;
    char *buff;
    buff = (char*)malloc(strlen(pHexConfig));
    strcpy(buff, pHexConfig);

    for (i = strlen(pHexConfig) - 1; i > 0 && buff[i] != 'x'; i--)
    {
        if ((buff[i] < '0' || buff[i] > '9') && (buff[i] < 'A' || buff[i] > 'F'))
            buff[i] = '0';
    }

    sscanf(buff, "0x%X", &i);
    return i;
}
