#ifndef RASM_BINARY_H
#define RASM_BINARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>

#include "../cstring.h"
#include "../error.h"
#include "../opcodes.h"

#define MAX_LABEL_COUNT    512
#define incAddr(amount)    currentAddress += bin_opLength(amount)
#define regIdentifier(reg) atoi(reg + 1)

typedef struct {
    string label;
    uint32_t address;
} rasm_label_t;

rasm_label_t* labels = (rasm_label_t*) malloc(sizeof(rasm_label_t) * MAX_LABEL_COUNT);
uint32_t      labelCount = 0;
uint32_t      byteAlignment = 16;

void bin_checkIsNumber(string label, string instruction) {
    if (label[0] == '$') {
        return;
    }
    for (int i = 0; i < strlen(label); i++) {
        if (!isdigit(label[i])) {
            syntax_error("Expected number but got \"%s\" (Instruction: %s)\n", label, instruction);
        }
    }
}

void bin_checkIsRegister(string label, string instruction) {
    if (label[0] != 'r') {
        syntax_error("Expected number but got \"%s\" (Instruction: %s)\n", label, instruction);
    }
    if (regIdentifier(label) > 15 || regIdentifier(label) < 0) {
        syntax_error("Unknown Register \"%s\" (Instruction: %s)\n", label, instruction);
    }
}

void bin_checkNull(string label, string instruction) {
    if (label == NULL || strlen(label) == 0) {
        syntax_error("Expected Value but got nothing (Instruction: %s)\n", instruction);
    }
}

uint32_t bin_getAddressOfLabel(string label) {
    if (label[0] == '$') {
        label++;
    }
    for (int i = 0; i < labelCount; i++) {
        if (strcmp(labels[i].label, label) == 0) {
            return labels[i].address;
        }
    }
    syntax_error("Label \"%s\" not found\n", label);
}

uint32_t bin_labelToInt(string label, string instruction) {
    uint32_t num = 0;
    if (label[0] == '$') {
        num = bin_getAddressOfLabel(label);
    } else {
        bin_checkIsNumber(label, instruction);
        num = atoi(label);
    }
    return num;
}

int bin_opLength(int opcode) {
    switch (opcode) {
        default:
            return 8;
    }
}

void bin_parseLabels(string data, uint32_t size) {
    uint32_t currentAddress = 0;
    string operand = strtok(data, " ");

    for (int i = 0; i < size; i++) {
        if (operand == NULL) {
            break;
        }
        if (strlen(operand) == 0) {
            continue;
        }
        if (strcmp(operand, "breakpoint") == 0) {
            incAddr(BREAKPOINT);
        } else if (strcmp(operand, "nop") == 0) {
            incAddr(NOP);
        } else if (strcmp(operand, "mov") == 0) {
            incAddr(MOVE);
        } else if (strcmp(operand, "psh") == 0) {
            incAddr(PUSH);
        } else if (strcmp(operand, "psi") == 0) {
            incAddr(PUSHI);
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
            incAddr(IF_EQ);
        } else if (strcmp(operand, "jne") == 0) {
            incAddr(IF_NE);
        } else if (strcmp(operand, "jlt") == 0) {
            incAddr(IF_LT);
        } else if (strcmp(operand, "jgt") == 0) {
            incAddr(IF_GT);
        } else if (strcmp(operand, "jle") == 0) {
            incAddr(IF_LE);
        } else if (strcmp(operand, "jge") == 0) {
            incAddr(IF_GE);
        } else if (strcmp(operand, "syscall") == 0) {
            incAddr(SYSTEM);
        } else if (strcmp(operand, "jsr") == 0) {
            incAddr(IF_TRUE);
        } else if (strcmp(operand, "rts") == 0) {
            incAddr(RETURN);
        } else if (strcmp(operand, "jz") == 0) {
            incAddr(IF_NULL);
        } else if (strcmp(operand, "jnz") == 0) {
            incAddr(IF_NOTNULL);
        } else if (strcmp(operand, ".here") == 0) {
            operand = strtok(NULL, " ");
            if (operand == NULL) {
                syntax_error("Missing label after .here\n");
            }
            if (strlen(operand) == 0) {
                syntax_error("Missing label after .here\n");
            }
            rasm_label_t l = {operand, currentAddress};
            labels[labelCount++] = l;
        } else if (strcmp(operand, ".align") == 0) {
            operand = strtok(NULL, " ");
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
            if (byteAlignment < 8) {
                syntax_error("Alignment must be bigger than 8\n");
            }
        } else if (strcmp(operand, ".asciiz") == 0) {      
            string str = (string) malloc(sizeof(char) * MAX_STRING_LENGTH);
            str[0] = '\0';
            operand = strtok(NULL, " ");
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
                operand = strtok(NULL, " ");
            }
            
            currentAddress += strlen(str) + 1;
            
            free(str);
        } else if (strcmp(operand, ".at") == 0) {
            operand = strtok(NULL, " ");
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
        }
        #ifdef REX_FLOAT_EXT
        else if (strcmp(operand, "fldi") == 0) {
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
        } else if (strcmp(operand, "fmod") == 0) {
            incAddr(FREM);
        } else if (strcmp(operand, "fneg") == 0) {
            incAddr(FNEG);
        } else if (strcmp(operand, "fcmp") == 0) {
            incAddr(FCMP);
        } else if (strcmp(operand, "finc") == 0) {
            incAddr(FINC);
        } else if (strcmp(operand, "fdec") == 0) {
            incAddr(FDEC);
        }
        #else
        else if (strcmp(operand, "fldi") == 0) {
            error("Floating Point operations are not supported in this build! (Problematic Instruction: %s)", operand);
        } else if (strcmp(operand, "fst") == 0) {
            error("Floating Point operations are not supported in this build! (Problematic Instruction: %s)", operand);
        } else if (strcmp(operand, "fld") == 0) {
            error("Floating Point operations are not supported in this build! (Problematic Instruction: %s)", operand);
        } else if (strcmp(operand, "fadd") == 0) {
            error("Floating Point operations are not supported in this build! (Problematic Instruction: %s)", operand);
        } else if (strcmp(operand, "fsub") == 0) {
            error("Floating Point operations are not supported in this build! (Problematic Instruction: %s)", operand);
        } else if (strcmp(operand, "fmul") == 0) {
            error("Floating Point operations are not supported in this build! (Problematic Instruction: %s)", operand);
        } else if (strcmp(operand, "fdiv") == 0) {
            error("Floating Point operations are not supported in this build! (Problematic Instruction: %s)", operand);
        } else if (strcmp(operand, "fmod") == 0) {
            error("Floating Point operations are not supported in this build! (Problematic Instruction: %s)", operand);
        } else if (strcmp(operand, "fneg") == 0) {
            error("Floating Point operations are not supported in this build! (Problematic Instruction: %s)", operand);
        } else if (strcmp(operand, "fcmp") == 0) {
            error("Floating Point operations are not supported in this build! (Problematic Instruction: %s)", operand);
        } else if (strcmp(operand, "finc") == 0) {
            error("Floating Point operations are not supported in this build! (Problematic Instruction: %s)", operand);
        } else if (strcmp(operand, "fdec") == 0) {
            error("Floating Point operations are not supported in this build! (Problematic Instruction: %s)", operand);
        }
        #endif
        operand = strtok(NULL, " ");
    }
}

#endif //RASM_BINARY_H