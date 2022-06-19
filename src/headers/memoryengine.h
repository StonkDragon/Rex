#ifndef rex_memoryengine
#define rex_memoryengine 3

#include <stdint.h>
#include <stdio.h>

#include "cstring.h"
#include "error.h"
#include "register.h"

#define ERR_HEAP           4
#define ERR_STACK          9
#define MAX_STRING_LENGTH  16384
#define STACK_SIZE         65536
#define HEAP_SIZE          0x1000000
#define HEAP_MAX           HEAP_SIZE-1

#define heapAddress        heap[addr]
#define heap_error(...)    { fprintf(stderr, "Heap Error: " __VA_ARGS__);    fprintf(stderr, "Dumping Memory.\n"); me_heapDump("memory.dump"); exit(ERR_HEAP); }
#define stack_error(...)   { fprintf(stderr, "Stack Error: " __VA_ARGS__);   fprintf(stderr, "Dumping Memory.\n"); me_heapDump("memory.dump"); exit(ERR_STACK); }
#define binaryPattern      "%c%c%c%c%c%c%c%c"
#define byteToBinary(byte) (byte & 0x80 ? '1' : '0'), (byte & 0x40 ? '1' : '0'), (byte & 0x20 ? '1' : '0'), (byte & 0x10 ? '1' : '0'), (byte & 0x08 ? '1' : '0'), (byte & 0x04 ? '1' : '0'), (byte & 0x02 ? '1' : '0'), (byte & 0x01 ? '1' : '0')

void        me_heapDump(const string dumpFile);
void        me_writeInt(uint32_t addr, uint32_t value);
void        me_writeShort(uint32_t addr, uint16_t value);
void        me_writeByte(uint32_t addr, uint8_t value);
void        me_writeString(uint32_t addr, string value);
void        me_writeFloat(uint32_t addr, float value);
void        me_writeLong(uint32_t addr, uint64_t value);
void        me_writeDouble(uint32_t addr, double value);
int         me_readInt(uint32_t addr);
uint16_t    me_readShort(uint32_t addr);
uint8_t     me_readByte(uint32_t addr);
string      me_readString(uint32_t addr);
float       me_readFloat(uint32_t addr);
uint64_t    me_readLong(uint32_t addr);
double      me_readDouble(uint32_t addr);

uint8_t     heap[HEAP_SIZE];
uint32_t    stack[STACK_SIZE];

void me_push(uint32_t s) {
    if (sp < STACK_SIZE)
        stack[sp++] = s;
    else stack_error("Stack Overflow!\n");
}

uint32_t me_pop() {
    if (sp > 0)
        return stack[--sp];
    stack_error("Stack Underflow\n");
}

void me_heapDump(const string dumpFile) {
    FILE* f = fopen(dumpFile, "w");
    fprintf(f, "Heap Dump:\n");
    fprintf(f, "  Address     .0 .1 .2 .3 .4 .5 .6 .7 .8 .9 .A .B .C .D .E .F  As Text\n");

    uint32_t zeroes = 0;

    for (int i = 0; i < HEAP_SIZE - 1; i+=16) {
        for (int j = 0; j < 16; j++) {
            if (me_readByte(i+j) == 0) {
                zeroes++;
            } else {
                zeroes = 0;
            }
        }
        if (zeroes <= 16) {
            fprintf(f, "  0x%08x: ", i);
            for (int j = 0; j < 16; j++) {
                fprintf(f, "%02x ", me_readByte(i+j));
            }
            fprintf(f, " ");
            for (int j = 0; j < 16; j++) {
                fprintf(f, "%c ", isprint(me_readByte(i+j)) ? me_readByte(i+j) : '.');
            }
            fprintf(f, "\n");
        } else if (zeroes <= 32) {
            fprintf(f, "              ...\n");
        }
    }
    
    fprintf(f, "\n");
    fprintf(f, "Stack:\n");
    if (sp > 0) {
        for (int i = 0; i < sp; i++) {
            fprintf(f, "  0x%04x: 0x%08x", i, stack[i]);
            fprintf(f, "\n");
        }
    } else {
        fprintf(f, "  <empty>\n");
    }

    fprintf(f, "\n");
    fprintf(f, "Registers:\n");
    fprintf(f, "  r0:     0x%08x\n", r0);
    fprintf(f, "  r1:     0x%08x\n", r1);
    fprintf(f, "  r2:     0x%08x\n", r2);
    fprintf(f, "  r3:     0x%08x\n", r3);
    fprintf(f, "  r4:     0x%08x\n", r4);
    fprintf(f, "  r5:     0x%08x\n", r5);
    fprintf(f, "  r6:     0x%08x\n", r6);
    fprintf(f, "  r7:     0x%08x\n", r7);
    fprintf(f, "  r8:     0x%08x\n", r8);
    fprintf(f, "  r9:     0x%08x\n", r9);
    fprintf(f, "  r10:    0x%08x\n", r10);
    fprintf(f, "  r11:    0x%08x\n", r11);
    fprintf(f, "  r12:    0x%08x\n", r12);
    fprintf(f, "  r13:    0x%08x\n", r13);
    fprintf(f, "  r14:    0x%08x\n", r14);
    fprintf(f, "  r15:    0x%08x\n", r15);
    fprintf(f, "  ip:     %u\n", ip);
    fprintf(f, "  sp:     %u\n", sp);
    fprintf(f, "  rFlags: " binaryPattern "\n", byteToBinary(rFlags));
    
    fprintf(f, "\n");
    fprintf(f, "Physical Locations:\n");
    fprintf(f, "  Heap:   0x%p\n", heap);
    fprintf(f, "  Stack:  0x%p\n", stack);
    fprintf(f, "  r0:     0x%p\n", &r0);
    fprintf(f, "  r1:     0x%p\n", &r1);
    fprintf(f, "  r2:     0x%p\n", &r2);
    fprintf(f, "  r3:     0x%p\n", &r3);
    fprintf(f, "  r4:     0x%p\n", &r4);
    fprintf(f, "  r5:     0x%p\n", &r5);
    fprintf(f, "  r6:     0x%p\n", &r6);
    fprintf(f, "  r7:     0x%p\n", &r7);
    fprintf(f, "  r8:     0x%p\n", &r8);
    fprintf(f, "  r9:     0x%p\n", &r9);
    fprintf(f, "  r10:    0x%p\n", &r10);
    fprintf(f, "  r11:    0x%p\n", &r11);
    fprintf(f, "  r12:    0x%p\n", &r12);
    fprintf(f, "  r13:    0x%p\n", &r13);
    fprintf(f, "  r14:    0x%p\n", &r14);
    fprintf(f, "  r15:    0x%p\n", &r15);
    fprintf(f, "  IP:     0x%p\n", &ip);
    fprintf(f, "  SP:     0x%p\n", &sp);
    fprintf(f, "  RFLAGS: 0x%p\n", &rFlags);

    fclose(f);
    printf("Created Memory Dump as \"%s\"\n", dumpFile);
}

void me_writeByte(uint32_t addr, uint8_t value) {
    if (addr < 0) heap_error("Memory Access Violation!\n");
    if (addr <= HEAP_MAX) {
        heap[addr] = value;
    } else heap_error("Memory Access Violation!\n");
}

void me_writeInt(uint32_t addr, uint32_t value) {
    me_writeByte(addr, value & 0xFF);
    me_writeByte(addr+1, (value >> 8) & 0xFF);
    me_writeByte(addr+2, (value >> 16) & 0xFF);
    me_writeByte(addr+3, (value >> 24) & 0xFF);
}

void me_writeShort(uint32_t addr, uint16_t value) {
    me_writeByte(addr, value & 0xFF);
    me_writeByte(addr+1, (value >> 8) & 0xFF);
}

void me_writeString(uint32_t addr, string value) {
    for (int i = 0; i < strlen(value); i++) {
        me_writeByte(addr+i, value[i]);
    }
}

void me_writeFloat(uint32_t addr, float value) {
    me_writeInt(addr, *(uint32_t*)&value);
}

void me_writeLong(uint32_t addr, uint64_t value) {
    me_writeInt(addr, value);
    me_writeInt(addr+4, value >> 16);
}

void me_writeDouble(uint32_t addr, double value) {
    me_writeLong(addr, *(uint64_t*)&value);
}

int me_readInt(uint32_t addr) {
    return (heap[addr] | (heap[addr+1] << 8) | (heap[addr+2] << 16) | (heap[addr+3] << 24));
}

uint16_t me_readShort(uint32_t addr) {
    return (heap[addr] | (heap[addr+1] << 8));
}

uint8_t me_readByte(uint32_t addr) {
    if (addr < 0) heap_error("Memory Access Violation!\n");
    if (addr <= HEAP_MAX) {
        return heap[addr];
    } else heap_error("Memory Access Violation!\n");
}

string me_readString(uint32_t addr) {
    string str = (string)malloc(sizeof(char) * HEAP_SIZE);
    for (int i = 0; i < HEAP_SIZE - addr; i++) {
        str[i] = heap[addr+i];
        if (str[i] == 0) {
            break;
        }
    }
    return str;
}

float me_readFloat(uint32_t addr) {
    return *(float*)me_readInt(addr);
}

uint64_t me_readLong(uint32_t addr) {
    return (me_readInt(addr) | (me_readInt(addr+4) << 16));
}

double me_readDouble(uint32_t addr) {
    return *(double*)me_readLong(addr);
}

#endif