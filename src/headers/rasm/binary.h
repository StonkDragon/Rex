#ifndef RASM_BINARY_H
#define RASM_BINARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../cutil.h"
#include "../cstring.h"
#include "../error.h"
#include "../opcodes.h"

#define MAX_LABEL_COUNT    512
#define incAddr(amount)    currentAddress += bin_opLength(amount)
#define regIdentifier(reg) atoi(reg + 1)

typedef struct {
    string label;
    uint64_t address;
} rasm_label_t;

rasm_label_t* labels;
uint64_t      labelCount = 0;
uint64_t      byteAlignment = 16;

#define bin_checkIsNumber(label, instruction) { \
    uint64_t __len = strlen(label); \
    for (uint64_t i = 0; i < __len; i++) { \
        if (!isDigit(label[i])) { \
            syntax_error("Expected number but got \"%s\" (Instruction: %s)\n", label, instruction); \
        } \
    } \
}

#define bin_checkIsRegister(label, instruction) { \
    if (label[0] != 'r' && label[0] != 'f' && label[0] != 'R' && label[0] != 'F') { \
        syntax_error("Expected register but got \"%s\" (Instruction: %s)\n", label, instruction); \
    } \
    if (regIdentifier(label) > 15 || regIdentifier(label) < 0) { \
        syntax_error("Unknown Register \"%s\" (Instruction: %s)\n", label, instruction); \
    } \
}

#define bin_checkNull(label, instruction) { \
    if (label == NULL || strlen(label) == 0) { \
        syntax_error("Expected Value but got nothing (Instruction: %s)\n", instruction); \
    } \
}

string bin_getEntryPouintLabel() {
    for (uint64_t i = 0; i < labelCount; i++) {
        if (strcmp(labels[i].label, "_main") == 0) {
            return labels[i].label;
        }
    }
    syntax_error("Could not find entry point label\n");
}

uint64_t bin_getAddressOfLabel(string label) {
    if (label[0] == '$') {
        label++;
    }

    for (uint64_t i = 0; i < labelCount; i++) {
        if (strcmp(labels[i].label, label) == 0) {
            return labels[i].address;
        }
    }
    syntax_error("Label \"%s\" not found\n", label);    
}

uint64_t bin_labelToInt(string label, string instruction) {;
    uint64_t num = 0;
    if (label[0] == '$') {
        num = bin_getAddressOfLabel(label);
    } else {
        bin_checkIsNumber(label, instruction);
        num = atoll(label);
    }
    return num;
}

int bin_opLength(int opcode) {
    return 2 + (opcode & 0x80 ? 8 : 0);
}

void bin_parseLabels(string data, uint64_t size) {
    uint64_t currentAddress = 0;
    string operand = strtok(data, " \n");

    for (uint64_t i = 0; i < size; i++) {
        if (operand == NULL) {
            break;
        }
        if (strlen(operand) == 0) {
            continue;
        }
        if (strcmp(operand, "BREAKPOINT") == 0) {
            incAddr(BREAKPOINT);
        } else if (strcmp(operand, "nop") == 0) {
            incAddr(NOP);
        } else if (strcmp(operand, "mov") == 0) {
            incAddr(MOVE);
        } else if (strcmp(operand, "psh") == 0) {
            incAddr(PUSH);
        } else if (strcmp(operand, "psi") == 0) {
            incAddr(PUSH_IMM);
        } else if (strcmp(operand, "st") == 0) {
            incAddr(STORE);
        } else if (strcmp(operand, "ldi") == 0) {
            incAddr(LOAD_IMM);
        } else if (strcmp(operand, "ld") == 0) {
            incAddr(LOAD);
        } else if (strcmp(operand, "pop") == 0) {
            incAddr(POP);
        } else if (strcmp(operand, "dup") == 0) {
            incAddr(DUP);
        } else if (strcmp(operand, "swap") == 0) {
            incAddr(SWAP);
        } else if (strcmp(operand, "add") == 0) {
            incAddr(IADD);
        } else if (strcmp(operand, "sub") == 0) {
            incAddr(ISUB);
        } else if (strcmp(operand, "mul") == 0) {
            incAddr(IMUL);
        } else if (strcmp(operand, "div") == 0) {
            incAddr(IDIV);
        } else if (strcmp(operand, "mod") == 0) {
            incAddr(IREM);
        } else if (strcmp(operand, "neg") == 0) {
            incAddr(INEG);
        } else if (strcmp(operand, "and") == 0) {
            incAddr(IAND);
        } else if (strcmp(operand, "or") == 0) {
            incAddr(IOR);
        } else if (strcmp(operand, "xor") == 0) {
            incAddr(IXOR);
        } else if (strcmp(operand, "shl") == 0) {
            incAddr(ISHL);
        } else if (strcmp(operand, "shr") == 0) {
            incAddr(ISHR);
        } else if (strcmp(operand, "not") == 0) {
            incAddr(INOT);
        } else if (strcmp(operand, "inc") == 0) {
            incAddr(IINC);
        } else if (strcmp(operand, "dec") == 0) {
            incAddr(IDEC);
        } else if (strcmp(operand, "cmp") == 0) {
            incAddr(CMP);
        } else if (strcmp(operand, "goto") == 0) {
            incAddr(GOTO);
        } else if (strcmp(operand, "jeq") == 0) {
            incAddr(JEQ);
        } else if (strcmp(operand, "jne") == 0) {
            incAddr(JNE);
        } else if (strcmp(operand, "jlt") == 0) {
            incAddr(JLT);
        } else if (strcmp(operand, "jgt") == 0) {
            incAddr(JGT);
        } else if (strcmp(operand, "jle") == 0) {
            incAddr(JLE);
        } else if (strcmp(operand, "jge") == 0) {
            incAddr(JGE);
        } else if (strcmp(operand, "syscall") == 0) {
            incAddr(SYSTEM);
        } else if (strcmp(operand, "jsr") == 0) {
            incAddr(JMP);
        } else if (strcmp(operand, "rts") == 0) {
            incAddr(RETURN);
        } else if (strcmp(operand, "jz") == 0) {
            incAddr(JZ);
        } else if (strcmp(operand, "jnz") == 0) {
            incAddr(JNZ);
        } else if (strcmp(operand, ".here") == 0) {
            operand = strtok(NULL, " \n");
            if (operand == NULL) {
                syntax_error("Missing label after .here\n");
            }
            if (strlen(operand) == 0) {
                syntax_error("Missing label after .here\n");
            }
            rasm_label_t l = {operand, currentAddress};
            labels[labelCount++] = l;
        } else if (strcmp(operand, ".align") == 0) {
            operand = strtok(NULL, " \n");
            if (operand == NULL) {
                syntax_error("Missing label after .align\n");
            }
            if (strlen(operand) == 0) {
                syntax_error("Missing label after .align\n");
            }
            byteAlignment = atoi(operand);
            if (byteAlignment == 0) {
                syntax_error("Invalid alignment\n");
            }
            if (byteAlignment < 6) {
                syntax_error("Alignment must be 6 or more\n");
            }
        } else if (strcmp(operand, ".asciiz") == 0) {      
            string str = (string) malloc(MAX_STRING_LENGTH);
            str[0] = '\0';
            operand = strtok(NULL, " \n");
            if (operand == NULL || strlen(operand) == 0) {
                syntax_error("Missing label after .asciiz\n");
            }
            if (operand[0] != '"') {
                syntax_error("Missing \" after .asciiz\n");
            }
            operand++;
            while (1) {
                if (operand[strlen(operand) - 1] == '"') {
                    strncat(str, operand, strlen(operand) - 1);
                    str[strlen(str)] = '\0';
                    break;
                }
                if (operand == NULL) {
                    printf("Error: .asciiz not closed\n");
                    exit(1);
                }
                if (strlen(operand) == 0) {
                    continue;
                }
                strcat(str, operand);
                strcat(str, " ");
                operand = strtok(NULL, " \n");
            }
            
            currentAddress += strlen(str) + 1;
            
            free(str);
        } else if (strcmp(operand, ".at") == 0) {
            operand = strtok(NULL, " \n");
            if (operand == NULL) {
                syntax_error("Missing label after .at\n");
            }
            if (strlen(operand) == 0) {
                syntax_error("Missing label after .at\n");
            }
            int pos = atoi(operand);
            if (pos < 0) {
                syntax_error("Invalid position: %d\n", pos);
            }
            currentAddress += pos;
        } else if (strcmp(operand, "fldi") == 0) {
            incAddr(LOAD_IMM_FLOAT);
        } else if (strcmp(operand, "fst") == 0) {
            incAddr(STORE_FLOAT);
        } else if (strcmp(operand, "fld") == 0) {
            incAddr(LOAD_FLOAT);
        } else if (strcmp(operand, "fadd") == 0) {
            incAddr(FADD);
        } else if (strcmp(operand, "fsub") == 0) {
            incAddr(FSUB);
        } else if (strcmp(operand, "fmul") == 0) {
            incAddr(FMUL);
        } else if (strcmp(operand, "fdiv") == 0) {
            incAddr(FDIV);
        } else if (strcmp(operand, "fneg") == 0) {
            incAddr(FNEG);
        } else if (strcmp(operand, "fcmp") == 0) {
            incAddr(FCMP);
        } else if (strcmp(operand, "finc") == 0) {
            incAddr(FINC);
        } else if (strcmp(operand, "fdec") == 0) {
            incAddr(FDEC);
        } else if (strcmp(operand, "f2i") == 0) {
            incAddr(F2I);
        } else if (strcmp(operand, "i2f") == 0) {
            incAddr(I2F);
        }
        operand = strtok(NULL, " \n");
    }
}

#endif //RASM_BINARY_H
