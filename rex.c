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
#include "register.h"

char*    buffer;
char*    getString(uint32_t str_addr);

#define error(...) fprintf(__stderrp, __VA_ARGS__)

#ifdef DEBUG
void dumpHeap() {
    printf("Heap Dump:\n");
    for (int i = 0; i < HEAP_SIZE; i++) {
        if (heap[i] != 0) {
            printf("%d: %u\n", i, heap[i]);
        }
    }
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
        return 1;
    }

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    buffer = (char*)malloc(size);
    fread(buffer, size, 1, file);
    fclose(file);
    for (int i = 0; i < size; i++) {
        heap[i] = buffer[i];
    }

    while (ip < size) {
        uint8_t opcode = heap[ip];
        addr = heap[ip + 1] |
            (heap[ip + 2] << 8) |
            (heap[ip + 3] << 16) |
            (heap[ip + 4] << 24);

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
                addr = heap[ip + 1] |
                    (heap[ip + 2] << 8) |
                    (heap[ip + 3] << 16) |
                    (heap[ip + 4] << 24);
                if (addr > HEAP_MAX || addr < 0) {
                    error("Heap Error: Attempted to write outside of memory space\n");
                    return -1;
                }
                if (addr >= HEAP_MAX - 19 || addr <= size) {
                    error("Heap Error: Attempted to write to reserved memory space\n");
                    return -1;
                }
                heapAddress = getRegister(heap[ip]);
                ip += 4;
                break;

            case LOAD:
                ip++;
                addr = heap[ip + 1] |
                    (heap[ip + 2] << 8) |
                    (heap[ip + 3] << 16) |
                    (heap[ip + 4] << 24);
                setRegister(heap[ip], heapAddress);
                ip += 4;
                break;

            case LOAD_IMM:
                ip++;
                addr = heap[ip + 1] |
                    (heap[ip + 2] << 8) |
                    (heap[ip + 3] << 16) |
                    (heap[ip + 4] << 24);
                setRegister(heap[ip], addr);
                ip += 4;
                break;

            case MOVE:
                setRegister(heap[++ip], getRegister(heap[++ip]));
                break;

            case POP:
                setRegister(heap[++ip], pop());
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
                r0 += getRegister(heap[++ip]);
                break;

            case ISUB:
                r0 -= getRegister(heap[++ip]);
                break;

            case IMUL:
                r0 *= getRegister(heap[++ip]);
                break;

            case IDIV:
                r0 /= getRegister(heap[++ip]);
                break;

            case IREM:
                r0 %= getRegister(heap[++ip]);
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
                r0 &= getRegister(heap[++ip]);
                break;

            case IOR:
                r0 |= getRegister(heap[++ip]);
                break;

            case IXOR:
                r0 ^= getRegister(heap[++ip]);
                break;

            case ISHL:
                r0 <<= getRegister(heap[++ip]);
                break;

            case ISHR:
                r0 >>= getRegister(heap[++ip]);
                break;

            case INOT:
                r0 = ~r0;
                break;

            case CMP:
                if (r0 == getRegister(heap[++ip])) {
                    rFlags = rFlags | FLAG_EQUAL;
                    rFlags = rFlags & ~FLAG_LESS;
                    rFlags = rFlags & ~FLAG_GREATER;
                } else if (r0 < getRegister(heap[++ip])) {
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
                if ((rFlags & FLAG_EQUAL) == 0) break;
                ip += 4;
                push(ip);
                ip = --addr;
                break;

            case IF_NE:
                if ((rFlags & FLAG_EQUAL) != 0) break;
                ip += 4;
                push(ip);
                ip = --addr;
                break;

            case IF_LT:
                if ((rFlags & FLAG_LESS) == 0) break;
                ip += 4;
                push(ip);
                ip = --addr;
                break;

            case IF_GE:
                if ((rFlags & FLAG_LESS) != 0) break;
                ip += 4;
                push(ip);
                ip = --addr;
                break;

            case IF_GT:
                if ((rFlags & FLAG_GREATER) == 0) break;
                ip += 4;
                push(ip);
                ip = --addr;
                break;

            case IF_LE:
                if ((rFlags & FLAG_GREATER) != 0) break;
                ip += 4;
                push(ip);
                ip = --addr;
                break;

            case SYSTEM:
                {
                    switch (getRegister(0xF))
                    {
                        case SC_EXIT:
                            #ifdef DEBUG
                            dumpHeap();
                            #endif
                            exit(getRegister(0));
                            break;

                        case SC_READ_CON:
                            {
                                addr = getRegister(0);
                                if (addr > HEAP_MAX || addr < 0) {
                                    error("Heap Error: Attempted to write outside of memory space\n");
                                    return -1;
                                }
                                if (addr >= HEAP_MAX - 19 || addr <= size) {
                                    error("Heap Error: Attempted to write to reserved memory space\n");
                                    return -1;
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
                                    heap[addr + i] = line[i];
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
                                int fd = open(str, getRegister(1));
                                free(str);
                                if (fd < 0) {
                                    error("IO Error: Failed to open file\n");
                                    return -1;
                                }
                                setRegister(r0, fd);
                            }
                            break;

                        case SC_CLOSE:
                            {
                                int cl = close(getRegister(0));
                                if (cl < 0) {
                                    error("Error: Failed to close file\n");
                                    return -1;
                                }
                            }
                            break;

                        case SC_READ:
                            {
                                char* str = NULL;
                                int fd = getRegister(0);
                                int len = getRegister(1);
                                addr = getRegister(2);
                                char* buf = malloc(len);
                                int r = read(fd, buf, len);
                                if (r < 0) {
                                    error("Error: Failed to read from file\n");
                                    return -1;
                                }
                                for (int i = 0; i < r; i++) {
                                    heap[addr + i] = buf[i];
                                }
                                free(buf);
                                free(str);
                            }
                            break;

                        case SC_WRITE:
                            {
                                char* str = NULL;
                                int fd = getRegister(0);
                                int len = getRegister(1);
                                addr = getRegister(2);
                                if (addr > HEAP_MAX || addr < 0) {
                                    error("Heap Error: Attempted to write outside of memory space\n");
                                    return -1;
                                }
                                if (addr >= HEAP_MAX - 19 || addr <= size) {
                                    error("Heap Error: Attempted to write to reserved memory space\n");
                                    return -1;
                                }

                                char* buf = malloc(len);
                                for (int i = 0; i < len; i++) {
                                    buf[i] = heap[addr + i];
                                }
                                int r = write(fd, buf, len);
                                if (r < 0) {
                                    error("Error: Failed to write to file\n");
                                    return -1;
                                }
                                free(buf);
                                free(str);
                            }
                            break;

                        case SC_EXEC:
                            {
                                char* str = getString(getRegister(0));
                                int r = system(str);
                                free(str);
                                if (r < 0) {
                                    error("Error: Failed to execute program\n");
                                    return -1;
                                }
                                push(r);
                            }
                            break;
                    }
                }
                break;

            case CALL:
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
                return -1;
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
    char* str = (char*)malloc(sizeof(char) * HEAP_SIZE);
    for (int i = 0; i < HEAP_SIZE; i++) {
        str[i] = heap[str_addr + i];
        if (str[i] == '\0') {
            break;
        }
    }
    #ifdef DEBUG
    printf("Read String \"%s\" from heap\n", str);
    #endif
    return str;
}

#ifdef __cplusplus
}
#endif