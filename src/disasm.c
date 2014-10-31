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

void disasm_GenerateOutput(DISASM *pDisasm, uint32_t argBase, uint32_t flag)
{
    uint32_t i, j, k, idx, addr;
    uint8_t mnemonicAdded = 0;
    char mnemonic[128]; // mnemonic string
    char name[32]; // function or label name string
    char argStr[16]; // argument value string (either hexadecimal or decimal)
    char org[16]; // origine of code

    pDisasm->outputASM = (char**)malloc(sizeof(char*)); // just to have a valid pointer
    pDisasm->outputASM[0] = (char*)malloc(sizeof(char));
    pDisasm->outputASM[0][0] = 0;

    if (flag & DISASM_ADD_ADDR)
    {
        sprintf(org, "0x%X: ", RES_PROGRAM_BASE_ADDRESS);
        disasm_AddString(pDisasm->outputASM, org);
        if (flag & DISASM_ADD_OPCODE)
        {
            disasm_AddString(pDisasm->outputASM, " - \t");
        }
        else
        {
            disasm_AddString(pDisasm->outputASM, ": ");
        }
    }

    disasm_AddOrigin(pDisasm->outputASM, RES_PROGRAM_BASE_ADDRESS);

    for (i = 0; i < pDisasm->hexCodeSize; i++)
    {
        for (j = 0; j < pDisasm->totalOpcode; j++)
        {
            if (((pDisasm->hexCode[i] & pDisasm->opcodeList[j].hexMask) ^ pDisasm->opcodeList[j].hexVal) == 0)
            {
                if (flag & DISASM_ADD_ADDR)
                {
                    sprintf(org, "0x%X", pDisasm->hexAddress[i]);
                    disasm_AddString(pDisasm->outputASM, org);
                    if (flag & DISASM_ADD_OPCODE)
                    {
                        disasm_AddString(pDisasm->outputASM, " - ");
                    }
                    else
                    {
                        disasm_AddString(pDisasm->outputASM, ": ");
                    }
                }

                if (flag & DISASM_ADD_OPCODE)
                {
                    sprintf(org, "0x%X: ", pDisasm->hexCode[i]);
                    disasm_AddString(pDisasm->outputASM, org);
                }

                // check for function address & add name
                idx = program_CheckAddressList(pDisasm->hexAddress[i], &pDisasm->callList);
                if (idx != (uint32_t)-1)
                {
                    sprintf(name, "%s:\n", pDisasm->callList.name[idx]);
                    disasm_AddString(pDisasm->outputASM, name);
                }

                // check for label address & add name
                idx = program_CheckAddressList(pDisasm->hexAddress[i], &pDisasm->jumpList);
                if (idx != (uint32_t)-1)
                {
                    disasm_AddOrigin(pDisasm->outputASM, pDisasm->hexAddress[i]);
                    sprintf(name, "%s:\n", pDisasm->jumpList.name[idx]);
                    disasm_AddString(pDisasm->outputASM, name);
                }

                sprintf(mnemonic, "%s", pDisasm->opcodeList[j].mnemonic);

                switch (pDisasm->opcodeList[j].type)
                {
                    case PROGRAM_INSTR_TYPE_CALL:
                        // replace $ from mnemonic to function name
                        addr = disasm_GetArg(pDisasm->hexCode[i], pDisasm->opcodeList[j].argMask[0]);
                        idx = program_CheckAddressList(pDisasm->hexAddress[i], &pDisasm->callList);
                        if (idx == (uint32_t)-1)
                        {
                            disasm_ArgToString(argStr, addr, argBase);
                            disasm_StringReplace(mnemonic, CONFIG_VAR_CHAR, argStr);
                        }
                        else
                            disasm_StringReplace(mnemonic, CONFIG_VAR_CHAR, pDisasm->callList.name[idx]);
                        break;
                    case PROGRAM_INSTR_TYPE_JUMP_ABS:
                        // replace $ from mnemonic to label name
                        addr = disasm_GetArg(pDisasm->hexCode[i], pDisasm->opcodeList[j].argMask[0]);
                        idx = program_CheckAddressList(pDisasm->hexAddress[i], &pDisasm->jumpList);
                        if (idx == (uint32_t)-1)
                        {
                            disasm_ArgToString(argStr, addr, argBase);
                            disasm_StringReplace(mnemonic, CONFIG_VAR_CHAR, argStr);
                        }
                        else
                            disasm_StringReplace(mnemonic, CONFIG_VAR_CHAR, pDisasm->callList.name[idx]);
                        break;
                    default:
                        // replace $ from mnemonic to arg
                        for(k = 0; k < pDisasm->opcodeList[j].argc; k++)
                        {
                            disasm_ArgToString(argStr, disasm_GetArg(pDisasm->hexCode[i], pDisasm->opcodeList[j].argMask[k]), argBase);
                            disasm_StringReplace(mnemonic, CONFIG_VAR_CHAR, argStr);
                        }
                        break;
                }

                // Add tab + mnemonic & args + new line to the generated output ASM code
                disasm_AddString(pDisasm->outputASM, "\t");
                disasm_AddString(pDisasm->outputASM, mnemonic);
                disasm_AddString(pDisasm->outputASM, "\n");

                mnemonicAdded = 1;
            }
        }

        // if mnemonic not found
        if (mnemonicAdded == 0)
        {
            if (flag & DISASM_ADD_ADDR)
            {
                sprintf(org, "0x%X", pDisasm->hexAddress[i]);
                disasm_AddString(pDisasm->outputASM, org);
                if (flag & DISASM_ADD_OPCODE)
                {
                    disasm_AddString(pDisasm->outputASM, " - ");
                }
                else
                {
                    disasm_AddString(pDisasm->outputASM, ": ");
                }
            }

            if (flag & DISASM_ADD_OPCODE)
            {
                sprintf(org, "0x%X: ", pDisasm->hexCode[i]);
                disasm_AddString(pDisasm->outputASM, org);
            }
            disasm_AddString(pDisasm->outputASM, "\tUNKNOWN OPCODE\n");
        }

        mnemonicAdded = 0;
    }
}

uint32_t disasm_GetArg(uint32_t hexCode, uint32_t argMask)
{
    uint32_t shift;
    uint32_t maskCopy = argMask;
    for (shift = 0; !(maskCopy & 0x01); shift++, maskCopy >>= 1);
    return ((hexCode & argMask) >> shift);
}

void disasm_ArgToString(char *argStr, uint32_t argVal, uint32_t argBase)
{
    switch(argBase)
    {
        case DISASM_ARG_DEC:
            sprintf(argStr, "%u", argVal);
            break;
        case DISASM_ARG_HEX:
        default:
            sprintf(argStr, "0x%X", argVal);
    }
}

void disasm_AddOrigin(char **pOutput, uint32_t hexAddr)
{
    char buff[16];
    sprintf(buff, "\tORG 0x%X\n", hexAddr);
    disasm_AddString(pOutput, buff);
}

void disasm_StringReplace(char *input, char character, char *repl)
{
    char *pTemp;
    char *copyIn;
    copyIn = (char*)malloc(sizeof(char) * (strlen(input) + 1));
    strcpy(copyIn, input);
    copyIn[strlen(input)] = 0;

    pTemp = strchr(input, character);
    strcpy(pTemp, repl);
    strcpy(pTemp + strlen(repl), &copyIn[pTemp - input + 1]);

    free(copyIn);
}

void disasm_AddString(char **pOutput, char *string2Add)
{
    char *copyOut = NULL;
    copyOut = (char*)malloc(sizeof(char) * (strlen(*pOutput) + 1));
    memset(copyOut, 0, strlen(*pOutput) + 1);
    strcpy(copyOut, *pOutput);

    pOutput[0] = (char*)realloc(*pOutput, sizeof(char) * (strlen(copyOut) + strlen(string2Add) + 1));
    strcpy(*pOutput, copyOut);
    strcat(*pOutput, string2Add);

    free(copyOut);
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
    for (i = 0; i < pDisasm->jumpList.total; i++)
        free(pDisasm->jumpList.name[i]);
    free(pDisasm->jumpList.name);

    free(pDisasm->callList.address);
    for (i = 0; i < pDisasm->callList.total; i++)
        free(pDisasm->callList.name[i]);
    free(pDisasm->callList.name);

    free(&pDisasm->outputASM[0]);
    free(pDisasm->outputASM);
}
