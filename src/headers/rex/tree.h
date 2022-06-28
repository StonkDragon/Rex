#ifndef REX_TREE_H
#define REX_TREE_H

#include "system.h"
#include "../fpengine.h"
#include "../error.h"

FILE**   openFiles;
int      filePouinter = 3;
uint64_t bpCounter = 0;

int parseBinaryTree(uint64_t size)
{
    uint8_t opcode = me_readByte(ip);
    ip++;
    uint8_t registers = me_readByte(ip);
    uint8_t reg1 = (registers >> 4) & 0x0F;
    uint8_t reg2 = registers & 0x0F;
    ip++;

    int ret = -1;

    uint64_t addr;
    if ((opcode & 0x80) != 0) {
        addr = me_readLong(ip);
        ip += 8;

        switch (opcode) {
            case GOTO:
                ip = addr;
                break;

            case JEQ:
                if ((rFlags & FLAG_EQUAL) != 0) {
                    me_push(ip);
                    ip = addr;
                }
                break;

            case JNE:
                if ((rFlags & FLAG_EQUAL) == 0) {
                    me_push(ip);
                    ip = addr;
                }
                break;

            case JGT:
                if ((rFlags & FLAG_GREATER) != 0) {
                    me_push(ip);
                    ip = addr;
                }
                break;

            case JLT:
                if ((rFlags & FLAG_LESS) != 0) {
                    me_push(ip);
                    ip = addr;
                }
                break;

            case JGE:
                if ((rFlags & FLAG_LESS) == 0) {
                    me_push(ip);
                    ip = addr;
                }
                break;

            case JLE:
                if ((rFlags & FLAG_GREATER) == 0) {
                    me_push(ip);
                    ip = addr;
                }
                break;

            case JMP:
                me_push(ip);
                ip = addr;
                break;

            case JZ:
                if ((rFlags & FLAG_ZERO) != 0) {
                    me_push(ip);
                    ip = addr;
                }
                break;

            case JNZ:
                if ((rFlags & FLAG_ZERO) == 0) {
                    me_push(ip);
                    ip = addr;
                }
                break;

            case LOAD_IMM_FLOAT:
                fe_set(reg1, *(double*) &addr);
                break;

            case STORE_FLOAT:
                me_writeDouble(addr, fe_get(reg1));
                break;

            case LOAD_FLOAT:
                fe_set(reg1, me_readDouble(addr));
                break;

            case STORE:
                me_writeLong(addr, re_get(reg1));
                break;

            case LOAD_IMM:
                re_set(reg1, addr);
                break;

            case PUSH_IMM:
                me_push(addr);
                break;

            case LOAD:
                re_set(reg1, me_readLong(addr));
                break;

        }
    } else {
        switch (opcode) {
            case NOP:
                break;

            case MOVE:
                re_set(reg1, re_get(reg2));
                break;

            case PUSH:
                me_push(re_get(reg1));
                break;

            case POP:
                re_set(reg1, me_pop());
                break;

            case DUP:
                {
                    uint64_t value = me_pop();
                    me_push(value);
                    me_push(value);
                }
                break;

            case SWAP:
                {
                    uint64_t value1 = me_pop();
                    uint64_t value2 = me_pop();
                    me_push(value1);
                    me_push(value2);
                }
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

            case IINC:
                r0++;
                break;

            case IDEC:
                r0--;
                break;

            case CMP:
                {
                    // FCMP
                    double value = re_get(reg1);
                    if (r0 == value)
                    {
                        rFlags = rFlags | FLAG_EQUAL;
                        rFlags = rFlags & ~FLAG_LESS;
                        rFlags = rFlags & ~FLAG_GREATER;
                    }
                    else if (r0 < value)
                    {
                        rFlags = rFlags | FLAG_LESS;
                        rFlags = rFlags & ~FLAG_EQUAL;
                        rFlags = rFlags & ~FLAG_GREATER;
                    }
                    else
                    {
                        rFlags = rFlags | FLAG_GREATER;
                        rFlags = rFlags & ~FLAG_EQUAL;
                        rFlags = rFlags & ~FLAG_LESS;
                    }
                    if (r0 == 0)
                    {
                        rFlags = rFlags | FLAG_ZERO;
                    }
                    else
                    {
                        rFlags = rFlags & ~FLAG_ZERO;
                    }
                }
                break;

            case SYSTEM:
                ret = execSyscall(size, openFiles, &filePouinter);
                break;

            case RETURN:
                ip = me_pop();
                break;

            case FADD:
                f0 += fe_get(reg1);
                break;

            case FSUB:
                f0 -= fe_get(reg1);
                break;

            case FMUL:
                f0 *= fe_get(reg1);
                break;

            case FDIV:
                f0 /= fe_get(reg1);
                break;

            case FNEG:
                f0 = -f0;
                break;

            case FCMP:
                {
                    double value = fe_get(reg1);
                    if (f0 == value)
                    {
                        rFlags = rFlags | FLAG_EQUAL;
                        rFlags = rFlags & ~FLAG_LESS;
                        rFlags = rFlags & ~FLAG_GREATER;
                    }
                    else if (f0 < value)
                    {
                        rFlags = rFlags | FLAG_LESS;
                        rFlags = rFlags & ~FLAG_EQUAL;
                        rFlags = rFlags & ~FLAG_GREATER;
                    }
                    else
                    {
                        rFlags = rFlags | FLAG_GREATER;
                        rFlags = rFlags & ~FLAG_EQUAL;
                        rFlags = rFlags & ~FLAG_LESS;
                    }
                    if (f0 == 0.0)
                    {
                        rFlags = rFlags | FLAG_ZERO;
                    }
                    else
                    {
                        rFlags = rFlags & ~FLAG_ZERO;
                    }
                }
                break;

            case FINC:
                f0++;
                break;

            case FDEC:
                f0--;
                break;

            case F2I:
                re_set(reg1, (uint64_t) fe_get(reg2));
                break;

            case I2F:
                fe_set(reg1, (double) re_get(reg2));
                break;

        }
    }
    return ret;
}

#endif
