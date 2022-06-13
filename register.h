#ifndef REX_REGISTER_H
#define REX_REGISTER_H

// __stderrp doesn't exist on all systems
#ifndef __stderrp
#define __stderrp stderr
#endif

#define FLAG_ZERO               0b00000001
#define FLAG_NEG                0b00000010
#define FLAG_EQUAL              0b00000100
#define FLAG_LESS               0b00001000
#define FLAG_GREATER            0b00010000
#define FLAG_UNUSED_1           0b00100000
#define FLAG_UNUSED_2           0b01000000
#define FLAG_UNUSED_3           0b10000000

#define STACK_SIZE              256
#define HEAP_SIZE               131072

uint32_t                        stack[STACK_SIZE];
uint32_t                        heap[HEAP_SIZE];

#define HEAP_MAX                HEAP_SIZE-1
#define r0                      heap[HEAP_MAX]
#define r1                      heap[HEAP_MAX - 1]
#define r2                      heap[HEAP_MAX - 2]
#define r3                      heap[HEAP_MAX - 3]
#define r4                      heap[HEAP_MAX - 4]
#define r5                      heap[HEAP_MAX - 5]
#define r6                      heap[HEAP_MAX - 6]
#define r7                      heap[HEAP_MAX - 7]
#define r8                      heap[HEAP_MAX - 8]
#define r9                      heap[HEAP_MAX - 9]
#define r10                     heap[HEAP_MAX - 10]
#define r11                     heap[HEAP_MAX - 11]
#define r12                     heap[HEAP_MAX - 12]
#define r13                     heap[HEAP_MAX - 13]
#define r14                     heap[HEAP_MAX - 14]
#define r15                     heap[HEAP_MAX - 15]
#define ip                      heap[HEAP_MAX - 16]
#define sp                      heap[HEAP_MAX - 17]
#define rFlags                  heap[HEAP_MAX - 18]
#define addr                    heap[HEAP_MAX - 19]

#define push(s)                 stack[sp++] = s
#define pop()                   stack[--sp]
#define setRegister(reg, val)   heap[HEAP_MAX - reg] = val
#define getRegister(reg)        heap[HEAP_MAX - reg]
#define heapAddress             heap[addr]

#endif // REX_REGISTER_H