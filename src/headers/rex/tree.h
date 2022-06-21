#ifndef REX_TREE_H
#define REX_TREE_H

#include "system.h"
#include "../fpengine.h"
#include "../error.h"

FILE**   openFiles;
int      filePointer = 0;
uint32_t bpCounter = 0;

int parseBinaryTree(uint32_t size, uint8_t b1, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7, uint8_t b8)
{
    uint32_t operand = me_readInt(ip);
    uint8_t reg1 = (operand >> 8) & 0xFF;
    uint8_t reg2 = (operand >> 16) & 0xFF;
    uint8_t reg3 = (operand >> 24) & 0xFF;
    ip += 4;
    uint32_t addr = me_readInt(ip);
    ip += 4;
    
    stopUnusedWarn(reg3);
    
    int returnValue = -1;

    if (b1 > 0)
    {
        if (b2 > 0)
        {
            if (b3 > 0)
            {
                if (b4 > 0)
                {
                    if (b5 > 0)
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    #ifdef DEBUG
                                    printf("Debug: Hit Breakpoint\n");
                                    string bp = (string)malloc(MAX_STRING_LENGTH);
                                    sprintf(bp, "bp-%d.dump", ++bpCounter);
                                    me_heapDump(bp);
                                    getchar();
                                    #endif
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                    else
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                }
                else
                {
                    if (b5 > 0)
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                    else
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                if (b4 > 0)
                {
                    if (b5 > 0)
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                    else
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                }
                else
                {
                    if (b5 > 0)
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                    else
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if (b3 > 0)
            {
                if (b4 > 0)
                {
                    if (b5 > 0)
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                    else
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                }
                else
                {
                    if (b5 > 0)
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                    else
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                if (b4 > 0)
                {
                    if (b5 > 0)
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                    else
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                }
                else
                {
                    if (b5 > 0)
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    // FDEC
                                    f0--;
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    // FINC
                                    f0++;
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    // FCMP
                                    float value = fe_get(reg1);
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
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                    else
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {// FNEG
                                    f0 = -f0;
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    // FDIV
                                    f0 /= fe_get(reg1);
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    // FMUL
                                    f0 *= fe_get(reg1);
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    // FSUB
                                    f0 -= fe_get(reg1);
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    // FADD
                                    f0 += fe_get(reg1);
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    // LOAD_FLOAT
                                    fe_set(reg1, me_readFloat(addr));
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    // STORE_FLOAT
                                    me_writeFloat(addr, fe_get(reg1));
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    // LOAD_IMM_FLOAT
                                    float* f = (float*) malloc(4);
                                    *f = *(float*) &addr;
                                    fe_set(reg1, *f);
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        if (b2 > 0)
        {
            if (b3 > 0)
            {
                if (b4 > 0)
                {
                    if (b5 > 0)
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                    else
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                }
                else
                {
                    if (b5 > 0)
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                    else
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                if (b4 > 0)
                {
                    if (b5 > 0)
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                    else
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                }
                else
                {
                    if (b5 > 0)
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                    else
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if (b3 > 0)
            {
                if (b4 > 0)
                {
                    if (b5 > 0)
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    if (r0 > 0)
                                    {
                                        me_push(ip);
                                        ip = addr;
                                    }
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    if (r0 == 0)
                                    {
                                        me_push(ip);
                                        ip = addr;
                                    }
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                    else
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    ip = me_pop();
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    me_push(ip);
                                    ip = addr;
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    returnValue = execSyscall(size, openFiles, &filePointer);
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    if ((rFlags & FLAG_LESS) == 0)
                                    {
                                        me_push(ip);
                                        ip = addr;
                                    }
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    if ((rFlags & FLAG_GREATER) == 0)
                                    {
                                        me_push(ip);
                                        ip = addr;
                                    }
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    if ((rFlags & FLAG_GREATER))
                                    {
                                        me_push(ip);
                                        ip = addr;
                                    }
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    if ((rFlags & FLAG_LESS))
                                    {
                                        me_push(ip);
                                        ip = addr;
                                    }
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    if ((rFlags & FLAG_EQUAL) == 0)
                                    {
                                        me_push(ip);
                                        ip = addr;
                                    }
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                }
                else
                {
                    if (b5 > 0)
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    if ((rFlags & FLAG_EQUAL))
                                    {
                                        me_push(ip);
                                        ip = addr;
                                    }
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    ip = addr;
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    uint32_t value = re_get(reg1);
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
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    r0--;
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    r0++;
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    r0 = ~r0;
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    r0 >>= re_get(reg1);
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    r0 <<= re_get(reg1);
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                    else
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    r0 ^= re_get(reg1);
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    r0 |= re_get(reg1);
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    r0 &= re_get(reg1);
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    r0 = -r0;
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    r0 %= re_get(reg1);
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    r0 /= re_get(reg1);
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    r0 *= re_get(reg1);
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    r0 -= re_get(reg1);
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                if (b4 > 0)
                {
                    if (b5 > 0)
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    r0 += re_get(reg1);
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    uint32_t a = me_pop();
                                    uint32_t b = me_pop();
                                    me_push(a);
                                    me_push(b);
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    r15 = me_pop();
                                    me_push(r15);
                                    me_push(r15);
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    re_set(reg1, me_pop());
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    // 0b00011011
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    // 0b00011010
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    // 0b00011001
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    // 0b00011000
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                    else
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    // 0b00010111
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    // 0b00010110
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    // 0b00010101
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    // 0b00010100
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    // 0b00010011
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    // 0b00010010
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    // 0b00010001
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    // 0b00010000
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                }
                else
                {
                    if (b5 > 0)
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    // 0b00001111
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    // 0b00001110
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    // 0b00001101
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    // 0b00001100
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    // 0b00001011
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    // 0b00001010
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    // 0b00001001
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    // 0b00001000
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                    else
                    {
                        if (b6 > 0)
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {
                                    // 0b00000111
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    me_push(addr);
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    re_set(reg1, heapAddress);
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    re_set(reg1, addr);
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                        else
                        {
                            if (b7 > 0)
                            {
                                if (b8 > 0)
                                {

                                    if (addr > HEAP_MAX || addr < 0)
                                    {
                                        heap_error("Attempted to write outside of memory space (Address: %02x)\n", addr);
                                        me_heapDump("memory.dump");
                                    }
                                    if (addr <= size)
                                    {
                                        heap_error("Attempted to write to reserved memory space (Address: %02x)\n", addr);
                                        me_heapDump("memory.dump");
                                    }
                                    uint32_t value = re_get(reg1);
                                    me_writeInt(addr, value);
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    me_push(re_get(reg1));
                                    goto binaryTreeEnd;
                                }
                            }
                            else
                            {
                                if (b8 > 0)
                                {
                                    re_set(reg1, re_get(reg2));
                                    goto binaryTreeEnd;
                                }
                                else
                                {
                                    // NOP
                                    goto binaryTreeEnd;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
binaryTreeEnd:
    return returnValue;
}

#endif
