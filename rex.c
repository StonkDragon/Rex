// check for c++
#ifdef __cplusplus
extern "C" {
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

#include "opcodes.h"
#include "rexcall.h"

// __stderrp doesn't exist on all systems
#ifndef __stderrp
#define __stderrp stderr
#endif

#define FLAG_ZERO       0b00000001
#define FLAG_NEG        0b00000010
#define FLAG_EQUAL      0b00000100
#define FLAG_LESS       0b00001000
#define FLAG_GREATER    0b00010000
#define FLAG_UNUSED_1   0b00100000
#define FLAG_UNUSED_2   0b01000000
#define FLAG_UNUSED_3   0b10000000

#define STACK_SIZE      256
#define HEAP_SIZE       65536

uint32_t getRegister(uint32_t reg);
void     setRegister(uint32_t reg, uint32_t val);
char*    getString(uint32_t addr);

uint32_t          stack[STACK_SIZE];
uint32_t          heap[HEAP_SIZE];
char*             buffer;

#define HEAP_MAX  HEAP_SIZE-1
#define r0        heap[HEAP_MAX]
#define r1        heap[HEAP_MAX - 1]
#define r2        heap[HEAP_MAX - 2]
#define r3        heap[HEAP_MAX - 3]
#define r4        heap[HEAP_MAX - 4]
#define r5        heap[HEAP_MAX - 5]
#define r6        heap[HEAP_MAX - 6]
#define r7        heap[HEAP_MAX - 7]
#define r8        heap[HEAP_MAX - 8]
#define r9        heap[HEAP_MAX - 9]
#define r10       heap[HEAP_MAX - 10]
#define r11       heap[HEAP_MAX - 11]
#define r12       heap[HEAP_MAX - 12]
#define r13       heap[HEAP_MAX - 13]
#define r14       heap[HEAP_MAX - 14]
#define r15       heap[HEAP_MAX - 15]
#define ip        heap[HEAP_MAX - 16]
#define sp        heap[HEAP_MAX - 17]
#define rFlags    heap[HEAP_MAX - 18]
#define addr      heap[HEAP_MAX - 19]

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }
    FILE *file = fopen(argv[1], "rb");
    
    if (file == NULL) {
        fprintf(__stderrp, "Error: Error opening file \"%s\"\n", argv[1]);
        return 1;
    }

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    buffer = (char*) malloc(size);
    fread(buffer, size, 1, file);
    fclose(file);

    while (ip < size) {
        uint8_t opcode = buffer[ip];
        addr = buffer[ip + 1] |
            (buffer[ip + 2] << 8) |
            (buffer[ip + 3] << 16) |
            (buffer[ip + 4] << 24);

        #ifdef DEBUG
        printf("Debug: 0x%x\n", opcode);
        #endif

        switch (opcode) {
            #ifdef DEBUG
            case BREAKPOINT:
                fprintf(__stderrp, "Debug: Hit Breakpoint\n");
                getchar();
                break;
            #endif

            case NOP:
                break;

            case PUSH:
                stack[sp++] = addr;
                ip += 4;
                break;

            case STORE:
                ip++;
                addr = buffer[ip + 1] |
                    (buffer[ip + 2] << 8) |
                    (buffer[ip + 3] << 16) |
                    (buffer[ip + 4] << 24);
                if (addr > HEAP_MAX || addr < 0) {
                    fprintf(__stderrp, "Error: Attempted to store outside of heap\n");
                    return -1;
                }
                if (addr >= HEAP_MAX - 19) {
                    fprintf(__stderrp, "Error: Attempted to store in reserved memory space\n");
                    return -1;
                }
                heap[addr] = getRegister(buffer[ip]);
                ip += 4;
                break;

            case LOAD:
                ip++;
                addr = buffer[ip + 1] |
                    (buffer[ip + 2] << 8) |
                    (buffer[ip + 3] << 16) |
                    (buffer[ip + 4] << 24);
                setRegister(buffer[ip], heap[addr]);
                ip += 4;
                break;

            case LOAD_IMM:
                ip++;
                addr = buffer[ip + 1] |
                    (buffer[ip + 2] << 8) |
                    (buffer[ip + 3] << 16) |
                    (buffer[ip + 4] << 24);
                setRegister(buffer[ip], addr);
                ip += 4;
                break;

            case MOVE:
                setRegister(buffer[++ip], getRegister(buffer[++ip]));
                break;

            case POP:
                setRegister(buffer[++ip], stack[--sp]);
                break;

            case DUP:
                r15 = stack[sp - 1];
                stack[sp++] = r15;
                stack[sp++] = r15;
                break;

            case SWAP:
                r14 = stack[--sp];
                r15 = stack[--sp];
                stack[sp++] = r14;
                stack[sp++] = r15;
                break;

            case IADD:
                r0 += getRegister(buffer[++ip]);
                break;

            case ISUB:
                r0 -= getRegister(buffer[++ip]);
                break;

            case IMUL:
                r0 *= getRegister(buffer[++ip]);
                break;

            case IDIV:
                r0 /= getRegister(buffer[++ip]);
                break;

            case IREM:
                r0 %= getRegister(buffer[++ip]);
                break;

            case INEG:
                r0 = -r0;
                break;

            case IINC:
                r0++;
                break;

            case IDEC:
                r0--;
                break;

            case IAND:
                r0 &= getRegister(buffer[++ip]);
                break;

            case IOR:
                r0 |= getRegister(buffer[++ip]);
                break;

            case IXOR:
                r0 ^= getRegister(buffer[++ip]);
                break;

            case ISHL:
                r0 <<= getRegister(buffer[++ip]);
                break;

            case ISHR:
                r0 >>= getRegister(buffer[++ip]);
                break;

            case INOT:
                r0 = ~r0;
                break;

            case CMP:
                if (r0 == getRegister(buffer[++ip])) {
                    rFlags = rFlags | FLAG_EQUAL;
                    rFlags = rFlags & ~FLAG_LESS;
                    rFlags = rFlags & ~FLAG_GREATER;
                } else if (r0 < getRegister(buffer[++ip])) {
                    rFlags = rFlags | FLAG_LESS;
                    rFlags = rFlags & ~FLAG_EQUAL;
                    rFlags = rFlags & ~FLAG_GREATER;
                } else {
                    rFlags = rFlags | FLAG_GREATER;
                    rFlags = rFlags & ~FLAG_EQUAL;
                    rFlags = rFlags & ~FLAG_LESS;
                }
                if (r0 == 0) {
                    rFlags = rFlags | FLAG_ZERO;
                } else {
                    rFlags = rFlags & ~FLAG_ZERO;
                }
                break;

            case GOTO:
                ip = --addr;
                break;

            case IF_EQ:
                if ((rFlags & FLAG_EQUAL) != 0) {
                    ip += 4;
                    stack[sp++] = ip;
                    ip = --addr;
                }
                break;

            case IF_NE:
                if ((rFlags & FLAG_EQUAL) == 0) {
                    ip += 4;
                    stack[sp++] = ip;
                    ip = --addr;
                }
                break;

            case IF_LT:
                if ((rFlags & FLAG_LESS) != 0) {
                    ip += 4;
                    stack[sp++] = ip;
                    ip = --addr;
                }
                break;

            case IF_GE:
                if ((rFlags & FLAG_LESS) == 0) {
                    ip += 4;
                    stack[sp++] = ip;
                    ip = --addr;
                }
                break;

            case IF_GT:
                if ((rFlags & FLAG_GREATER) != 0) {
                    ip += 4;
                    stack[sp++] = ip;
                    ip = --addr;
                }
                break;

            case IF_LE:
                if ((rFlags & FLAG_GREATER) == 0) {
                    ip += 4;
                    stack[sp++] = ip;
                    ip = --addr;
                }
                break;

            case SYSTEM:
                {
                    char* str = getString(getRegister(0));
                    switch (getRegister(0xF))
                    {
                        case SC_EXIT:
                            exit(getRegister(0));
                            break;

                        case SC_READ_CON:
                            r1 = getchar();
                            break;

                        case SC_WRITE_CON:
                            printf("%s", str);
                            break;

                        case SC_SLEEP:
                            sleep(getRegister(0));
                            break;
                    }
                }
                break;

            case CALL:
                stack[sp++] = ip;
                ip = --addr;
                break;

            case IF_NULL:
                if (stack[sp - 1] == 0) {
                    stack[sp++] = ip;
                    ip = --addr;
                }
                break;

            case IF_NOTNULL:
                if (stack[sp - 1] != 0) {
                    stack[sp++] = ip;
                    ip = --addr;
                }
                break;

            case RETURN:
                ip = --stack[--sp];
                break;

            default:
                fprintf(__stderrp, "Error: Unknown opcode 0x%02x\n", opcode);
                return -1;
                break;
        }
        ip++;
    }
    return 0;
}

uint32_t getRegister(uint32_t reg) {
    switch (reg) {
        case 0: return r0;
        case 1: return r1;
        case 2: return r2;
        case 3: return r3;
        case 4: return r4;
        case 5: return r5;
        case 6: return r6;
        case 7: return r7;
        case 8: return r8;
        case 9: return r9;
        case 10: return r10;
        case 11: return r11;
        case 12: return r12;
        case 13: return r13;
        case 14: return r14;
        case 15: return r15;
    }
    return 0;
}

void setRegister(uint32_t reg, uint32_t val) {
    switch (reg) {
        case 0: r0 = val;
        case 1: r1 = val;
        case 2: r2 = val;
        case 3: r3 = val;
        case 4: r4 = val;
        case 5: r5 = val;
        case 6: r6 = val;
        case 7: r7 = val;
        case 8: r8 = val;
        case 9: r9 = val;
        case 10: r10 = val;
        case 11: r11 = val;
        case 12: r12 = val;
        case 13: r13 = val;
        case 14: r14 = val;
        case 15: r15 = val;
    }
}

char* getString(uint32_t str_addr) {
    char* str = (char*)malloc(sizeof(char) * (strlen(buffer + str_addr) + 1));
    strcpy(str, buffer + str_addr);
    #ifdef DEBUG
    printf("Read String \"%s\" from buffer\n", str);
    #endif
    return str;
}

#ifdef __cplusplus
}
#endif