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

#include "headers/cstring.h"
#include "headers/opcodes.h"
#include "headers/rexcall.h"
#include "headers/register.h"
#include "headers/error.h"
#include "headers/crc32.h"
#include "headers/memoryengine.h"

string   buffer;
FILE*    openFiles[STACK_SIZE];
int      filePointer = 0;

int run(int argc, string argv[]) {
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
    
    buffer = (string)malloc(size);
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

    uint32_t version = buffer[12] & 0xFF
        | (buffer[13] & 0xFF) << 8
        | (buffer[14] & 0xFF) << 16
        | (buffer[15] & 0xFF) << 24;
    
    if (version > REX_COMPILER_VER) {
        native_error("The Program was compiled with a more recent Version.\nPlease update to Version v%d or higher to run this executable.\nCurrent version: v%d\n", version, REX_COMPILER_VER);
    }
    if (version < REX_COMPILER_VER) {
        warn("The Program was compiled with a older Version. (Binary: v%d, Runtime: v%d)\nSome features may not work as advertised!\n", version, REX_COMPILER_VER);
    }

    for (int i = HEADER_SIZE; i < size; i++) {
        me_writeByte(i - HEADER_SIZE, buffer[i]);
    }

    ip = _main;

    uint32_t bpCounter = 0;

    while (ip < size) {
        uint32_t operand = me_readInt(ip);
        uint8_t opcode = operand & 0xFF;
        uint8_t reg1 = (operand >> 8) & 0xFF;
        uint8_t reg2 = (operand >> 16) & 0xFF;
        uint8_t reg3 = (operand >> 24) & 0xFF;
        ip += 4;
        addr = me_readInt(ip);
        ip += 4;

        switch (opcode) {
            case BREAKPOINT:
            #ifdef DEBUG
                {
                    printf("Debug: Hit Breakpoint\n");
                    string bp = (string)malloc(sizeof(char) * MAX_STRING_LENGTH);
                    sprintf(bp, "bp-%d.dump", ++bpCounter);
                    me_heapDump(bp);
                    getchar();
                }
            #endif
                break;

            case NOP:
                break;

            case PUSH:
                me_push(re_get(reg1));
                break;

            case PUSHI:
                me_push(addr);
                break;

            case STORE:
                {
                    if (addr > HEAP_MAX || addr < 0) {
                        heap_error("Attempted to write outside of memory space (Address: %02x)\n", addr);
                        me_heapDump("memory.dump");
                    }
                    if (addr <= size) {
                        heap_error("Attempted to write to reserved memory space (Address: %02x)\n", addr);
                        me_heapDump("memory.dump");
                    }
                    uint32_t value = re_get(reg1);
                    me_writeInt(addr, value);
                }
                break;

            case LOAD:
                re_set(reg1, heapAddress);
                break;

            case LOAD_IMM:
                re_set(reg1, addr);
                break;

            case MOVE:
                re_set(reg1, re_get(reg2));
                break;

            case POP:
                re_set(reg1, me_pop());
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
                r0 += re_get(reg1);
                break;

            case ISUB:
                r0 -= re_get(reg1);
                break;

            case IMUL:
                r0 *= re_get(reg1);
                break;

            case IDIV:
                r0 /= re_get(reg1);
                break;

            case IREM:
                r0 %= re_get(reg1);
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
                r0 &= re_get(reg1);
                break;

            case IOR:
                r0 |= re_get(reg1);
                break;

            case IXOR:
                r0 ^= re_get(reg1);
                break;

            case ISHL:
                r0 <<= re_get(reg1);
                break;

            case ISHR:
                r0 >>= re_get(reg1);
                break;

            case INOT:
                r0 = ~r0;
                break;

            case CMP:
                {
                    uint32_t value = re_get(reg1);
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
                ip = addr;
                break;

            case IF_EQ:
                ip += 4;
                if ((rFlags & FLAG_EQUAL)) {
                    me_push(ip);
                    ip = addr;
                }
                break;

            case IF_NE:
                ip += 4;
                if ((rFlags & FLAG_EQUAL) == 0) {
                    me_push(ip);
                    ip = addr;
                }
                break;

            case IF_LT:
                ip += 4;
                if ((rFlags & FLAG_LESS)) {
                    me_push(ip);
                    ip = addr;
                }
                break;

            case IF_GE:
                ip += 4;
                if ((rFlags & FLAG_LESS) == 0) {
                    me_push(ip);
                    ip = addr;
                }
                break;

            case IF_GT:
                ip += 4;
                if ((rFlags & FLAG_GREATER)) {
                    me_push(ip);
                    ip = addr;
                }
                break;

            case IF_LE:
                ip += 4;
                if ((rFlags & FLAG_GREATER) == 0) {
                    me_push(ip);
                    ip = addr;
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
                            me_heapDump("memory.dump");
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
                                    me_heapDump("memory.dump");
                                }
                                if (addr <= size) {
                                    heap_error("Attempted to write to reserved memory space (Address: %02x)\n", addr);
                                    me_heapDump("memory.dump");
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
                                string str = me_readString(re_get(0));
                                printf("%s", str);
                                free(str);
                            }
                            break;

                        case SC_SLEEP:
                            sleep(re_get(0));
                            break;

                        case SC_OPEN:
                            {
                                string str = me_readString(re_get(0));
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
                                string str = NULL;
                                int fd = re_get(0);
                                int len = re_get(1);
                                addr = re_get(2);
                                string buf = (string)malloc(len);
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

                                string buf = (string)malloc(len + 1);
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
                                string str = me_readString(re_get(0));
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
                me_push(ip);
                ip = addr;
                break;

            case IF_NULL:
                if ((rFlags & FLAG_ZERO)) {
                    me_push(ip);
                    ip = addr;
                }
                break;

            case IF_NOTNULL:
                if ((rFlags & FLAG_ZERO) == 0) {
                    me_push(ip);
                    ip = addr;
                }
                break;

            case RETURN:
                ip = me_pop();
                break;

            default:
                error("Error: Unknown opcode 0x%02x\n", opcode);
            #ifdef DEBUG
                me_heapDump("memory.dump");
            #endif
                break;
        }
    }
    #ifdef DEBUG
    me_heapDump("memory.dump");
    #endif
    return -1;
}

int main(int argc, string argv[]) {
    int r = run(argc, argv);
    if (r != 0) {
        me_heapDump("memory.dump");
    }
    return r;
}

#ifdef __cplusplus
}
#endif