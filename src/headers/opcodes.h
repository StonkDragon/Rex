#ifndef REX_OPCODES_H
#define REX_OPCODES_H

#define REX_COMPILER_VER 8
#define HEADER_SIZE      16
#define FILE_IDENTIFIER  0xCEBACEBA

// 2-byte instructions
#define NOP              0x00
#define MOVE             0x01
#define PUSH             0x02
#define FREE             0x03

// Stack Manipulation
#define POP              0x1C
#define DUP              0x1D
#define SWAP             0x1E

// Integer Arithmetic
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
#define SYSTEM           0x35
#define RETURN           0x37

// Floating Point Arithmetic
#define FADD             0x3D
#define FSUB             0x3E
#define FMUL             0x3F
#define FDIV             0x40
#define FNEG             0x41
#define FCMP             0x42
#define FINC             0x43
#define FDEC             0x44
#define F2I              0x45
#define I2F              0x46

// 10-byte instructions
#define GOTO             0xAE
#define JEQ              0xAF
#define JNE              0xB0
#define JLT              0xB1
#define JGT              0xB2
#define JLE              0xB3
#define JGE              0xB4
#define JMP              0xB6
#define JZ               0xB8
#define JNZ              0xB9
#define LOAD_IMM_FLOAT   0xBA
#define STORE_FLOAT      0xBB
#define LOAD_FLOAT       0xBC

#define STORE            0x83
#define LOAD_IMM         0x84
#define PUSH_IMM         0x86
#define LOAD             0x85
#define ALLOC            0x87

// Debugging
#define BREAKPOINT       0x7F

#endif //REX_OPCODES_H
