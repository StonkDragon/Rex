// check for c++
#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#define OS_WINDOWS
// #error "Rasm is currently not supported on Windows!"
#else

#define RASM

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "headers/cutil.h"
#include "headers/cstring.h"
#include "headers/register.h"
#include "headers/opcodes.h"
#include "headers/error.h"
#include "headers/crc32.h"
#include "headers/memoryengine.h"
#include "headers/rasm/binary.h"
#include "headers/rasm/asm.h"

int main(int argc, string argv[]) {
    if (argc != 2) {
        usage_error("%s <file>\n", argv[0]);
    }
    FILE* file = fopen(argv[1], "r");
    if (file == 0) {
        native_error("Could not open file %s\n", argv[1]);
    }
    string outFile = (string) mallocOrErr(strlen(argv[1]) - 3);
    strncpy(outFile, argv[1], strlen(argv[1]) - 5);
    strcat(outFile, ".rx");
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    string file_buffer = (string) mallocOrErr(size);
    fread(file_buffer, size, 1, file);
    fclose(file);

    string token = strtok(file_buffer, " \n");
    string token_buffer = (string) mallocOrErr(size);
    string buffer = (string) mallocOrErr(size);
    int i = 0;
    while (token != NULL) {
        if (strlen(token) > 0) {
            strcat(token_buffer, token);
            strcat(token_buffer, " ");
        }
        i++;
        token = NULL;
        token = strtok(NULL, " \n");
    }
    strcpy(buffer, token_buffer);

    labels = (rasm_label_t*) mallocOrErr(sizeof(rasm_label_t) * STACK_SIZE);

    bin_parseLabels(token_buffer, size);

    uint64_t asm_size = asm_writeData(buffer, size);

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
    
    string entryPouintLabel = bin_getEntryPouintLabel();

    uint64_t entryPoint = bin_getAddressOfLabel(entryPouintLabel);
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

    asm_finalize();

    return 0;
}

#ifdef __cplusplus
}
#endif
#endif // Windows
