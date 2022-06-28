#ifndef REX_ERROR_H
#define REX_ERROR_H

#include <stdio.h>

void    me_heapDump(const string dumpFile);

#ifndef RASM
#include "memoryengine.h"
#define error(...)            { fprintf(stderr, "%s:%u: ", __FILE__, __LINE__); fprintf(stderr,  __VA_ARGS__);                   fprintf(stderr, "Dumping Memory.\n"); me_heapDump("memory.dump"); exit(ERR_GENERIC); }
#define native_error(...)     { fprintf(stderr, "%s:%u: ", __FILE__, __LINE__); fprintf(stderr, "Native Error: " __VA_ARGS__);  fprintf(stderr, "Dumping Memory.\n"); me_heapDump("memory.dump"); exit(ERR_NATIVE); }
#define usage_error(...)      { fprintf(stderr, "%s:%u: ", __FILE__, __LINE__); fprintf(stderr, "Usage: " __VA_ARGS__);         fprintf(stderr, "Dumping Memory.\n"); me_heapDump("memory.dump"); exit(ERR_USAGE); }
#define io_error(...)         { fprintf(stderr, "%s:%u: ", __FILE__, __LINE__); fprintf(stderr, "IO Error: " __VA_ARGS__);      fprintf(stderr, "Dumping Memory.\n"); me_heapDump("memory.dump"); exit(ERR_IO); }
#define system_error(...)     { fprintf(stderr, "%s:%u: ", __FILE__, __LINE__); fprintf(stderr, "System Error: " __VA_ARGS__);  fprintf(stderr, "Dumping Memory.\n"); me_heapDump("memory.dump"); exit(ERR_SYSTEM); }
#define syntax_error(...)     { fprintf(stderr, "%s:%u: ", __FILE__, __LINE__); fprintf(stderr, "Syntax Error: " __VA_ARGS__);  fprintf(stderr, "Dumping Memory.\n"); me_heapDump("memory.dump"); exit(ERR_SYNTAX); }
#else
#define error(...)            { fprintf(stderr, "%s:%u: ", __FILE__, __LINE__); fprintf(stderr, __VA_ARGS__);                   exit(ERR_GENERIC); }
#define native_error(...)     { fprintf(stderr, "%s:%u: ", __FILE__, __LINE__); fprintf(stderr, "Native Error: " __VA_ARGS__);  exit(ERR_NATIVE); }
#define usage_error(...)      { fprintf(stderr, "%s:%u: ", __FILE__, __LINE__); fprintf(stderr, "Usage: " __VA_ARGS__);         exit(ERR_USAGE); }
#define io_error(...)         { fprintf(stderr, "%s:%u: ", __FILE__, __LINE__); fprintf(stderr, "IO Error: " __VA_ARGS__);      exit(ERR_IO); }
#define system_error(...)     { fprintf(stderr, "%s:%u: ", __FILE__, __LINE__); fprintf(stderr, "System Error: " __VA_ARGS__);  exit(ERR_SYSTEM); }
#define syntax_error(...)     { fprintf(stderr, "%s:%u: ", __FILE__, __LINE__); fprintf(stderr, "Syntax Error: " __VA_ARGS__);  exit(ERR_SYNTAX); }
#endif
#define warn(...)             { fprintf(stderr, "%s:%u: ", __FILE__, __LINE__); fprintf(stderr, "Warn: " __VA_ARGS__); }
#define TODO(...)             { printf("%s:%u:", __FILE__, __LINE__); printf("TODO: " __VA_ARGS__); }
#define stopUnusedWarn(var)   (void) var

#define ERR_GENERIC           1
#define ERR_NATIVE            2
#define ERR_USAGE             3
#define ERR_HEAP              4
#define ERR_IO                5
#define ERR_INVALID_OPCODE    6
#define ERR_SYSTEM            7
#define ERR_SYNTAX            8
#define ERR_STACK             9

void* mallocOrErr(size_t size) {
    void* ptr = malloc(size);
    if (ptr == NULL) {
        native_error("Not enough Memory to allocate %lu Bytes.\n", size);
    }
    return ptr;
}

#endif // REX_ERROR_H
