// check for c++
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/file.h>
#include <stdint.h>
#include <errno.h>

#include "headers/cstring.h"
#include "headers/opcodes.h"
#include "headers/rexcall.h"
#include "headers/register.h"
#include "headers/error.h"
#include "headers/crc32.h"
#include "headers/memoryengine.h"
#include "headers/rex/tree.h"

string buffer;

int run(int argc, string argv[])
{
    if (argc < 2)
    {
        error("Usage: %s <file>\n", argv[0]);
    }
    FILE *file = fopen(argv[1], "rb");

    if (file == NULL)
    {
        native_error("Error opening file \"%s\"\n", argv[1]);
    }

    fseek(file, 0, SEEK_END);
    uint32_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    buffer = (string)malloc(size);
    fread(buffer, size, 1, file);
    fclose(file);

    uint32_t identifier = (buffer[0] & 0xFF) | ((buffer[1] & 0xFF) << 8) | ((buffer[2] & 0xFF) << 16) | ((buffer[3] & 0xFF) << 24);
    if (identifier != FILE_IDENTIFIER)
    {
        native_error("Invalid file\n");
    }

    uint8_t *crcData = (uint8_t *)malloc(size - HEADER_SIZE);
    memcpy(crcData, buffer + HEADER_SIZE, size - HEADER_SIZE);

    uint32_t crc = rex_crc32(crcData, size - HEADER_SIZE);
    free(crcData);

    uint32_t dataCRC = (buffer[4] & 0xFF) | ((buffer[5] & 0xFF) << 8) | ((buffer[6] & 0xFF) << 16) | ((buffer[7] & 0xFF) << 24);

    if (crc != dataCRC)
    {
        native_error("CRC mismatch\n");
    }

    uint32_t _main = (buffer[8] & 0xFF) | ((buffer[9] & 0xFF) << 8) | ((buffer[10] & 0xFF) << 16) | ((buffer[11] & 0xFF) << 24);

    uint32_t version = (buffer[12] & 0xFF) | ((buffer[13] & 0xFF) << 8) | ((buffer[14] & 0xFF) << 16) | ((buffer[15] & 0xFF) << 24);

    if (version > REX_COMPILER_VER)
    {
        native_error("The Program was compiled with a more recent Version.\nPlease update to Version v%d or higher to run this executable.\nCurrent version: v%d\n", version, REX_COMPILER_VER);
    }
    if (version < REX_COMPILER_VER)
    {
        warn("The Program was compiled with a older Version. (Binary: v%d, Runtime: v%d)\nSome features may not work as advertised!\n", version, REX_COMPILER_VER);
    }

    for (uint32_t i = HEADER_SIZE; i < size; i++)
    {
        me_writeByte(i - HEADER_SIZE, buffer[i]);
    }

    ip = _main;

    int returnCode = -1;

    openFiles = (FILE **)malloc(sizeof(FILE *) * STACK_SIZE);

    while (ip < size)
    {
        uint8_t opcode = me_readByte(ip) & 0xFF;

        int b1 = opcode & 0x80;
        int b2 = opcode & 0x40;
        int b3 = opcode & 0x20;
        int b4 = opcode & 0x10;
        int b5 = opcode & 0x08;
        int b6 = opcode & 0x04;
        int b7 = opcode & 0x02;
        int b8 = opcode & 0x01;

        #ifdef DEBUG
        printf("Opcode: 0x%02x 0b" binaryPattern "\n", opcode, byteToBinary(opcode));
        #endif

        returnCode = parseBinaryTree(size, b1, b2, b3, b4, b5, b6, b7, b8);
        if (returnCode != -1)
        {
            return returnCode;
        }
    }
    #ifdef DEBUG
    me_heapDump("debug.dump");
    #endif
    return -1;
}

int main(int argc, string argv[])
{
    int r = run(argc, argv);
    if (r != 0)
    {
        me_heapDump("error.dump");
    }
    return r;
}

#ifdef __cplusplus
}
#endif
