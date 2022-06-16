#ifndef REX_ERROR_H
#define REX_ERROR_H

#include <stdio.h>

#define error(...) fprintf(stderr, __VA_ARGS__)

#define ERR_NATIVE            1
#define ERR_USAGE             2
#define ERR_INVALID_ADDRESS   3
#define ERR_RESERVED_ADDRESS  4
#define ERR_IO                5
#define ERR_INVALID_OPCODE    6
#define ERR_SYSTEM            7
#define ERR_SYNTAX            8

#endif // REX_ERROR_H