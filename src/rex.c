// check for c++
#ifdef __cplusplus
extern "C"
{
#endif

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#define OS_WINDOWS
// #error "Rasm is currently not supported on Windows!"
#else

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "headers/cutil.h"
#include "headers/cstring.h"
#include "headers/opcodes.h"
#include "headers/memoryengine.h"
#include "headers/rexcall.h"
#include "headers/register.h"
#include "headers/error.h"
#include "headers/crc32.h"
#include "headers/rex/tree.h"
#include "headers/rasm/asm.h"

FILE* inputFile;
string inputFileName;

int run()
{
    if (inputFile == NULL) {
        native_error("Error opening file \"%s\"\n", inputFileName);
    }

    fseek(inputFile, 0, SEEK_END);
    uint64_t size = ftell(inputFile);
    fseek(inputFile, 0, SEEK_SET);

    string buffer = (string) mallocOrErr(size);
    fread(buffer, size, 1, inputFile);
    fclose(inputFile);

    uint32_t identifier = (buffer[0] & 0xFF)
        | ((buffer[1] & 0xFF) << 8)
        | ((buffer[2] & 0xFF) << 16)
        | ((buffer[3] & 0xFF) << 24);
    if (identifier != FILE_IDENTIFIER) {
        native_error("Invalid file: %s\n", inputFileName);
    }

    uint8_t* crcData = (uint8_t*) mallocOrErr(size - HEADER_SIZE);
    for (uint64_t i = 0; i < size - HEADER_SIZE; i++) {
        crcData[i] = buffer[i + HEADER_SIZE];
    }

    uint32_t crc = rex_crc32(crcData, size - HEADER_SIZE);
    freeOrErr(crcData);

    uint32_t dataCRC = (buffer[4] & 0xFF)
        | ((buffer[5] & 0xFF) << 8)
        | ((buffer[6] & 0xFF) << 16)
        | ((buffer[7] & 0xFF) << 24);

    if (crc != dataCRC) {
        native_error("CRC mismatch! Expected %u but got %u\n", crc, dataCRC);
    }

    openFiles = (FILE **) mallocOrErr(sizeof(FILE *) * STACK_SIZE);

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
        returnCode = executeOpcode(size);
        if (returnCode != -1) {
            return returnCode;
        }
    }

    #ifdef DEBUG
    me_heapDump("debug.dump");
    #endif
    return -1;
}

int compile() {
    if (inputFile == 0) {
        native_error("Could not open file %s\n", inputFileName);
    }
    string outFile = (string) mallocOrErr(strlen(inputFileName) - 3);
    strncpy(outFile, inputFileName, strlen(inputFileName) - 5);
    strcat(outFile, ".rx");
    fseek(inputFile, 0, SEEK_END);
    int size = ftell(inputFile);
    fseek(inputFile, 0, SEEK_SET);
    
    string file_buffer = (string) mallocOrErr(size);
    fread(file_buffer, size, 1, inputFile);
    fclose(inputFile);

    string tokens[HEAP_SIZE];

    string token = strtok(file_buffer, " \n");
    // string token_buffer = (string) mallocOrErr(size);
    string buffer = (string) mallocOrErr(size);
    // string included = (string) mallocOrErr(size);

    int token_count = 0;
    while (token != NULL) {
        if (strlen(token) > 0) {
            tokens[token_count++] = token;
            // strcat(token_buffer, token);
            // strcat(token_buffer, " ");
        }
        token = strtok(NULL, " \n");
    }
    // strcpy(included, token_buffer);

    labels = (rasm_label_t*) mallocOrErr(sizeof(rasm_label_t) * STACK_SIZE);

    // free(token_buffer);
    bin_parseIncludes(tokens, &token_count);

    // strcpy(buffer, token_buffer);
    bin_parseLabels(tokens, &token_count);

    uint64_t asm_size = asm_writeData(tokens, &token_count);

    uint8_t* code = (uint8_t*) mallocOrErr(asm_size + HEADER_SIZE);
    
    code[0] = FILE_IDENTIFIER & 0xFF;
    code[1] = (FILE_IDENTIFIER >> 8) & 0xFF;
    code[2] = (FILE_IDENTIFIER >> 16) & 0xFF;
    code[3] = (FILE_IDENTIFIER >> 24) & 0xFF;

    uint64_t crc = rex_crc32(asm_data, asm_size);
    code[4] = crc & 0xFF;
    code[5] = (crc >> 8) & 0xFF;
    code[6] = (crc >> 16) & 0xFF;
    code[7] = (crc >> 24) & 0xFF;
    
    string entryPointLabel = bin_getEntryPointLabel();

    uint64_t entryPoint = bin_getAddressOfLabel(entryPointLabel);
    code[8] = entryPoint & 0xFF;
    code[9] = (entryPoint >> 8) & 0xFF;
    code[10] = (entryPoint >> 16) & 0xFF;
    code[11] = (entryPoint >> 24) & 0xFF;
    code[12] = (entryPoint >> 32) & 0xFF;
    code[13] = (entryPoint >> 40) & 0xFF;
    code[14] = (entryPoint >> 48) & 0xFF;
    code[15] = (entryPoint >> 56) & 0xFF;
    
    for (uint64_t i = 0; i < (asm_size + HEADER_SIZE); i++) {
        code[i + HEADER_SIZE] = asm_data[i];
    }

    FILE* out = fopen(outFile, "wb");
    if (out == NULL) {
        native_error("Could not open file %s\n", outFile);
    }

    for (uint64_t i = 0; i < (asm_ptr + HEADER_SIZE); i++) {
        fprintf(out, "%c", code[i]);
    }
    
    #ifdef DEBUG
    string debug = (string) mallocOrErr(strlen(outFile) + 5);
    strcpy(debug, outFile);
    strcat(debug, ".dsym");
    FILE* dsym = fopen(debug, "w");
    if (dsym == NULL) {
        native_error("Could not open file %s\n", debug);
    }
    for (uint64_t i = 0; i < labelCount; i++) {
        fprintf(dsym, "%016llx:%s\n", labels[i].address, labels[i].label);
    }
    fclose(dsym);
    freeOrErr(debug);
    #endif

    fclose(out);
    freeOrErr(buffer);
    freeOrErr(outFile);
    freeOrErr(asm_data);

    return 0;
}

int main(int argc, string argv[])
{
    printf("Compile Addr: %p\n", &compile);
    printf("Runtime Addr: %p\n", &run);
    if (argc < 3) {
        error("Usage: %s <comp|run> <file>\n", argv[0]);
    }

    inputFileName = argv[2];
    inputFile = fopen(inputFileName, "r");

    if (strcmp(argv[1], "comp") == 0) {
        return compile();
    } else if (strcmp(argv[1], "run") == 0) {
        int r = run();
        if (r != 0) {
            me_heapDump("error.dump");
        }
        return r;
    } else {
        error("Usage: %s <comp|run> <file>\n", argv[0]);
    }
    return 0;
}

#ifdef __cplusplus
}
#endif
#endif // Windows
