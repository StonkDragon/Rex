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

#include "headers/opcodes.h"
#include "headers/rexcall.h"
#include "headers/register.h"
#include "headers/error.h"
#include "headers/crc32.h"
#include "headers/memoryengine.h"

char*    buffer;
FILE*    openFiles[STACK_SIZE];
int      filePointer = 0;

int run(int argc, char* argv[]) {
    if (argc < 2) {
        error("Usage: %s <file>\n", argv[0]);
    }
    FILE *file = fopen(argv[1], "rb");
    
    if (file == NULL) {
        native_error("Error opening file \"%s\"\n", argv[1]);
    }

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    buffer = (char*)malloc(size);
    fread(buffer, size, 1, file);
    fclose(file);
    uint8_t* crcData = (uint8_t*)malloc(size - HEADER_SIZE);
    memcpy(crcData, buffer + HEADER_SIZE, size - HEADER_SIZE);
    
    uint32_t crc = rex_crc32(crcData, size - HEADER_SIZE);
    free(crcData);

    uint32_t identifier = buffer[0] & 0xFF | (buffer[1] & 0xFF) << 8 | (buffer[2] & 0xFF) << 16 | (buffer[3] & 0xFF) << 24;
    if (identifier != FILE_IDENTIFIER) {
        native_error("Invalid file\n");
    }

    uint32_t dataCRC = buffer[4] & 0xFF
        | (buffer[5] & 0xFF) << 8
        | (buffer[6] & 0xFF) << 16
        | (buffer[7] & 0xFF) << 24;
    
    if (crc != dataCRC) {
        native_error("CRC mismatch\n");
    }

    uint32_t _main = buffer[8] & 0xFF
        | (buffer[9] & 0xFF) << 8
        | (buffer[10] & 0xFF) << 16
        | (buffer[11] & 0xFF) << 24;

    for (int i = HEADER_SIZE; i < size; i++) {
        me_writeByte(i - HEADER_SIZE, buffer[i]);
    }

    ip = _main;

    while (ip < size) {
        uint8_t opcode = me_readByte(ip);
        addr = me_readInt(ip + 1);


        switch (opcode) {
            #ifdef DEBUG
            case BREAKPOINT:
                printf("Debug: Hit Breakpoint\n");
                me_heapDump();
                getchar();
                break;
            #endif

            case NOP:
                break;

            case PUSH:
                me_push(addr);
                ip += 4;
                break;

            case STORE:
                {
                    ip++;
                    addr = me_readInt(ip);
                    if (addr > HEAP_MAX || addr < 0) {
                        heap_error("Attempted to write outside of memory space (Address: %02x)\n", addr);
                        me_heapDump();
                    }
                    if (addr <= size) {
                        heap_error("Attempted to write to reserved memory space (Address: %02x)\n", addr);
                        me_heapDump();
                    }
                    ip += 4;
                    uint32_t value = re_get(me_readByte(ip));
                    me_writeInt(addr, value);
                }
                break;

            case LOAD:
                ip++;
                addr = me_readInt(ip + 1);
                re_set(me_readByte(ip), heapAddress);
                ip += 4;
                break;

            case LOAD_IMM:
                ip++;
                addr = me_readInt(ip + 1);
                re_set(me_readByte(ip), addr);
                ip += 4;
                break;

            case MOVE:
                re_set(me_readByte(++ip), re_get(me_readByte(++ip)));
                break;

            case POP:
                re_set(me_readByte(++ip), me_pop());
                break;

            case DUP:
                r15 = me_pop();
                me_push(r15);
                me_push(r15);
                break;

            case SWAP:
                r14 = me_pop();
                r15 = me_pop();
                me_push(r14);
                me_push(r15);
                break;

            case IADD:
                r0 += re_get(me_readByte(++ip));
                break;

            case ISUB:
                r0 -= re_get(me_readByte(++ip));
                break;

            case IMUL:
                r0 *= re_get(me_readByte(++ip));
                break;

            case IDIV:
                r0 /= re_get(me_readByte(++ip));
                break;

            case IREM:
                r0 %= re_get(me_readByte(++ip));
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
                r0 &= re_get(me_readByte(++ip));
                break;

            case IOR:
                r0 |= re_get(me_readByte(++ip));
                break;

            case IXOR:
                r0 ^= re_get(me_readByte(++ip));
                break;

            case ISHL:
                r0 <<= re_get(me_readByte(++ip));
                break;

            case ISHR:
                r0 >>= re_get(me_readByte(++ip));
                break;

            case INOT:
                r0 = ~r0;
                break;

            case CMP:
                {
                    uint32_t value = re_get(me_readByte(++ip));
                    if (r0 == value) {
                        rFlags = rFlags | FLAG_EQUAL;
                        rFlags = rFlags & ~FLAG_LESS;
                        rFlags = rFlags & ~FLAG_GREATER;
                    } else if (r0 < value) {
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
                }
                break;

            case GOTO:
                ip = --addr;
                break;

            case IF_EQ:
                ip += 4;
                if ((rFlags & FLAG_EQUAL)) {
                    me_push(ip);
                    ip = --addr;
                }
                break;

            case IF_NE:
                ip += 4;
                if ((rFlags & FLAG_EQUAL) == 0) {
                    me_push(ip);
                    ip = --addr;
                }
                break;

            case IF_LT:
                ip += 4;
                if ((rFlags & FLAG_LESS)) {
                    me_push(ip);
                    ip = --addr;
                }
                break;

            case IF_GE:
                ip += 4;
                if ((rFlags & FLAG_LESS) == 0) {
                    me_push(ip);
                    ip = --addr;
                }
                break;

            case IF_GT:
                ip += 4;
                if ((rFlags & FLAG_GREATER)) {
                    me_push(ip);
                    ip = --addr;
                }
                break;

            case IF_LE:
                ip += 4;
                if ((rFlags & FLAG_GREATER) == 0) {
                    me_push(ip);
                    ip = --addr;
                }
                break;

            case SYSTEM:
                {
                    switch (re_get(0xF))
                    {
                        case SC_EXIT:
                            #ifdef DEBUG
                            printf("\n");
                            printf("Debug: ");
                            me_heapDump();
                            printf("Exited with exit code %d (0x%08x)\n", re_get(0), re_get(0));
                            #endif
                            {
                                int ret = re_get(0);
                                return ret;
                            }

                        case SC_READ_CON:
                            {
                                addr = re_get(0);
                                if (addr > HEAP_MAX || addr < 0) {
                                    heap_error("Attempted to write outside of memory space (Address: %02x)\n", addr);
                                    me_heapDump();
                                }
                                if (addr <= size) {
                                    heap_error("Attempted to write to reserved memory space (Address: %02x)\n", addr);
                                    me_heapDump();
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
                                    me_writeByte(addr + i, line[i]);
                                }
                            }
                            break;

                        case SC_WRITE_CON:
                            {
                                char* str = me_readString(re_get(0));
                                printf("%s", str);
                                free(str);
                            }
                            break;

                        case SC_SLEEP:
                            sleep(re_get(0));
                            break;

                        case SC_OPEN:
                            {
                                char* str = me_readString(re_get(0));
                                int mode = re_get(1);
                                if (mode != 0 && mode != 1) {
                                    system_error("Invalid mode for open\n");
                                }
                                FILE* f = fopen(str, mode == 0 ? "r" : "w");
                                openFiles[filePointer++] = f;
                                if (f == NULL) {
                                    system_error("IO: Could not open file \"%s\"\n", str);
                                }
                                free(str);
                                re_set(r0, filePointer-1);
                            }
                            break;

                        case SC_CLOSE:
                            fclose(openFiles[re_get(0)]);
                            break;

                        case SC_READ:
                            {
                                char* str = NULL;
                                int fd = re_get(0);
                                int len = re_get(1);
                                addr = re_get(2);
                                char* buf = (char*)malloc(len);
                                error("File IO is not implemented yet\n");
                                free(buf);
                                break;
                                fread(buf, len, 1, openFiles[fd]);
                                for (int i = 0; i < len; i++) {
                                    me_writeByte(addr + i, buf[i]);
                                }
                                free(buf);
                                free(str);
                            }
                            break;

                        case SC_WRITE:
                            {
                                int fd = re_get(0);
                                int len = re_get(1);
                                addr = re_get(2);

                                char* buf = (char*)malloc(len + 1);
                                strncpy(buf, me_readString(addr), len);
                                buf[len] = '\0';
                                error("File IO is not implemented yet\n");
                                free(buf);
                                break;
                                fprintf(openFiles[fd], "%s", buf);
                                free(buf);
                            }
                            break;

                        case SC_EXEC:
                            {
                                char* str = me_readString(re_get(0));
                                int r = system(str);
                                free(str);
                                if (r < 0) {
                                    system_error("Failed to execute program\n");
                                }
                                me_push(r);
                            }
                            break;
                    }
                }
                break;

            case IF_TRUE:
                ip += 4;
                me_push(ip);
                ip = --addr;
                break;

            case IF_NULL:
                ip += 4;
                if ((rFlags & FLAG_ZERO)) {
                    me_push(ip);
                    ip = --addr;
                }
                break;

            case IF_NOTNULL:
                ip += 4;
                if ((rFlags & FLAG_ZERO) == 0) {
                    me_push(ip);
                    ip = --addr;
                }
                break;

            case RETURN:
                ip = me_pop() - 1;
                break;

            default:
                error("Error: Unknown opcode 0x%02x\n", opcode);
            #ifdef DEBUG
                me_heapDump();
            #endif
                break;
        }
        ip++;
    }
    #ifdef DEBUG
    me_heapDump();
    #endif
    return 0;
}

int main(int argc, char* argv[]) {
    int r = run(argc, argv);
    if (r != 0) {
        me_heapDump();
    }
    return r;
}

#ifdef __cplusplus
}
#endif