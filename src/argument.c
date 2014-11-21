/**
 *  Disassembler
 *      by pyroesp, aka Nicolas Lopez Fernandez
 *
 *  Date: 16/11/2014
 *
 *  License:
 *      This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
**/

#include "argument.h"
#include "disasm.h"

const uint32_t defaultVal[ARGUMENT_DEF_SIZE] = {
    0x0000, // default program base address
    1, // default opcode size in bytes
    DISASM_BIG_ENDIAN, // default endianness
    (uint32_t)'$'
};

uint32_t argument_ParseCmd(CMDLINE *arg, int argc, char **argv)
{
    uint32_t i;

    if (argc < 2) // Check for enough arguments
        return 1;

    // Apply default values, extra commands will overwrite these if used
    arg->programBase = defaultVal[ARGUMENT_DEFPOS_PROGRAM_BASE];
    arg->opcodeSize = defaultVal[ARGUMENT_DEFPOS_OPCODE_SIZE];
    arg->endianness = defaultVal[ARGUMENT_DEFPOS_ENDIANNESS];
    arg->varChar = defaultVal[ARGUMENT_DEFPOS_VARCHAR];

    if (argc > 2) // Extra commands in command line, parse those
    {
        for (i = 3; i < argc; i++)
        {
            if (argv[i][0] == '-') // Check for start of command
            {
                switch (argv[i][1])
                {
                    // Base address
                    case 'b':
                    case 'B':
                        if (argv[i][3] == '0' && (argv[i][4] == 'x' || argv[i][4] == 'X'))
                            sscanf(&argv[i][5], "%X", &arg->programBase);
                        else if (argv[i][3] >= '0' && argv[i][3] <= '9')
                            sscanf(&argv[i][3], "%d", &arg->programBase);
                        else
                        {
                            printf("Error base address command.\n");
                            return 1;
                        }
                        break;
                    // Endianness of program
                    case 'e':
                    case 'E':
                        if (argv[i][3] == 'b' || argv[i][3] == 'B')
                            arg->endianness = DISASM_BIG_ENDIAN;
                        else if (argv[i][3] == 'l' || argv[i][3] == 'L')
                            arg->endianness = DISASM_LITTLE_ENDIAN;
                        else
                            return 1;
                        break;
                    // Print help
                    case 'h':
                    case 'H':
                        printf("Known commands:\n%s\n%s\n%s\n%s\n%s\n%s",
                            "\t-b or -B : Base address of program, either hexadecimal or decimal.",
                            "\t-e or -E : Program endianness, either b(ig) or l(ittle).",
                            "\t-h or -H : Prints this help.",
                            "\t-o or -O : Opcode size in bytes, in decimal.",
                            "\t-v or -V : Define the placeholder char for the instruction variables.",
                            "Exiting program ...");
                        return 1;
                    // Opcode size
                    case 'o':
                    case 'O':
                        if (argv[i][3] >= '0' && argv[i][3] <= '9')
                            sscanf(&argv[i][3], "%d", &arg->opcodeSize);
                        else
                        {
                            printf("Error opcode size command.\n");
                            return 1;
                        }
                        break;
                    // Var char
                    case 'v':
                    case 'V':
                        if (argv[i][3] != '\0' || argv[i][3] != '\t' || argv[i][3] != '\n' || argv[i][3] != '\r' || argv[i][3] != ' ')
                            arg->varChar = (uint32_t)argv[i][3];
                        else
                        {
                            printf("Error var char command, no special chars (zero, tab, whitespace, ...).\n");
                            return 1;
                        }
                        break;
                    // Unknown cmd
                    default:
                        printf("Unknown command in command line.\n");
                        return 1;
                }
            }
            else // Not a string starting with '-'
            {
                printf("Error in command line.\n");
                return 1;
            }
        }
    }
    return 0; // On success, return 0;
}
