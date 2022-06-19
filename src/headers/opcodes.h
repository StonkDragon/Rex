#ifndef REX_OPCODES_H
#define REX_OPCODES_H

#define REX_COMPILER_VER 6
#define HEADER_SIZE      16
#define FILE_IDENTIFIER  0xCEBACEBA

#define NOP              0x00
#define MOVE             0x01   
#define PUSH             0x02
#define STORE            0x03
#define LOAD_IMM         0x04
#define LOAD             0x05
#define PUSHI            0x06

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
#define IF_TRUE          0x36
#define RETURN           0x37
#define IF_NULL          0x38
#define IF_NOTNULL       0x39

#ifdef REX_FLOAT_EXT
#define LOAD_IMM_FLOAT   0x80
#define STORE_FLOAT      0x81
#define LOAD_FLOAT       0x82
#define FADD             0x83
#define FSUB             0x84
#define FMUL             0x85
#define FDIV             0x86
#define FREM             0x87
#define FNEG             0x88
#define FCMP             0x89
#define FINC             0x8A
#define FDEC             0x8B
#endif

#define BREAKPOINT       0xFF

#endif //REX_OPCODES_H
