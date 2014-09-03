#include "../include/config.h"

uint8_t config_ReadFile(DISASM *pDisasm, char *configPath)
{
    FILE *file;
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

void config_ParseInstrSet(DISASM *pDisasm)
{

}
