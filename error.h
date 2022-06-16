#ifndef REX_ERROR_H
#define REX_ERROR_H

#include <stdio.h>

#define error(...)            fprintf(stderr, __VA_ARGS__)
#define native_error(...)     fprintf(stderr, "Native Error: " __VA_ARGS__)
#define usage_error(...)      fprintf(stderr, "Usage: " __VA_ARGS__)
#define heap_error(...)       fprintf(stderr, "Heap Error: " __VA_ARGS__)
#define io_error(...)         fprintf(stderr, "IO Error: " __VA_ARGS__)
#define system_error(...)     fprintf(stderr, "System Error: " __VA_ARGS__)
#define syntax_error(...)     fprintf(stderr, "Syntax Error: " __VA_ARGS__)

#define ERR_NATIVE            1
#define ERR_USAGE             2
#define ERR_INVALID_ADDRESS   3
#define ERR_RESERVED_ADDRESS  4
#define ERR_IO                5
#define ERR_INVALID_OPCODE    6
#define ERR_SYSTEM            7
#define ERR_SYNTAX            8

#endif // REX_ERROR_H