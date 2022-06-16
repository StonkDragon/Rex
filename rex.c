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
#include <errno.h>

#include "opcodes.h"
#include "rexcall.h"
#include "register.h"
#include "error.h"
#include "rutil.c"

char*    buffer;
char*    getString(uint32_t str_addr);
FILE*    files[STACK_SIZE];
int      fp = 0;

#ifdef DEBUG
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

void dumpHeap() {
    printf("Heap Dump:\n");
    for (int i = 0; i < HEAP_SIZE; i++) {
        if (memGetByte(i] != 0) {
            printf("%d: %u\n", i, memGetByte(i]);
        }
    }
    printf("\n");
    printf("Registers:\n");
    printf("r0: %u\n", r0);
    printf("r1: %u\n", r1);
    printf("r2: %u\n", r2);
    printf("r3: %u\n", r3);
    printf("r4: %u\n", r4);
    printf("r5: %u\n", r5);
    printf("r6: %u\n", r6);
    printf("r7: %u\n", r7);
    printf("r8: %u\n", r8);
    printf("r9: %u\n", r9);
    printf("r10: %u\n", r10);
    printf("r11: %u\n", r11);
    printf("r12: %u\n", r12);
    printf("r13: %u\n", r13);
    printf("r14: %u\n", r14);
    printf("r15: %u\n", r15);
    printf("ip: %u\n", ip);
    printf("sp: %u\n", sp);
    printf("rFlags: "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(rFlags));
}

#endif
int main(int argc, char* argv[]) {
    if (argc < 2) {
        error("Usage: %s <file>\n", argv[0]);
        return 1;
    }
    FILE *file = fopen(argv[1], "rb");
    
    if (file == NULL) {
        error("Native Error: Error opening file \"%s\"\n", argv[1]);
        return ERR_NATIVE;
    }

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    buffer = (char*)malloc(size);
    fread(buffer, size, 1, file);
    fclose(file);
    char* crcData = (char*)malloc(size - HEADER_SIZE);
    memcpy(crcData, buffer + HEADER_SIZE, size - HEADER_SIZE);
    
    uint32_t crc = crc32(crcData, size - HEADER_SIZE);
    free(crcData);

    uint32_t identifier = buffer[0] & 0xFF | (buffer[1] & 0xFF) << 8 | (buffer[2] & 0xFF) << 16 | (buffer[3] & 0xFF) << 24;
    if (identifier != HEADER) {
        error("Native Error: Invalid file\n");
        return ERR_NATIVE;
    }

    uint32_t dataCRC = buffer[4] & 0xFF
        | (buffer[5] & 0xFF) << 8
        | (buffer[6] & 0xFF) << 16
        | (buffer[7] & 0xFF) << 24;
    
    if (crc != dataCRC) {
        error("Native Error: CRC mismatch\n");
        return ERR_NATIVE;
    }

    uint32_t _main = buffer[8] & 0xFF
        | (buffer[9] & 0xFF) << 8
        | (buffer[10] & 0xFF) << 16
        | (buffer[11] & 0xFF) << 24;

    for (int i = HEADER_SIZE; i < size; i++) {
        memWriteByte(i - HEADER_SIZE, buffer[i]);
    }

    ip = _main;

    while (ip < size) {
        uint8_t opcode = memGetByte(ip);
        addr = memGetInt(ip + 1);

        #ifdef DEBUG
        printf("Debug: 0x%x\n", opcode);
        #endif

        switch (opcode) {
            #ifdef DEBUG
            case BREAKPOINT:
                error("Debug: Hit Breakpoint\n");
                getchar();
                break;
            #endif

            case NOP:
                break;

            case PUSH:
                push(addr);
                ip += 4;
                break;

            case STORE:
                ip++;
                addr = memGetInt(ip);
                if (addr > HEAP_MAX || addr < 0) {
                    error("Heap Error: Attempted to write outside of memory space (Address: %02x)\n", addr);
                    return ERR_INVALID_ADDRESS;
                }
                if (addr <= size) {
                    error("Heap Error: Attempted to write to reserved memory space\n");
                    return ERR_RESERVED_ADDRESS;
                }
                uint32_t value = getRegister(memGetByte(ip));
                memWriteInt(addr, value);
                ip += 4;
                break;

            case LOAD:
                ip++;
                addr = memGetInt(ip + 1);
                setRegister(memGetByte(ip), heapAddress);
                ip += 4;
                break;

            case LOAD_IMM:
                ip++;
                addr = memGetInt(ip + 1);
                setRegister(memGetByte(ip), addr);
                ip += 4;
                break;

            case MOVE:
                setRegister(memGetByte(++ip), getRegister(memGetByte(++ip)));
                break;

            case POP:
                setRegister(memGetByte(++ip), pop());
                break;

            case DUP:
                r15 = pop();
                push(r15);
                push(r15);
                break;

            case SWAP:
                r14 = pop();
                r15 = pop();
                push(r14);
                push(r15);
                break;

            case IADD:
                r0 += getRegister(memGetByte(++ip));
                break;

            case ISUB:
                r0 -= getRegister(memGetByte(++ip));
                break;

            case IMUL:
                r0 *= getRegister(memGetByte(++ip));
                break;

            case IDIV:
                r0 /= getRegister(memGetByte(++ip));
                break;

            case IREM:
                r0 %= getRegister(memGetByte(++ip));
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
                r0 &= getRegister(memGetByte(++ip));
                break;

            case IOR:
                r0 |= getRegister(memGetByte(++ip));
                break;

            case IXOR:
                r0 ^= getRegister(memGetByte(++ip));
                break;

            case ISHL:
                r0 <<= getRegister(memGetByte(++ip));
                break;

            case ISHR:
                r0 >>= getRegister(memGetByte(++ip));
                break;

            case INOT:
                r0 = ~r0;
                break;

            case CMP:
                if (r0 == getRegister(memGetByte(++ip))) {
                    rFlags = rFlags | FLAG_EQUAL;
                    rFlags = rFlags & ~FLAG_LESS;
                    rFlags = rFlags & ~FLAG_GREATER;
                } else if (r0 < getRegister(memGetByte(++ip))) {
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
                if ((rFlags & FLAG_EQUAL)) {
                    ip += 4;
                    push(ip);
                    ip = --addr;
                }
                break;

            case IF_NE:
                if ((rFlags & FLAG_EQUAL) == 0) {
                    ip += 4;
                    push(ip);
                    ip = --addr;
                }
                break;

            case IF_LT:
                if ((rFlags & FLAG_LESS)) {
                    ip += 4;
                    push(ip);
                    ip = --addr;
                }
                break;

            case IF_GE:
                if ((rFlags & FLAG_LESS) == 0) {
                    ip += 4;
                    push(ip);
                    ip = --addr;
                }
                break;

            case IF_GT:
                if ((rFlags & FLAG_GREATER)) {
                    ip += 4;
                    push(ip);
                    ip = --addr;
                }
                break;

            case IF_LE:
                if ((rFlags & FLAG_GREATER) == 0) {
                    ip += 4;
                    push(ip);
                    ip = --addr;
                }
                break;

            case SYSTEM:
                {
                    switch (getRegister(0xF))
                    {
                        case SC_EXIT:
                            #ifdef DEBUG
                            dumpHeap();
                            printf("Exiting with code %d\n", getRegister(0));
                            #endif
                            {
                                int ret = getRegister(0);
                                return ret;
                            }

                        case SC_READ_CON:
                            {
                                addr = getRegister(0);
                                if (addr > HEAP_MAX || addr < 0) {
                                    error("Heap Error: Attempted to write outside of memory space (Address: %02x)\n", addr);
                                    return ERR_INVALID_ADDRESS;
                                }
                                if (addr <= size) {
                                    error("Heap Error: Attempted to write to reserved memory space\n");
                                    return ERR_RESERVED_ADDRESS;
                                }
                                char *line = NULL;
                                size_t len = 0;
                                ssize_t lineLen = 0;
                                lineLen = getline(&line, &len, stdin);
                                line[strlen(line) - 1] = '\0';
                                
                                #ifdef DEBUG
                                printf("Entered String: \"%s\"\n", line);
                                #endif

                                for (int i = 0; i < strlen(line); i++) {
                                    memWriteByte(addr + i, line[i]);
                                }
                            }
                            break;

                        case SC_WRITE_CON:
                            {
                                char* str = getString(getRegister(0));
                                printf("%s", str);
                                free(str);
                            }
                            break;

                        case SC_SLEEP:
                            sleep(getRegister(0));
                            break;

                        case SC_OPEN:
                            {
                                char* str = getString(getRegister(0));
                                int mode = getRegister(1);
                                if (mode != 0 && mode != 1) {
                                    error("Heap Error: Invalid mode for open\n");
                                    return ERR_IO;
                                }
                                FILE* f = fopen(str, mode == 0 ? "r" : "w");
                                files[fp++] = f;
                                if (f == NULL) {
                                    error("File Error: Could not open file \"%s\"\n", str);
                                    return ERR_IO;
                                }
                                free(str);
                                setRegister(r0, fp-1);
                            }
                            break;

                        case SC_CLOSE:
                            fclose(files[getRegister(0)]);
                            break;

                        case SC_READ:
                            {
                                char* str = NULL;
                                int fd = getRegister(0);
                                int len = getRegister(1);
                                addr = getRegister(2);
                                char* buf = malloc(len);
                                error("File IO is not implemented yet\n");
                                free(buf);
                                break;
                                fread(buf, len, 1, files[fd]);
                                for (int i = 0; i < len; i++) {
                                    memWriteByte(addr + i, buf[i]);
                                }
                                free(buf);
                                free(str);
                            }
                            break;

                        case SC_WRITE:
                            {
                                int fd = getRegister(0);
                                int len = getRegister(1);
                                addr = getRegister(2);

                                char* buf = malloc(len + 1);
                                strncpy(buf, getString(addr), len);
                                buf[len] = '\0';
                                error("File IO is not implemented yet\n");
                                free(buf);
                                break;
                                fprintf(files[fd], "%s", buf);
                                free(buf);
                            }
                            break;

                        case SC_EXEC:
                            {
                                char* str = getString(getRegister(0));
                                int r = system(str);
                                free(str);
                                if (r < 0) {
                                    error("Error: Failed to execute program\n");
                                    return ERR_SYSTEM;
                                }
                                push(r);
                            }
                            break;
                    }
                }
                break;

            case IF_TRUE:
                ip += 4;
                push(ip);
                ip = --addr;
                break;

            case IF_NULL:
                if (stack[sp - 1] == 0) {
                    ip += 4;
                    push(ip);
                    ip = --addr;
                }
                break;

            case IF_NOTNULL:
                if (stack[sp - 1] != 0) {
                    ip += 4;
                    push(ip);
                    ip = --addr;
                }
                break;

            case RETURN:
                ip = --pop();
                break;

            default:
                error("Error: Unknown opcode 0x%02x\n", opcode);
                return ERR_INVALID_OPCODE;
                break;
        }
        ip++;
    }
    #ifdef DEBUG
    dumpHeap();
    #endif
    return 0;
}

char* getString(uint32_t str_addr) {
    return memGetString(str_addr);
}

#ifdef __cplusplus
}
#endif