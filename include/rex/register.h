#ifndef register_engine
#define register_engine 1

#include <stdint.h>

#define FLAG_ZERO               0b00000001
#define FLAG_NEG                0b00000010
#define FLAG_EQUAL              0b00000100
#define FLAG_LESS               0b00001000
#define FLAG_GREATER            0b00010000
#define FLAG_UNUSED_1           0b00100000
#define FLAG_UNUSED_2           0b01000000
#define FLAG_UNUSED_3           0b10000000

uint32_t r0 = 0;
uint32_t r1 = 0;
uint32_t r2 = 0;
uint32_t r3 = 0;
uint32_t r4 = 0;
uint32_t r5 = 0;
uint32_t r6 = 0;
uint32_t r7 = 0;
uint32_t r8 = 0;
uint32_t r9 = 0;
uint32_t r10 = 0;
uint32_t r11 = 0;
uint32_t r12 = 0;
uint32_t r13 = 0;
uint32_t r14 = 0;
uint32_t r15 = 0;
uint32_t ip = 0;
uint32_t sp = 0;
uint32_t rFlags = 0;
uint32_t addr = 0;

void re_set(uint8_t reg, uint32_t val) {
    switch (reg) {
        case 0: r0 = val; break;
        case 1: r1 = val; break;
        case 2: r2 = val; break;
        case 3: r3 = val; break;
        case 4: r4 = val; break;
        case 5: r5 = val; break;
        case 6: r6 = val; break;
        case 7: r7 = val; break;
        case 8: r8 = val; break;
        case 9: r9 = val; break;
        case 10: r10 = val; break;
        case 11: r11 = val; break;
        case 12: r12 = val; break;
        case 13: r13 = val; break;
        case 14: r14 = val; break;
        case 15: r15 = val; break;
    }
}

uint32_t re_get(uint8_t reg) {
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

#endif // REX_REGISTER_H