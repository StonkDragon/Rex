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
#include "headers/memoryengine.h"
#include "headers/rexcall.h"
#include "headers/register.h"
#include "headers/error.h"
#include "headers/crc32.h"
#include "headers/rex/tree.h"

int run(int argc, string argv[])
{
    if (argc < 2) {
        error("Usage: %s <file>\n", argv[0]);
    }
    FILE *file = fopen(argv[1], "rb");

    if (file == NULL) {
        native_error("Error opening file \"%s\"\n", argv[1]);
    }

    fseek(file, 0, SEEK_END);
    uint64_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    string buffer = (string) malloc(size);
    fread(buffer, size, 1, file);
    fclose(file);

    uint32_t identifier = (buffer[0] & 0xFF)
        | ((buffer[1] & 0xFF) << 8)
        | ((buffer[2] & 0xFF) << 16)
        | ((buffer[3] & 0xFF) << 24);
    if (identifier != FILE_IDENTIFIER) {
        native_error("Invalid file: %s\n", argv[1]);
    }

    uint8_t* crcData = (uint8_t*) malloc(size - HEADER_SIZE);
    for (uint64_t i = 0; i < size - HEADER_SIZE; i++) {
        crcData[i] = buffer[i + HEADER_SIZE];
    }

    uint32_t crc = rex_crc32(crcData, size - HEADER_SIZE);
    free(crcData);

    uint32_t dataCRC = (buffer[4] & 0xFF)
        | ((buffer[5] & 0xFF) << 8)
        | ((buffer[6] & 0xFF) << 16)
        | ((buffer[7] & 0xFF) << 24);

    if (crc != dataCRC) {
        native_error("CRC mismatch! Expected %u but got %u\n", crc, dataCRC);
    }

    openFiles = (FILE **) malloc(sizeof(FILE *) * STACK_SIZE);

    if (openFiles == NULL) {
        native_error("Error allocating memory for open files\n");
    }

    openFiles[0] = stdin;
    openFiles[1] = stdout;
    openFiles[2] = stderr;

    for (uint64_t i = HEADER_SIZE; i < size; i++) {
        me_writeByte(i - HEADER_SIZE, buffer[i]);
    }

    int returnCode = -1;

    ip = (uint64_t) (buffer[8] & 0xFF)
        | ((uint64_t) (buffer[9] & 0xFF) << 8)
        | ((uint64_t) (buffer[10] & 0xFF) << 16)
        | ((uint64_t) (buffer[11] & 0xFF) << 24)
        | ((uint64_t) (buffer[12] & 0xFF) << 32)
        | ((uint64_t) (buffer[13] & 0xFF) << 40)
        | ((uint64_t) (buffer[14] & 0xFF) << 48)
        | ((uint64_t) (buffer[15] & 0xFF) << 56);

    while (ip < size) {
        returnCode = parseBinaryTree(size);
        if (returnCode != -1) {
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
    if (r != 0) {
        me_heapDump("error.dump");
    }
    return r;
}

#ifdef __cplusplus
}
#endif
