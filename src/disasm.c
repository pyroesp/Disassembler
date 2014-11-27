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
#include "program.h"
#include "disasm.h"

void disasm_Init(DISASM *pDisasm)
{
    memset(pDisasm, 0, sizeof(DISASM));
}

void disasm_GenerateOutput(DISASM *pDisasm, uint32_t argBase)
{
    uint32_t i, k;
    uint32_t idx;
    uint32_t opcode;
    uint32_t instr;
    uint32_t addr;
    uint32_t PC = pDisasm->arg.programBase;

    char mnemonic[128]; // Mnemonic string
    char name[32]; // Function or label name string
    char argStr[16]; // Argument value string (either hexadecimal or decimal)
//    char org[16]; // Origin of code

    pDisasm->outputASM = (char**)malloc(sizeof(char*)); // Just to have a valid pointer to start with
    pDisasm->outputASM[0] = (char*)malloc(sizeof(char));
    pDisasm->outputASM[0][0] = 0;

    disasm_AddOrigin(pDisasm->outputASM, pDisasm->arg.programBase); // Write origin to output code

    for (i = 0; i < pDisasm->programSize;)
    {
        instr = disasm_GetInstruction(pDisasm, i);

        if (instr == (uint32_t)-1)
        {
            // Add "unknown opcode" to output code
            sprintf(mnemonic, "\t; UNKNOWN OPCODE - 0x%X\n", disasm_GetOpcode(pDisasm, i, pDisasm->smallestOpcodeSize));
            disasm_AddString(pDisasm->outputASM, mnemonic);
            i += pDisasm->smallestOpcodeSize;
        }
        else
        {
            opcode = disasm_GetOpcode(pDisasm, i, pDisasm->opcodeList[instr].opcodeSize);

            // Check for function address & add name to output code
            idx = program_CheckAddressList(PC, &pDisasm->callList);
            if (idx != (uint32_t)-1)
            {
                sprintf(name, "\n%s:\n", pDisasm->callList.name[idx]);
                disasm_AddString(pDisasm->outputASM, name);
            }

            // Check for label address & add name to output code
            idx = program_CheckAddressList(PC, &pDisasm->jumpList);
            if (idx != (uint32_t)-1)
            {
                // Add origin for good measure before label
                disasm_AddString(pDisasm->outputASM, "\n");
                disasm_AddOrigin(pDisasm->outputASM, PC);
                sprintf(name, "%s:\n", pDisasm->jumpList.name[idx]);
                disasm_AddString(pDisasm->outputASM, name);
            }

            // Check for add address flag
//            if (flag & DISASM_ADD_ADDR)
//            {
//                // Add address to output code
//                sprintf(org, "0x%X", pDisasm->hexAddress[i]);
//                disasm_AddString(pDisasm->outputASM, org);
//                if (flag & DISASM_ADD_OPCODE)
//                    disasm_AddString(pDisasm->outputASM, " - ");
//                else
//                    disasm_AddString(pDisasm->outputASM, ": ");
//            }

            // Check for add opcode flag
//            if (flag & DISASM_ADD_OPCODE)
//            {
//                // Add opcode to output code
//                sprintf(org, "0x%X: ", pDisasm->hexCode[i]);
//                disasm_AddString(pDisasm->outputASM, org);
//            }

            // Get opcode mnemonic
            sprintf(mnemonic, "%s", pDisasm->opcodeList[instr].mnemonic);

            // Check opcode type
            switch (pDisasm->opcodeList[instr].type)
            {
                // If instruction is a CALL type
                case PROGRAM_INSTR_TYPE_CALL:
                    // Get address of CALL
                    addr = disasm_GetArg(opcode, pDisasm->opcodeList[instr].argMask[0]);
                    // Search address in call list
                    idx = program_CheckAddressList(addr, &pDisasm->callList);
                    if (idx == (uint32_t)-1) // if address not found in list
                    {
                        disasm_ArgToString(argStr, addr, argBase); // Convert hex address to string
                        disasm_StringReplace(mnemonic, (char)pDisasm->arg.varChar, argStr); // Replace variable char from mnemonic with hex string
                    }
                    else
                    {
                        // Replace variable char from mnemonic with function name string
                        disasm_StringReplace(mnemonic, (char)pDisasm->arg.varChar, pDisasm->callList.name[idx]);
                    }
                    break;
                // If instruction is a jump abs type
                case PROGRAM_INSTR_TYPE_JUMP_ABS:
                    // Get address of JUMP ABS
                    addr = disasm_GetArg(opcode, pDisasm->opcodeList[instr].argMask[0]);
                    // Search address in jump abs list
                    idx = program_CheckAddressList(addr, &pDisasm->jumpList);
                    if (idx == (uint32_t)-1)
                    {
                        disasm_ArgToString(argStr, addr, argBase); // Convert hex address to string
                        disasm_StringReplace(mnemonic, (char)pDisasm->arg.varChar, argStr); // Replace variable char from mnemonic with hex string
                    }
                    else
                    {
                        // Replace variable char from mnemonic with jump name string
                        disasm_StringReplace(mnemonic, (char)pDisasm->arg.varChar, pDisasm->jumpList.name[idx]);
                    }
                    break;
                // If instruction is a jump rel type
                case PROGRAM_INSTR_TYPE_JUMP_REL:
                // If instruction is none of the above, NON BRANCH type
                case PROGRAM_INSTR_TYPE_NONBRANCH:
                default:
                    // For all arguments in opcode
                    for(k = 0; k < pDisasm->opcodeList[instr].argc; k++)
                    {
                        // Get argument and convert it to string
                        disasm_ArgToString(argStr, disasm_GetArg(opcode, pDisasm->opcodeList[instr].argMask[k]), argBase);
                        disasm_StringReplace(mnemonic, (char)pDisasm->arg.varChar, argStr); // Replace variable char from mnemonic with hex string
                    }
                    break;
            }

            // Add tab + mnemonic & args + new line to the generated output ASM code
            disasm_AddString(pDisasm->outputASM, "\t");
            disasm_AddString(pDisasm->outputASM, mnemonic);
            disasm_AddString(pDisasm->outputASM, "\n");

            i += pDisasm->opcodeList[instr].opcodeSize;
        }
        PC++;
    }
}

uint32_t disasm_GetInstruction(DISASM *pDisasm, uint32_t idx)
{
    uint32_t i;
    uint32_t opcode;

    for (i = 0; i < pDisasm->totalOpcode; i++)
    {
        opcode = disasm_GetOpcode(pDisasm, idx, pDisasm->opcodeList[i].opcodeSize);
        if (((opcode & pDisasm->opcodeList[i].hexMask) ^ pDisasm->opcodeList[i].hexVal) == 0)
            return i;
    }

    return (uint32_t)-1;
}

uint32_t disasm_GetOpcode(DISASM *pDisasm, uint32_t idx, uint32_t opcodeSize)
{
    uint32_t retVal = 0;
    uint32_t i;

    for (i = 0; i < opcodeSize; i++)
    {
        if (pDisasm->arg.endianness == DISASM_BIG_ENDIAN)
            retVal |= (pDisasm->program[idx + i] & 0xFF); // Add byte to opcode
        else if (pDisasm->arg.endianness == DISASM_LITTLE_ENDIAN)
            retVal |= ((pDisasm->program[idx + opcodeSize - 1 - i] & 0xFF)); // Add byte to opcode

        if (i < (opcodeSize - 1))
            retVal <<= 8;
    }

    return retVal;
}


uint32_t disasm_GetArg(uint32_t hexCode, uint32_t argMask)
{
    uint32_t shift;
    uint32_t maskCopy = argMask;
    for (shift = 0; !(maskCopy & 0x01); shift++, maskCopy >>= 1);
    return ((hexCode & argMask) >> shift); // Mask argument from opcode and shift it
}

void disasm_ArgToString(char *argStr, uint32_t argVal, uint32_t argBase)
{
    switch(argBase)
    {
        // Write in decimal
        case DISASM_ARG_DEC:
            sprintf(argStr, "%u", argVal);
            break;
        // Write in hex
        case DISASM_ARG_HEX:
        default:
            sprintf(argStr, "0x%X", argVal);
    }
}

void disasm_AddOrigin(char **pOutput, uint32_t hexAddr)
{
    char buff[16];
    sprintf(buff, "\tORG 0x%X\n", hexAddr); // Make the ORG string
    disasm_AddString(pOutput, buff); // Add it to the output code
}

void disasm_StringReplace(char *input, char character, char *repl)
{
    char *pTemp;
    char *copyIn;
    // Allocate memory to make a copy of the input string
    copyIn = (char*)malloc(sizeof(char) * (strlen(input) + 1));
    // Copy input string
    strcpy(copyIn, input);
    copyIn[strlen(input)] = 0;

    // Search for the config variable char
    pTemp = strchr(input, character);
    strcpy(pTemp, repl); // Replace char with string
    strcpy(pTemp + strlen(repl), &copyIn[pTemp - input + 1]); // Readd end of input string that has been overwritten

    // Free copy of input
    free(copyIn);
}

void disasm_AddString(char **pOutput, char *string2Add)
{
    char *copyOut = NULL;
    // Allocate memory to make a copy of the output code
    copyOut = (char*)malloc(sizeof(char) * (strlen(*pOutput) + 1));
    memset(copyOut, 0, strlen(*pOutput) + 1);
    strcpy(copyOut, *pOutput); // Copy output string to copyOut buffer

    // Realloc the output code buffer
    pOutput[0] = (char*)realloc(*pOutput, sizeof(char) * (strlen(copyOut) + strlen(string2Add) + 1));
    // Rewrite the output code string with the data in copyOut
    strcpy(*pOutput, copyOut);
    // Concatenate the output code string with string2Add
    strcat(*pOutput, string2Add);

    // Free copyOut
    free(copyOut);
}

void disasm_Free(DISASM *pDisasm)
{
    uint32_t i;

    free(pDisasm->config);
    free(pDisasm->program);

    for (i = 0; i < pDisasm->totalOpcode; i++)
    {
        free(pDisasm->opcodeList[i].mnemonic);
        free(pDisasm->opcodeList[i].hexConfig);
        if( pDisasm->opcodeList[i].argc)
            free(pDisasm->opcodeList[i].argMask);

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
