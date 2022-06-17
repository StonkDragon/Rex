// check for c++
#ifdef __cplusplus
extern "C" {
#endif
#define RASM

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>

#include "headers/register.h"
#include "headers/opcodes.h"
#include "headers/error.h"
#include "headers/crc32.h"

#define incAddr(amount)      currentAddress += opLength(amount)
#define MAX_LABEL_COUNT      512
#define MAX_STRING_LENGTH    16384
#define checkNull(reg, inst) if (reg == NULL) { syntax_error("Missing operand. (Instruction: %s)\n", inst); } if (strlen(reg) == 0) { syntax_error("Missing operand. (Instruction: %s)\n", inst); }

int opLength(int op);
int regIdentifier(char* reg);

uint32_t getAddressOfLabel(char* label);

typedef struct {
    char* label;
    uint32_t address;
} rasm_label_t;

rasm_label_t* labels = (rasm_label_t*) malloc(sizeof(rasm_label_t) * MAX_LABEL_COUNT);
uint32_t labelCount = 0;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        usage_error("%s <file>\n", argv[0]);
    }
    FILE* file = fopen(argv[1], "r");
    if (file == 0) {
        native_error("Could not open file %s\n", argv[1]);
    }
    char* outFile = (char*) malloc(strlen(argv[1]) - 3);
    strncpy(outFile, argv[1], strlen(argv[1]) - 5);
    strcat(outFile, ".rx");
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* file_buffer = (char*)malloc(size);
    fread(file_buffer, size, 1, file);
    fclose(file);

    char* token = strtok(file_buffer, " \n");
    char* token_buffer = (char*)malloc(sizeof(char) * size);
    char* buffer = (char*)malloc(sizeof(char) * size);
    int i = 0;
    while (token != NULL) {
        if (strlen(token) > 0) {
            strcat(token_buffer, token);
            strcat(token_buffer, " ");
        }
        i++;
        token = strtok(NULL, " \n");
    }
    strcpy(buffer, token_buffer);

    uint32_t currentAddress = 0;
    char* operand = strtok(token_buffer, " ");
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
        } else if (strcmp(operand, ".asciiz") == 0) {
            operand = strtok(NULL, " ");
            if (operand == NULL) {
                syntax_error("Missing label after .asciiz\n");
            }
            if (strlen(operand) == 0) {
                syntax_error("Missing label after .asciiz\n");
            }
            char* str = (char*) malloc(sizeof(char) * MAX_STRING_LENGTH);
            if (operand[strlen(operand) - 1] == '"') {
                operand[strlen(operand) - 1] = '\0';
                strcpy(str, operand + 1);
                incAddr(strlen(str));
            }
            int strIndex = 0;
            if (operand[0] == '"') {
                strcpy(str, operand + 1);
            } else {
                for (int i = 0; i < strlen(operand); i++) {
                    if (operand[i] == '\\') {
                        if (operand[i + 1] == 'n') {
                            str[strIndex] = '\n';
                            i++;
                        } else if (operand[i + 1] == 't') {
                            str[strIndex] = '\t';
                            i++;
                        } else if (operand[i + 1] == '\\') {
                            str[strIndex] = '\\';
                            i++;
                        } else if (operand[i + 1] == '"') {
                            str[strIndex] = '"';
                            i++;
                        } else {
                            syntax_error("Invalid escape sequence\n");
                        }
                    } else {
                        str[strIndex] = operand[i];
                    }
                    strIndex++;
                }
                continue;
            }
            operand = strtok(NULL, " ");

            int i = 0;
            char last = operand[strlen(operand) - 1];
            char* word = (char*) malloc(sizeof(char) * MAX_STRING_LENGTH);
            const char* space = " ";

            while (last != '"') {
                if (operand == NULL) {
                    break;
                }
                if (operand[i] == '\\') {
                    if (operand[i + 1] == 'n') {
                        word[i] = '\n';
                        i++;
                    } else if (operand[i + 1] == 't') {
                        word[i] = '\t';
                        i++;
                    } else if (operand[i + 1] == '\\') {
                        word[i] = '\\';
                        i++;
                    } else if (operand[i + 1] == '"') {
                        word[i] = '"';
                        i++;
                    } else {
                        syntax_error("Invalid escape sequence\n");
                    }
                } else {
                    word[i] = operand[i];
                }
                if (i == strlen(operand)) {
                    incAddr(strlen(str) + 1);
                    operand = strtok(NULL, " ");
                    last = operand[strlen(operand) - 1];
                    i = 0;
                    strcat(str, space);
                    strcat(str, word);
                    continue;
                }
                i++;
            }
            if (operand == NULL) {
                syntax_error("Missing closing quote\n");
            }
            char* word2 = (char*) malloc(sizeof(char) * MAX_STRING_LENGTH);
            int j = 0;
            for (int i = 0; i < strlen(operand); i++) {
                if (operand[i] == '\\') {
                    if (operand[i + 1] == 'n') {
                        word2[j] = '\n';
                    } else if (operand[i + 1] == 't') {
                        word2[j] = '\t';
                    } else if (operand[i + 1] == '\\') {
                        word2[j] = '\\';
                    } else if (operand[i + 1] == '"') {
                        word2[j] = '"';
                    } else {
                        syntax_error("Invalid escape sequence\n");
                    }
                    i++;
                } else {
                    if (operand[i] == '"') {
                        break;
                    }
                    word2[j] = operand[i];
                }
                j++;
            }
            strcat(str, space);
            strcat(str, word2);
            
            currentAddress += strlen(str) + 1;
            
            free(str);
            free(word);
            free(word2);
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

    FILE* out = fopen(outFile, "wb");
    if (out == 0) {
        native_error("Could not open file %s\n", outFile);
    }

    uint8_t* data = (uint8_t*)malloc(sizeof(uint8_t) * currentAddress + HEADER_SIZE);
    uint32_t ptr = 0;

    operand = strtok(buffer, " ");
    for (int i = 0; i < size; i++) {
        if (operand == NULL) {
            continue;
        }
        if (strlen(operand) == 0) {
            continue;
        }
        if (operand == NULL) {
            continue;
        }
        if (strcmp(operand, "breakpoint") == 0) {
            data[ptr++] = BREAKPOINT;
        } else if (strcmp(operand, "nop") == 0) {
            data[ptr++] = NOP;
        } else if (strcmp(operand, "mov") == 0) {
            data[ptr++] = MOVE;
            char* reg1 = strtok(NULL, " ");
            char* reg2 = strtok(NULL, " ");
            checkNull(reg1, operand);
            checkNull(reg2, operand);
            data[ptr++] = regIdentifier(reg1);
            data[ptr++] = regIdentifier(reg2);
        } else if (strcmp(operand, "psh") == 0) {
            data[ptr++] = PUSH;
            char* imm = strtok(NULL, " ");
            checkNull(imm, operand);
            int num = atoi(imm);
            data[ptr++] = num & 0xFF;
            data[ptr++] = (num >> 8) & 0xFF;
            data[ptr++] = (num >> 16) & 0xFF;
            data[ptr++] = (num >> 24) & 0xFF;
        } else if (strcmp(operand, "st") == 0) {
            data[ptr++] = STORE;
            char* address = strtok(NULL, " ");
            checkNull(address, operand);
            int num = atoi(address);
            data[ptr++] = num & 0xFF;
            data[ptr++] = (num >> 8) & 0xFF;
            data[ptr++] = (num >> 16) & 0xFF;
            data[ptr++] = (num >> 24) & 0xFF;
            char* reg = strtok(NULL, " ");
            checkNull(reg, operand);
            data[ptr++] = regIdentifier(reg);
        } else if (strcmp(operand, "ldi") == 0) {
            data[ptr++] = LOAD_IMM;
            char* reg = strtok(NULL, " ");
            checkNull(reg, operand);
            data[ptr++] = regIdentifier(reg);
            char* imm = strtok(NULL, " ");
            checkNull(imm, operand);
            if (imm[0] == '$') {
                uint32_t add = getAddressOfLabel(imm);
                data[ptr++] = add & 0xFF;
                data[ptr++] = (add >> 8) & 0xFF;
                data[ptr++] = (add >> 16) & 0xFF;
                data[ptr++] = (add >> 24) & 0xFF;
            } else {
                int num = atoi(imm);
                data[ptr++] = num & 0xFF;
                data[ptr++] = (num >> 8) & 0xFF;
                data[ptr++] = (num >> 16) & 0xFF;
                data[ptr++] = (num >> 24) & 0xFF;
            }
        } else if (strcmp(operand, "ld") == 0) {
            data[ptr++] = LOAD;
            char* reg = strtok(NULL, " ");
            checkNull(reg, operand);
            data[ptr++] = regIdentifier(reg);
            char* address = strtok(NULL, " ");
            checkNull(address, operand);
            if (address[0] == '$') {
                uint32_t add = getAddressOfLabel(address);
                data[ptr++] = add & 0xFF;
                data[ptr++] = (add >> 8) & 0xFF;
                data[ptr++] = (add >> 16) & 0xFF;
                data[ptr++] = (add >> 24) & 0xFF;
            } else {
                uint32_t num = atoll(address);
                data[ptr++] = num & 0xFF;
                data[ptr++] = (num >> 8) & 0xFF;
                data[ptr++] = (num >> 16) & 0xFF;
                data[ptr++] = (num >> 24) & 0xFF;
            }
        } else if (strcmp(operand, "pop") == 0) {
            data[ptr++] = POP;
            char* reg = strtok(NULL, " ");
            checkNull(reg, operand);
            data[ptr++] = regIdentifier(reg);
        } else if (strcmp(operand, "dup") == 0) {
            data[ptr++] = DUP;
        } else if (strcmp(operand, "swap") == 0) {
            data[ptr++] = SWAP;
        } else if (strcmp(operand, "add") == 0) {
            data[ptr++] = IADD;
            char* reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);
            data[ptr++] = regIdentifier(reg1);
        } else if (strcmp(operand, "sub") == 0) {
            data[ptr++] = ISUB;
            char* reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);
            data[ptr++] = regIdentifier(reg1);
        } else if (strcmp(operand, "mul") == 0) {
            data[ptr++] = IMUL;
            char* reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);
            data[ptr++] = regIdentifier(reg1);
        } else if (strcmp(operand, "div") == 0) {
            data[ptr++] = IDIV;
            char* reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);
            data[ptr++] = regIdentifier(reg1);
        } else if (strcmp(operand, "mod") == 0) {
            data[ptr++] = IREM;
            char* reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);
            data[ptr++] = regIdentifier(reg1);
        } else if (strcmp(operand, "neg") == 0) {
            data[ptr++] = INEG;
        } else if (strcmp(operand, "and") == 0) {
            data[ptr++] = IAND;
            char* reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);
            data[ptr++] = regIdentifier(reg1);
        } else if (strcmp(operand, "or") == 0) {
            data[ptr++] = IOR;
            char* reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);
            data[ptr++] = regIdentifier(reg1);
        } else if (strcmp(operand, "xor") == 0) {
            data[ptr++] = IXOR;
            char* reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);
            data[ptr++] = regIdentifier(reg1);
        } else if (strcmp(operand, "shl") == 0) {
            data[ptr++] = ISHL;
            char* reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);
            data[ptr++] = regIdentifier(reg1);
        } else if (strcmp(operand, "shr") == 0) {
            data[ptr++] = ISHR;
            char* reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);
            data[ptr++] = regIdentifier(reg1);
        } else if (strcmp(operand, "not") == 0) {
            data[ptr++] = INOT;
        } else if (strcmp(operand, "inc") == 0) {
            data[ptr++] = IINC;
        } else if (strcmp(operand, "dec") == 0) {
            data[ptr++] = IDEC;
        } else if (strcmp(operand, "cmp") == 0) {
            data[ptr++] = CMP;
            char* reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);
            data[ptr++] = regIdentifier(reg1);
        } else if (strcmp(operand, "goto") == 0) {
            data[ptr++] = GOTO;
            char* label = strtok(NULL, " ");
            checkNull(label, operand);
            if (label[0] == '$') {
                uint32_t add = getAddressOfLabel(label);
                data[ptr++] = add & 0xFF;
                data[ptr++] = (add >> 8) & 0xFF;
                data[ptr++] = (add >> 16) & 0xFF;
                data[ptr++] = (add >> 24) & 0xFF;
            } else {
                uint32_t add = atoi(label);
                data[ptr++] = add & 0xFF;
                data[ptr++] = (add >> 8) & 0xFF;
                data[ptr++] = (add >> 16) & 0xFF;
                data[ptr++] = (add >> 24) & 0xFF;
            }
        } else if (strcmp(operand, "jeq") == 0) {
            data[ptr++] = IF_EQ;
            char* label = strtok(NULL, " ");
            checkNull(label, operand);
            if (label[0] == '$') {
                uint32_t add = getAddressOfLabel(label);
                data[ptr++] = add & 0xFF;
                data[ptr++] = (add >> 8) & 0xFF;
                data[ptr++] = (add >> 16) & 0xFF;
                data[ptr++] = (add >> 24) & 0xFF;
            } else {
                uint32_t add = atoi(label);
                data[ptr++] = add & 0xFF;
                data[ptr++] = (add >> 8) & 0xFF;
                data[ptr++] = (add >> 16) & 0xFF;
                data[ptr++] = (add >> 24) & 0xFF;
            }
        } else if (strcmp(operand, "jne") == 0) {
            data[ptr++] = IF_NE;
            char* label = strtok(NULL, " ");
            checkNull(label, operand);
            if (label[0] == '$') {
                uint32_t add = getAddressOfLabel(label);
                data[ptr++] = add & 0xFF;
                data[ptr++] = (add >> 8) & 0xFF;
                data[ptr++] = (add >> 16) & 0xFF;
                data[ptr++] = (add >> 24) & 0xFF;
            } else {
                uint32_t add = atoi(label);
                data[ptr++] = add & 0xFF;
                data[ptr++] = (add >> 8) & 0xFF;
                data[ptr++] = (add >> 16) & 0xFF;
                data[ptr++] = (add >> 24) & 0xFF;
            }
        } else if (strcmp(operand, "jlt") == 0) {
            data[ptr++] = IF_LT;
            char* label = strtok(NULL, " ");
            checkNull(label, operand);
            if (label[0] == '$') {
                uint32_t add = getAddressOfLabel(label);
                data[ptr++] = add & 0xFF;
                data[ptr++] = (add >> 8) & 0xFF;
                data[ptr++] = (add >> 16) & 0xFF;
                data[ptr++] = (add >> 24) & 0xFF;
            } else {
                uint32_t add = atoi(label);
                data[ptr++] = add & 0xFF;
                data[ptr++] = (add >> 8) & 0xFF;
                data[ptr++] = (add >> 16) & 0xFF;
                data[ptr++] = (add >> 24) & 0xFF;
            }
        } else if (strcmp(operand, "jgt") == 0) {
            data[ptr++] = IF_GT;
            char* label = strtok(NULL, " ");
            checkNull(label, operand);
            if (label[0] == '$') {
                uint32_t add = getAddressOfLabel(label);
                data[ptr++] = add & 0xFF;
                data[ptr++] = (add >> 8) & 0xFF;
                data[ptr++] = (add >> 16) & 0xFF;
                data[ptr++] = (add >> 24) & 0xFF;
            } else {
                uint32_t add = atoi(label);
                data[ptr++] = add & 0xFF;
                data[ptr++] = (add >> 8) & 0xFF;
                data[ptr++] = (add >> 16) & 0xFF;
                data[ptr++] = (add >> 24) & 0xFF;
            }
        } else if (strcmp(operand, "jle") == 0) {
            data[ptr++] = IF_LE;
            char* label = strtok(NULL, " ");
            checkNull(label, operand);
            if (label[0] == '$') {
                uint32_t add = getAddressOfLabel(label);
                data[ptr++] = add & 0xFF;
                data[ptr++] = (add >> 8) & 0xFF;
                data[ptr++] = (add >> 16) & 0xFF;
                data[ptr++] = (add >> 24) & 0xFF;
            } else {
                uint32_t add = atoi(label);
                data[ptr++] = add & 0xFF;
                data[ptr++] = (add >> 8) & 0xFF;
                data[ptr++] = (add >> 16) & 0xFF;
                data[ptr++] = (add >> 24) & 0xFF;
            }
        } else if (strcmp(operand, "jge") == 0) {
            data[ptr++] = IF_GE;
            char* label = strtok(NULL, " ");
            checkNull(label, operand);
            if (label[0] == '$') {
                uint32_t add = getAddressOfLabel(label);
                data[ptr++] = add & 0xFF;
                data[ptr++] = (add >> 8) & 0xFF;
                data[ptr++] = (add >> 16) & 0xFF;
                data[ptr++] = (add >> 24) & 0xFF;
            } else {
                uint32_t add = atoi(label);
                data[ptr++] = add & 0xFF;
                data[ptr++] = (add >> 8) & 0xFF;
                data[ptr++] = (add >> 16) & 0xFF;
                data[ptr++] = (add >> 24) & 0xFF;
            }
        } else if (strcmp(operand, "syscall") == 0) {
            data[ptr++] = SYSTEM;
        } else if (strcmp(operand, "jsr") == 0) {
            data[ptr++] = IF_TRUE;
            char* label = strtok(NULL, " ");
            checkNull(label, operand);
            if (label[0] == '$') {
                uint32_t add = getAddressOfLabel(label);
                data[ptr++] = add & 0xFF;
                data[ptr++] = (add >> 8) & 0xFF;
                data[ptr++] = (add >> 16) & 0xFF;
                data[ptr++] = (add >> 24) & 0xFF;
            } else {
                uint32_t add = atoi(label);
                data[ptr++] = add & 0xFF;
                data[ptr++] = (add >> 8) & 0xFF;
                data[ptr++] = (add >> 16) & 0xFF;
                data[ptr++] = (add >> 24) & 0xFF;
            }
        } else if (strcmp(operand, "rts") == 0) {
            data[ptr++] = RETURN;
        } else if (strcmp(operand, "jz") == 0) {
            data[ptr++] = IF_NULL;
            char* label = strtok(NULL, " ");
            checkNull(label, operand);
            if (label[0] == '$') {
                uint32_t add = getAddressOfLabel(label);
                data[ptr++] = add & 0xFF;
                data[ptr++] = (add >> 8) & 0xFF;
                data[ptr++] = (add >> 16) & 0xFF;
                data[ptr++] = (add >> 24) & 0xFF;
            } else {
                uint32_t add = atoi(label);
                data[ptr++] = add & 0xFF;
                data[ptr++] = (add >> 8) & 0xFF;
                data[ptr++] = (add >> 16) & 0xFF;
                data[ptr++] = (add >> 24) & 0xFF;
            }
        } else if (strcmp(operand, "jnz") == 0) {
            data[ptr++] = IF_NOTNULL;
            char* label = strtok(NULL, " ");
            checkNull(label, operand);
            if (label[0] == '$') {
                uint32_t add = getAddressOfLabel(label);
                data[ptr++] = add & 0xFF;
                data[ptr++] = (add >> 8) & 0xFF;
                data[ptr++] = (add >> 16) & 0xFF;
                data[ptr++] = (add >> 24) & 0xFF;
            } else {
                uint32_t add = atoi(label);
                data[ptr++] = add & 0xFF;
                data[ptr++] = (add >> 8) & 0xFF;
                data[ptr++] = (add >> 16) & 0xFF;
                data[ptr++] = (add >> 24) & 0xFF;
            }
        } else if (strcmp(operand, ".asciiz") == 0) {
            operand = strtok(NULL, " ");
            if (operand == NULL) {
                syntax_error("Missing label after .asciiz\n");
            }
            if (strlen(operand) == 0) {
                syntax_error("Missing label after .asciiz\n");
            }
            char* str = (char*) malloc(sizeof(char) * MAX_STRING_LENGTH);
            if (operand[strlen(operand) - 1] == '"') {
                operand[strlen(operand) - 1] = '\0';
                strcpy(str, operand + 1);
                incAddr(strlen(str));
            }
            int strIndex = 0;
            if (operand[0] == '"') {
                strcpy(str, operand + 1);
            } else {
                for (int i = 0; i < strlen(operand); i++) {
                    if (operand[i] == '\\') {
                        if (operand[i + 1] == 'n') {
                            str[strIndex] = '\n';
                            i++;
                        } else if (operand[i + 1] == 't') {
                            str[strIndex] = '\t';
                            i++;
                        } else if (operand[i + 1] == '\\') {
                            str[strIndex] = '\\';
                            i++;
                        } else if (operand[i + 1] == '"') {
                            str[strIndex] = '"';
                            i++;
                        } else {
                            syntax_error("Invalid escape sequence\n");
                        }
                    } else {
                        str[strIndex] = operand[i];
                    }
                    strIndex++;
                }
                continue;
            }
            operand = strtok(NULL, " ");

            int i = 0;
            char last = operand[strlen(operand) - 1];
            char* word = (char*) malloc(sizeof(char) * MAX_STRING_LENGTH);
            const char* space = " ";

            while (last != '"') {
                if (operand == NULL) {
                    break;
                }
                if (operand[i] == '\\') {
                    if (operand[i + 1] == 'n') {
                        word[i] = '\n';
                        i++;
                    } else if (operand[i + 1] == 't') {
                        word[i] = '\t';
                        i++;
                    } else if (operand[i + 1] == '\\') {
                        word[i] = '\\';
                        i++;
                    } else if (operand[i + 1] == '"') {
                        word[i] = '"';
                        i++;
                    } else {
                        syntax_error("Invalid escape sequence\n");
                    }
                } else {
                    word[i] = operand[i];
                }
                if (i == strlen(operand)) {
                    incAddr(strlen(str) + 1);
                    operand = strtok(NULL, " ");
                    last = operand[strlen(operand) - 1];
                    i = 0;
                    strcat(str, space);
                    strcat(str, word);
                    continue;
                }
                i++;
            }
            if (operand == NULL) {
                syntax_error("Missing closing quote\n");
            }
            char* word2 = (char*) malloc(sizeof(char) * MAX_STRING_LENGTH);
            int j = 0;
            for (int i = 0; i < strlen(operand); i++) {
                if (operand[i] == '\\') {
                    if (operand[i + 1] == 'n') {
                        word2[j] = '\n';
                    } else if (operand[i + 1] == 't') {
                        word2[j] = '\t';
                    } else if (operand[i + 1] == '\\') {
                        word2[j] = '\\';
                    } else if (operand[i + 1] == '"') {
                        word2[j] = '"';
                    } else {
                        syntax_error("Invalid escape sequence\n");
                    }
                    i++;
                } else {
                    if (operand[i] == '"') {
                        break;
                    }
                    word2[j] = operand[i];
                }
                j++;
            }
            strcat(str, space);
            strcat(str, word2);

            int strLen = strlen(str);
            for (int i = 0; i < strLen; i++) {
                data[ptr++] = str[i];
            }
            data[ptr++] = '\0';
            free(str);
            free(word);
            free(word2);

        }
        #ifdef REX_FLOAT_EXT
        else if (strcmp(operand, "fldi") == 0) {
            data[ptr++] = LOAD_IMM_FLOAT;
            char* reg = strtok(NULL, " ");
            checkNull(reg, operand);
            data[ptr++] = regIdentifier(reg);
            char* imm = strtok(NULL, " ");
            checkNull(imm, operand);
            float* num = (float*)malloc(sizeof(float));
            *num = atof(imm);
            int num_int = *(int*)num;
            data[ptr++] = num_int & 0xFF;
            data[ptr++] = (num_int >> 8) & 0xFF;
            data[ptr++] = (num_int >> 16) & 0xFF;
            data[ptr++] = (num_int >> 24) & 0xFF;
        } else if (strcmp(operand, "fst") == 0) {
            data[ptr++] = STORE_FLOAT;
            char* address = strtok(NULL, " ");
            checkNull(address, operand);
            float* num = atof(address);
            int num_int = *(int*)num;
            data[ptr++] = num_int & 0xFF;
            data[ptr++] = (num_int >> 8) & 0xFF;
            data[ptr++] = (num_int >> 16) & 0xFF;
            data[ptr++] = (num_int >> 24) & 0xFF;
            char* reg = strtok(NULL, " ");
            checkNull(reg, operand);
            data[ptr++] = regIdentifier(reg);
        } else if (strcmp(operand, "fld") == 0) {
            data[ptr++] = LOAD_FLOAT;
            char* reg = strtok(NULL, " ");
            checkNull(reg, operand);
            data[ptr++] = regIdentifier(reg);
            char* address = strtok(NULL, " ");
            checkNull(address, operand);
            float* num = atof(address);
            int num_int = *(int*)num;
            data[ptr++] = num_int & 0xFF;
            data[ptr++] = (num_int >> 8) & 0xFF;
            data[ptr++] = (num_int >> 16) & 0xFF;
            data[ptr++] = (num_int >> 24) & 0xFF;
        } else if (strcmp(operand, "fadd") == 0) {
            data[ptr++] = FADD;
            char* reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);
            data[ptr++] = regIdentifier(reg1);
        } else if (strcmp(operand, "fsub") == 0) {
            data[ptr++] = FSUB;
            char* reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);
            data[ptr++] = regIdentifier(reg1);
        } else if (strcmp(operand, "fmul") == 0) {
            data[ptr++] = FMUL;
            char* reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);
            data[ptr++] = regIdentifier(reg1);
        } else if (strcmp(operand, "fdiv") == 0) {
            data[ptr++] = FDIV;
            char* reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);
            data[ptr++] = regIdentifier(reg1);
        } else if (strcmp(operand, "fmod") == 0) {
            data[ptr++] = FREM;
            char* reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);
            data[ptr++] = regIdentifier(reg1);
        } else if (strcmp(operand, "fneg") == 0) {
            data[ptr++] = FNEG;
        } else if (strcmp(operand, "fcmp") == 0) {
            data[ptr++] = FCMP;
            char* reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);
            data[ptr++] = regIdentifier(reg1);
        } else if (strcmp(operand, "finc") == 0) {
            data[ptr++] = FINC;
        } else if (strcmp(operand, "fdec") == 0) {
            data[ptr++] = FDEC;
        }
        #endif
        operand = strtok(NULL, " ");
    }

    uint8_t* code = (uint8_t*) malloc(sizeof(uint8_t) * ptr + HEADER_SIZE);
    
    code[0] = FILE_IDENTIFIER & 0xFF;
    code[1] = (FILE_IDENTIFIER >> 8) & 0xFF;
    code[2] = (FILE_IDENTIFIER >> 16) & 0xFF;
    code[3] = (FILE_IDENTIFIER >> 24) & 0xFF;

    uint32_t crc = rex_crc32(data, ptr);
    code[4] = crc & 0xFF;
    code[5] = (crc >> 8) & 0xFF;
    code[6] = (crc >> 16) & 0xFF;
    code[7] = (crc >> 24) & 0xFF;
    
    uint32_t _main = getAddressOfLabel("_main");

    code[8] = _main & 0xFF;
    code[9] = (_main >> 8) & 0xFF;
    code[10] = (_main >> 16) & 0xFF;
    code[11] = (_main >> 24) & 0xFF;
    
    // code
    for (int i = 0; i < (ptr + HEADER_SIZE); i++) {
        code[i + HEADER_SIZE] = data[i];
    }
    for (int i = 0; i < (ptr + HEADER_SIZE); i++) {
        fprintf(out, "%c", code[i]);
    }

    fclose(out);
    free(data);
    free(token);
    free(buffer);
    free(outFile);

    return 0;
}

uint32_t getAddressOfLabel(char* label) {
    if (label[0] == '$') {
        label++;
    }
    for (int i = 0; i < labelCount; i++) {
        if (strcmp(labels[i].label, label) == 0) {
            return labels[i].address;
        }
    }
    return -1;
}

int regIdentifier(char* reg) {
    if (strcmp(reg, "r0") == 0) {
        return 0;
    } else if (strcmp(reg, "r1") == 0) {
        return 1;
    } else if (strcmp(reg, "r2") == 0) {
        return 2;
    } else if (strcmp(reg, "r3") == 0) {
        return 3;
    } else if (strcmp(reg, "r4") == 0) {
        return 4;
    } else if (strcmp(reg, "r5") == 0) {
        return 5;
    } else if (strcmp(reg, "r6") == 0) {
        return 6;
    } else if (strcmp(reg, "r7") == 0) {
        return 7;
    } else if (strcmp(reg, "r8") == 0) {
        return 8;
    } else if (strcmp(reg, "r9") == 0) {
        return 9;
    } else if (strcmp(reg, "r10") == 0) {
        return 10;
    } else if (strcmp(reg, "r11") == 0) {
        return 11;
    } else if (strcmp(reg, "r12") == 0) {
        return 12;
    } else if (strcmp(reg, "r13") == 0) {
        return 13;
    } else if (strcmp(reg, "r14") == 0) {
        return 14;
    } else if (strcmp(reg, "r15") == 0) {
        return 15;
    } else {
        return 0;
    }
}

int opLength(int opcode) {
    switch (opcode) {
        case POP:
        case IADD:
        case ISUB:
        case IMUL:
        case IDIV:
        case IREM:
        case IAND:
        case IOR:
        case IXOR:
        case ISHL:
        case ISHR:
        case CMP:
            return 2;
        case PUSH:
        case GOTO:
        case IF_EQ:
        case IF_NE:
        case IF_LT:
        case IF_GT:
        case IF_LE:
        case IF_GE:
        case IF_NULL:
        case IF_NOTNULL:
        case IF_TRUE:
            return 5;
        case MOVE:
            return 3;
        case STORE:
        case LOAD_IMM:
        case LOAD:
            return 6;
    }
    return 1;
}

#ifdef __cplusplus
}
#endif