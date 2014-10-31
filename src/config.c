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

    // Get file length
    fseek(file, 0, SEEK_END);
    pDisasm->configSize = ftell(file);
    rewind(file);

    // Allocate memory for config buffer
    pDisasm->config = (char*)malloc(pDisasm->configSize * sizeof(char) + 1);
    if (pDisasm->config == NULL)
        return 1;

    // Read file in buffer
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
    uint32_t csvPos = CONFIG_CSVPOS_INSTR; // set CSV position to instruction
    char *buff = NULL;

    // Get number of opcodes in CSV
    config_OpcodeListSize(pDisasm);
    // Allocate memory for opcodeList
    pDisasm->opcodeList = (OPCODE*)malloc(pDisasm->totalOpcode * sizeof(OPCODE));

    for (i = 0; i < pDisasm->configSize;)
    {
        // Get string at index i from CSV, increase i accordingly
        buff = config_GetValue(pDisasm, &i);

        switch(csvPos)
        {
            // 1. Read instruction
            case CONFIG_CSVPOS_INSTR:
                pDisasm->opcodeList[instr].hexConfig = buff; // Copy pointer to buffer containing the string opcode
                pDisasm->opcodeList[instr].hexVal = config_OpcodeStr2Hex(buff); // Convert string opcode to hex value
                csvPos = CONFIG_CSVPOS_INSTR_MASK; // Set next step to read instruction mask
                break;
            // 2. Read instruction mask
            case CONFIG_CSVPOS_INSTR_MASK:
                sscanf(buff, "0x%X", &pDisasm->opcodeList[instr].hexMask); // Convert instruction mask string to hex with sscanf
                csvPos = CONFIG_CSVPOS_INSTR_TYPE; // Set next step to read instruction type
                break;
            // 3. Read instruction type
            case CONFIG_CSVPOS_INSTR_TYPE:
                sscanf(buff, "%d", &pDisasm->opcodeList[instr].type); // Convert instruction type string to integer
                csvPos = CONFIG_CSVPOS_INSTR_MNEMONIC; // Set next step to read instruction mnemonic
                break;
            // 4. Read mnemonic
            case CONFIG_CSVPOS_INSTR_MNEMONIC:
                pDisasm->opcodeList[instr].mnemonic = buff; // Copy pointer to instruction mnemonic
                csvPos = CONFIG_CSVPOS_NUM_ARG; // Set next step to read instruction arguments
                break;
            // 5. Read total arguments of instruction
            case CONFIG_CSVPOS_NUM_ARG:
                sscanf(buff, "%d", &pDisasm->opcodeList[instr].argc); // Convert instruction argument to integer
                if (pDisasm->opcodeList[instr].argc) // if non null
                {
                    // Allocate memory for the argument masks
                    pDisasm->opcodeList[instr].argMask = (uint32_t*)malloc(pDisasm->opcodeList[instr].argc * sizeof(uint32_t));
                    csvPos = CONFIG_CSVPOS_ARG_MASK; // Set next step to read argument mask
                    arg = 0; // Set the arg variable (counter) to 0
                }
                else // If no arguments
                {
                    csvPos = CONFIG_CSVPOS_INSTR; // Go back to read instruction
                    instr++; // Increase instruction counter
                }
                break;
            // 6. Read arguments mask, if there are arguments
            case CONFIG_CSVPOS_ARG_MASK:
                sscanf(buff, "0x%X", &pDisasm->opcodeList[instr].argMask[arg]); // Convert argument mask string to hex
                arg++; // Increase argument counter
                if (arg >= pDisasm->opcodeList[instr].argc) // If all argument masks have been read
                {
                    instr++; // Increase instruction counter
                    csvPos = CONFIG_CSVPOS_INSTR; // Go back to read instruction
                }
                break;
        }

        // Increase variable i to skip certain chars
        if (pDisasm->config[i] == CONFIG_CSV_CHAR)
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

    // Check for all contents in csv file
    for (i = 0; i < pDisasm->configSize; i++)
    {
        // check if line starts with 0x
        if (pDisasm->config[i] == '0' && pDisasm->config[i + 1] == 'x' && isOpcode == 0)
            isOpcode = 1; // Opcode found

        // wait end of line
        if ((pDisasm->config[i] == '\n' || pDisasm->config[i] == 0) && isOpcode)
        {
            pDisasm->totalOpcode++; // Increase total opcode counter
            isOpcode = 0; // Reset isOpcode counter
        }
    }

    pDisasm->totalOpcode++; // Increase total opcode counter
}

char* config_GetValue(DISASM *pDisasm, uint32_t *pIdx)
{
    uint32_t length;
    char *buff;

    // Get length of csv element at index pIdx
    length = config_GetLength(pDisasm, pIdx);
    // Allocate memory for buffer
    buff = (char*)malloc(sizeof(char) * (length + 1));
    // Copy n char to buffer, starting at *pIdx
    strncpy(buff, &pDisasm->config[*pIdx], length);

    *pIdx += length; // Increase index
    buff[length] = 0;// Add termination char to buffer
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
    // Allocate memory to make a copy of the string opcode
    buff = (char*)malloc(sizeof(char) * (strlen(pHexConfig) + 1));
    // Copy opcode string to buff
    strcpy(buff, pHexConfig);
    buff[strlen(pHexConfig)] = 0;

    // Replace all chars that are not part of hexadecimal value
    for (i = strlen(pHexConfig) - 1; i > 0 && buff[i] != 'x'; i--)
    {
        if ((buff[i] < '0' || buff[i] > '9') && (buff[i] < 'A' || buff[i] > 'F'))
            buff[i] = '0'; // Replace char with '0'
    }

    // Convert string to hex
    sscanf(buff, "0x%X", &i);
    free(buff);
    return i;
}
