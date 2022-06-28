#ifndef rex_memoryengine
#define rex_memoryengine 4

#include <stdint.h>
#include <stdio.h>

#include "cstring.h"
#include "error.h"
#include "register.h"
#include "fpengine.h"

#define ERR_HEAP           4
#define ERR_STACK          9
#define MAX_STRING_LENGTH  16384
#define STACK_SIZE         65536
#define HEAP_SIZE          0x1000000
#define HEAP_MAX           HEAP_SIZE-1

#define heap_error(...)    { fprintf(stderr, "%s:%d: ", __FILE__, __LINE__); fprintf(stderr, "Heap Error: " __VA_ARGS__);    fprintf(stderr, "Dumping Memory.\n"); me_heapDump("memory.dump"); exit(ERR_HEAP); }
#define stack_error(...)   { fprintf(stderr, "%s:%d: ", __FILE__, __LINE__); fprintf(stderr, "Stack Error: " __VA_ARGS__);   fprintf(stderr, "Dumping Memory.\n"); me_heapDump("memory.dump"); exit(ERR_STACK); }
#define byteToBinary(byte) (byte & 0x80 ? '1' : '0'), (byte & 0x40 ? '1' : '0'), (byte & 0x20 ? '1' : '0'), (byte & 0x10 ? '1' : '0'), (byte & 0x08 ? '1' : '0'), (byte & 0x04 ? '1' : '0'), (byte & 0x02 ? '1' : '0'), (byte & 0x01 ? '1' : '0')

void       me_heapDump(const string dumpFile);
void       me_writeuint(uint64_t addr, uint32_t value);
void       me_writeShort(uint64_t addr, uint16_t value);
void       me_writeByte(uint64_t addr, uint8_t value);
void       me_writeString(uint64_t addr, string value);
void       me_writeFloat(uint64_t addr, float value);
void       me_writeLong(uint64_t addr, uint64_t value);
void       me_writeDouble(uint64_t addr, double value);
uint32_t   me_readuint(uint64_t addr);
uint16_t   me_readShort(uint64_t addr);
uint8_t    me_readByte(uint64_t addr);
string     me_readString(uint64_t addr);
float      me_readFloat(uint64_t addr);
uint64_t   me_readLong(uint64_t addr);
double     me_readDouble(uint64_t addr);

uint8_t    heap[HEAP_SIZE];
uint64_t    stack[STACK_SIZE];

void me_push(uint64_t s) {
    if (sp < STACK_SIZE)
        stack[sp++] = s;
    else stack_error("Stack Overflow!\n");
}

uint64_t me_pop() {
    if (sp > 0)
        return stack[--sp];
    stack_error("Stack Underflow\n");
}

void me_heapDump(const string dumpFile) {
    FILE* f = fopen(dumpFile, "w");
    fprintf(f, "Heap Dump:\n");
    fprintf(f, "  Address     .0 .1 .2 .3 .4 .5 .6 .7 .8 .9 .A .B .C .D .E .F  As Text\n");

    uint64_t zeroes = 0;

    for (uint64_t i = 0; i < HEAP_SIZE - 1; i+=16) {
        for (size_t j = 0; j < 16; j++) {
            if (me_readByte(i+j) == 0) {
                zeroes++;
            } else {
                zeroes = 0;
            }
        }
        if (zeroes <= 16) {
            fprintf(f, "  0x%08llx: ", i);
            for (size_t j = 0; j < 16; j++) {
                fprintf(f, "%02x ", me_readByte(i+j) & 0xFF);
            }
            fprintf(f, " ");
            for (size_t j = 0; j < 16; j++) {
                fprintf(f, "%c ", isPrintable(me_readByte(i+j)) ? me_readByte(i+j) : '.');
            }
            fprintf(f, "\n");
        } else if (zeroes <= 32) {
            fprintf(f, "              ...\n");
        }
    }
    
    fprintf(f, "\n");
    fprintf(f, "Stack:\n");
    if (sp > 0) {
        for (uint64_t i = 0; i < sp; i++) {
            fprintf(f, "  0x%04llx: 0x%16llx", i, stack[i]);
            fprintf(f, "\n");
        }
    } else {
        fprintf(f, "  <empty>\n");
    }

    fprintf(f, "\n");
    fprintf(f, "Registers:\n");
    fprintf(f, "  r0:     0x%016llx\n", r0);
    fprintf(f, "  r1:     0x%016llx\n", r1);
    fprintf(f, "  r2:     0x%016llx\n", r2);
    fprintf(f, "  r3:     0x%016llx\n", r3);
    fprintf(f, "  r4:     0x%016llx\n", r4);
    fprintf(f, "  r5:     0x%016llx\n", r5);
    fprintf(f, "  r6:     0x%016llx\n", r6);
    fprintf(f, "  r7:     0x%016llx\n", r7);
    fprintf(f, "  r8:     0x%016llx\n", r8);
    fprintf(f, "  r9:     0x%016llx\n", r9);
    fprintf(f, "  r10:    0x%016llx\n", r10);
    fprintf(f, "  r11:    0x%016llx\n", r11);
    fprintf(f, "  r12:    0x%016llx\n", r12);
    fprintf(f, "  r13:    0x%016llx\n", r13);
    fprintf(f, "  r14:    0x%016llx\n", r14);
    fprintf(f, "  r15:    0x%016llx\n", r15);
    fprintf(f, "  ip:     %llu\n", ip);
    fprintf(f, "  sp:     %llu\n", sp);
    fprintf(f, "  rFlags: %c%c%c%c%c%c%c%c\n", byteToBinary(rFlags));
    fprintf(f, "\n");

    fprintf(f, "Floating Point Registers:\n");
    fprintf(f, "  f0:     %f (0x%016llx)\n", f0, *(uint64_t*) &f0);
    fprintf(f, "  f1:     %f (0x%016llx)\n", f1, *(uint64_t*) &f1);
    fprintf(f, "  f2:     %f (0x%016llx)\n", f2, *(uint64_t*) &f2);
    fprintf(f, "  f3:     %f (0x%016llx)\n", f3, *(uint64_t*) &f3);
    fprintf(f, "  f4:     %f (0x%016llx)\n", f4, *(uint64_t*) &f4);
    fprintf(f, "  f5:     %f (0x%016llx)\n", f5, *(uint64_t*) &f5);
    fprintf(f, "  f6:     %f (0x%016llx)\n", f6, *(uint64_t*) &f6);
    fprintf(f, "  f7:     %f (0x%016llx)\n", f7, *(uint64_t*) &f7);

    fprintf(f, "\n");
    fprintf(f, "Physical Locations:\n");
    fprintf(f, "  Heap:   %p\n", (void*) heap);
    fprintf(f, "  Stack:  %p\n", (void*) stack);
    fprintf(f, "  r0:     %p\n", (void*) &r0);
    fprintf(f, "  r1:     %p\n", (void*) &r1);
    fprintf(f, "  r2:     %p\n", (void*) &r2);
    fprintf(f, "  r3:     %p\n", (void*) &r3);
    fprintf(f, "  r4:     %p\n", (void*) &r4);
    fprintf(f, "  r5:     %p\n", (void*) &r5);
    fprintf(f, "  r6:     %p\n", (void*) &r6);
    fprintf(f, "  r7:     %p\n", (void*) &r7);
    fprintf(f, "  r8:     %p\n", (void*) &r8);
    fprintf(f, "  r9:     %p\n", (void*) &r9);
    fprintf(f, "  r10:    %p\n", (void*) &r10);
    fprintf(f, "  r11:    %p\n", (void*) &r11);
    fprintf(f, "  r12:    %p\n", (void*) &r12);
    fprintf(f, "  r13:    %p\n", (void*) &r13);
    fprintf(f, "  r14:    %p\n", (void*) &r14);
    fprintf(f, "  r15:    %p\n", (void*) &r15);
    fprintf(f, "  f0:     %p\n", (void*) &f0);
    fprintf(f, "  f1:     %p\n", (void*) &f1);
    fprintf(f, "  f2:     %p\n", (void*) &f2);
    fprintf(f, "  f3:     %p\n", (void*) &f3);
    fprintf(f, "  f4:     %p\n", (void*) &f4);
    fprintf(f, "  f5:     %p\n", (void*) &f5);
    fprintf(f, "  f6:     %p\n", (void*) &f6);
    fprintf(f, "  f7:     %p\n", (void*) &f7);
    fprintf(f, "  IP:     %p\n", (void*) &ip);
    fprintf(f, "  SP:     %p\n", (void*) &sp);
    fprintf(f, "  RFLAGS: %p\n", (void*) &rFlags);

    fclose(f);
    printf("Created Register Dump as \"%s\"\n", dumpFile);
}

void me_writeByte(uint64_t addr, uint8_t value) {
    // printf("Writing 0x%02x to 0x%016llx\n", value, addr);
    if (addr < 0) heap_error("Memory Access Violation! Trying to write to Address 0x%016llx\n", addr);
    if (addr <= HEAP_MAX) {
        heap[addr] = value & 0xFF;
    } else heap_error("Memory Access Violation! Trying to write to Address 0x%016llx\n", addr);
}

void me_writeuint(uint64_t addr, uint32_t value) {
    me_writeByte(addr, value & 0xFF);
    me_writeByte(addr+1, (value >> 8) & 0xFF);
    me_writeByte(addr+2, (value >> 16) & 0xFF);
    me_writeByte(addr+3, (value >> 24) & 0xFF);
}

void me_writeShort(uint64_t addr, uint16_t value) {
    me_writeByte(addr, value & 0xFF);
    me_writeByte(addr+1, (value >> 8) & 0xFF);
}

void me_writeString(uint64_t addr, string value) {
    uint64_t len = strlen(value);
    for (uint64_t i = 0; i < len; i++) {
        me_writeByte(addr+i, value[i]);
    }
}

void me_writeFloat(uint64_t addr, float value) {
    me_writeuint(addr, *(uint64_t*) &value);
}

void me_writeLong(uint64_t addr, uint64_t value) {
    me_writeuint(addr, value);
    me_writeuint(addr+4, value >> 16);
}

void me_writeDouble(uint64_t addr, double value) {
    me_writeLong(addr, *(uint64_t*) &value);
}

uint32_t me_readuint(uint64_t addr) {
    return (me_readByte(addr) | (me_readByte(addr+1) << 8) | (me_readByte(addr+2) << 16) | (me_readByte(addr+3) << 24));
}

uint16_t me_readShort(uint64_t addr) {
    return (me_readByte(addr) | (me_readByte(addr+1) << 8));
}

uint8_t me_readByte(uint64_t addr) {
    if (addr < 0) heap_error("Memory Access Violation! Trying to read from Address 0x%016llx\n", addr);
    if (addr <= HEAP_MAX) {
        return heap[addr] & 0xFF;
    } else heap_error("Memory Access Violation! Trying to read from Address 0x%016llx\n", addr);
}

string me_readString(uint64_t addr) {
    string str = (string)malloc(HEAP_SIZE);
    for (uint64_t i = 0; i < HEAP_SIZE - addr; i++) {
        str[i] = heap[addr+i];
        if (str[i] == 0) {
            break;
        }
    }
    return str;
}

float me_readFloat(uint64_t addr) {
    int i = me_readuint(addr);
    return *(float*) &i;
}

uint64_t me_readLong(uint64_t addr) {
    return (me_readuint(addr) | (me_readuint(addr+4) << 16));
}

double me_readDouble(uint64_t addr) {
    long i = me_readLong(addr);
    return *(double*) &i;
}

#endif
