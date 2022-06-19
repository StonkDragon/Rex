#ifndef REX_ERROR_H
#define REX_ERROR_H

#include <stdio.h>

#ifndef RASM
#include "memoryengine.h"
#define error(...)            { fprintf(stderr, __VA_ARGS__);                   fprintf(stderr, "Dumping Memory.\n"); me_heapDump("memory.dump"); exit(ERR_GENERIC); }
#define native_error(...)     { fprintf(stderr, "Native Error: " __VA_ARGS__);  fprintf(stderr, "Dumping Memory.\n"); me_heapDump("memory.dump"); exit(ERR_NATIVE); }
#define usage_error(...)      { fprintf(stderr, "Usage: " __VA_ARGS__);         fprintf(stderr, "Dumping Memory.\n"); me_heapDump("memory.dump"); exit(ERR_USAGE); }
#define heap_error(...)       { fprintf(stderr, "Heap Error: " __VA_ARGS__);    fprintf(stderr, "Dumping Memory.\n"); me_heapDump("memory.dump"); exit(ERR_HEAP); }
#define io_error(...)         { fprintf(stderr, "IO Error: " __VA_ARGS__);      fprintf(stderr, "Dumping Memory.\n"); me_heapDump("memory.dump"); exit(ERR_IO); }
#define system_error(...)     { fprintf(stderr, "System Error: " __VA_ARGS__);  fprintf(stderr, "Dumping Memory.\n"); me_heapDump("memory.dump"); exit(ERR_SYSTEM); }
#define syntax_error(...)     { fprintf(stderr, "Syntax Error: " __VA_ARGS__);  fprintf(stderr, "Dumping Memory.\n"); me_heapDump("memory.dump"); exit(ERR_SYNTAX); }
#else
#define error(...)            { fprintf(stderr, __VA_ARGS__);                   exit(ERR_GENERIC); }
#define native_error(...)     { fprintf(stderr, "Native Error: " __VA_ARGS__);  exit(ERR_NATIVE); }
#define usage_error(...)      { fprintf(stderr, "Usage: " __VA_ARGS__);         exit(ERR_USAGE); }
#define heap_error(...)       { fprintf(stderr, "Heap Error: " __VA_ARGS__);    exit(ERR_HEAP); }
#define io_error(...)         { fprintf(stderr, "IO Error: " __VA_ARGS__);      exit(ERR_IO); }
#define system_error(...)     { fprintf(stderr, "System Error: " __VA_ARGS__);  exit(ERR_SYSTEM); }
#define syntax_error(...)     { fprintf(stderr, "Syntax Error: " __VA_ARGS__);  exit(ERR_SYNTAX); }
#endif
#define warn(...)             { fprintf(stderr, "Warn: " __VA_ARGS__); }

#define ERR_GENERIC           1
#define ERR_NATIVE            2
#define ERR_USAGE             3
#define ERR_HEAP              4
#define ERR_IO                5
#define ERR_INVALID_OPCODE    6
#define ERR_SYSTEM            7
#define ERR_SYNTAX            8
#define ERR_STACK             9

#endif // REX_ERROR_H