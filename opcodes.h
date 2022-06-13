#ifndef REX_OPCODES_H
#define REX_OPCODES_H
#define REX_BYTECODE_VERSION "rbia-3"

#define NOP              0x00
#define MOVE             0x01   
#define PUSH             0x02
#define STORE            0x03
#define LOAD_IMM         0x04
#define LOAD             0x05

#define POP              0x1C
#define DUP              0x1D
#define SWAP             0x1E
#define IADD             0x1F
#define ISUB             0x20
#define IMUL             0x21
#define IDIV             0x22
#define IREM             0x23
#define INEG             0x24
#define IAND             0x25
#define IOR              0x26
#define IXOR             0x27
#define ISHL             0x28
#define ISHR             0x29
#define INOT             0x2A
#define IINC             0x2B
#define IDEC             0x2C

#define CMP              0x2D
#define GOTO             0x2E
#define IF_EQ            0x2F
#define IF_NE            0x30
#define IF_LT            0x31
#define IF_GT            0x32
#define IF_LE            0x33
#define IF_GE            0x34
#define SYSTEM           0x35
#define CALL             0x36
#define RETURN           0x37
#define IF_NULL          0x38
#define IF_NOTNULL       0x39

#define BREAKPOINT       0xFF

#endif //REX_OPCODES_H
