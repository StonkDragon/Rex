// check for c++
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>

#include "register.h"
#include "opcodes.h"
#include "error.h"
#include "rutil.c"

#define incAddr(amount)    currentAddress += opLength(amount)
#define MAX_LABEL_COUNT    512
#define MAX_STRING_LENGTH  16384
#define checkNull(reg)     if (reg == NULL) { error("Missing operand.\n"); return ERR_SYNTAX; } if (strlen(reg) == 0) { error("Missing operand.\n"); return ERR_SYNTAX; }

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
        error("Usage: %s <file>\n", argv[0]);
        return ERR_USAGE;
    }
    FILE* file = fopen(argv[1], "r");
    if (file == 0) {
        error("Could not open file %s\n", argv[1]);
        return ERR_IO;
    }
    char* outFile = (char*) malloc(strlen(argv[1]) - 5);
    strncpy(outFile, argv[1], strlen(argv[1]) - 5);
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* buffer = (char*)malloc(size);
    fread(buffer, size, 1, file);
    fclose(file);

    char* token = strtok(buffer, " \n");
    char** token_list = (char**)malloc(sizeof(char*) * size);
    int i = 0;
    while (token != NULL) {
        if (token[0] != ';') token_list[i] = token;
        i++;
        token = strtok(NULL, " \n");
    }
    char** tokens = (char**)malloc(sizeof(char*) * size);
    memcpy(tokens, token_list, sizeof(char*) * size);

    uint32_t currentAddress = 0;
    for (int i = 0; i < size; i++) {
        if (tokens[i] == NULL) {
            break;
        }
        if (strlen(tokens[i]) == 0) {
            continue;
        }
        char* operand = strtok(tokens[i], " ");
        if (operand == NULL) {
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
        } else if (strcmp(operand, "jmp") == 0) {
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
        } else if (strcmp(operand, "call") == 0) {
            incAddr(IF_TRUE);
        } else if (strcmp(operand, "ret") == 0) {
            incAddr(RETURN);
        } else if (strcmp(operand, "jz") == 0) {
            incAddr(IF_NULL);
        } else if (strcmp(operand, "jnz") == 0) {
            incAddr(IF_NOTNULL);
        } else if (strcmp(operand, ".here") == 0) {
            i++;
            if (tokens[i] == NULL) {
                error("Missing label after .here\n");
                return ERR_SYNTAX;
            }
            if (strlen(tokens[i]) == 0) {
                error("Missing label after .here\n");
                return ERR_SYNTAX;
            }
            rasm_label_t l = {tokens[i], currentAddress};
            labels[labelCount++] = l;
        }
    }
    
    FILE* out = fopen(outFile, "wb");
    if (out == 0) {
        error("Could not open file %s\n", outFile);
        return ERR_IO;
    }

    uint8_t* data = (uint8_t*)malloc(sizeof(uint8_t) * currentAddress + HEADER_SIZE);
    uint32_t ptr = 0;

    for (int i = 0; i < size; i++) {
        char* operand = token_list[i];
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
            char* reg1 = token_list[++i];
            char* reg2 = token_list[++i];
            checkNull(reg1);
            checkNull(reg2);
            data[ptr++] = regIdentifier(reg1);
            data[ptr++] = regIdentifier(reg2);
        } else if (strcmp(operand, "psh") == 0) {
            data[ptr++] = PUSH;
            char* imm = token_list[++i];
            checkNull(imm);
            int num = atoi(imm);
            data[ptr++] = num & 0xFF;
            data[ptr++] = (num >> 8) & 0xFF;
            data[ptr++] = (num >> 16) & 0xFF;
            data[ptr++] = (num >> 24) & 0xFF;
        } else if (strcmp(operand, "st") == 0) {
            data[ptr++] = STORE;
            char* address = token_list[++i];
            checkNull(address);
            int num = atoi(address);
            data[ptr++] = num & 0xFF;
            data[ptr++] = (num >> 8) & 0xFF;
            data[ptr++] = (num >> 16) & 0xFF;
            data[ptr++] = (num >> 24) & 0xFF;
            char* reg = token_list[++i];
            checkNull(reg);
            data[ptr++] = regIdentifier(reg);
        } else if (strcmp(operand, "ldi") == 0) {
            data[ptr++] = LOAD_IMM;
            char* reg = token_list[++i];
            checkNull(reg);
            data[ptr++] = regIdentifier(reg);
            char* imm = token_list[++i];
            checkNull(imm);
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
            char* reg = token_list[++i];
            checkNull(reg);
            data[ptr++] = regIdentifier(reg);
            char* address = token_list[++i];
            if (address == NULL) {
                error("Missing operand\n");
                return ERR_SYNTAX;
            }
            if (strlen(address) == 0) {
                error("Missing operand\n");
                return ERR_SYNTAX;
            }
            if (address[0] == '$') {
                uint64_t add = getAddressOfLabel(address);
                data[ptr++] = add & 0xFF;
                data[ptr++] = (add >> 8) & 0xFF;
                data[ptr++] = (add >> 16) & 0xFF;
                data[ptr++] = (add >> 24) & 0xFF;
            } else {
                uint64_t num = atoll(address);
                data[ptr++] = num & 0xFF;
                data[ptr++] = (num >> 8) & 0xFF;
                data[ptr++] = (num >> 16) & 0xFF;
                data[ptr++] = (num >> 24) & 0xFF;
            }
        } else if (strcmp(operand, "pop") == 0) {
            data[ptr++] = POP;
            char* reg = token_list[++i];
            checkNull(reg);
            data[ptr++] = regIdentifier(reg);
        } else if (strcmp(operand, "dup") == 0) {
            data[ptr++] = DUP;
        } else if (strcmp(operand, "swap") == 0) {
            data[ptr++] = SWAP;
        } else if (strcmp(operand, "add") == 0) {
            data[ptr++] = IADD;
            char* reg1 = token_list[++i];
            checkNull(reg1);
            data[ptr++] = regIdentifier(reg1);
        } else if (strcmp(operand, "sub") == 0) {
            data[ptr++] = ISUB;
            char* reg1 = token_list[++i];
            checkNull(reg1);
            data[ptr++] = regIdentifier(reg1);
        } else if (strcmp(operand, "mul") == 0) {
            data[ptr++] = IMUL;
            char* reg1 = token_list[++i];
            checkNull(reg1);
            data[ptr++] = regIdentifier(reg1);
        } else if (strcmp(operand, "div") == 0) {
            data[ptr++] = IDIV;
            char* reg1 = token_list[++i];
            checkNull(reg1);
            data[ptr++] = regIdentifier(reg1);
        } else if (strcmp(operand, "mod") == 0) {
            data[ptr++] = IREM;
            char* reg1 = token_list[++i];
            checkNull(reg1);
            data[ptr++] = regIdentifier(reg1);
        } else if (strcmp(operand, "neg") == 0) {
            data[ptr++] = INEG;
        } else if (strcmp(operand, "and") == 0) {
            data[ptr++] = IAND;
            char* reg1 = token_list[++i];
            checkNull(reg1);
            data[ptr++] = regIdentifier(reg1);
        } else if (strcmp(operand, "or") == 0) {
            data[ptr++] = IOR;
            char* reg1 = token_list[++i];
            checkNull(reg1);
            data[ptr++] = regIdentifier(reg1);
        } else if (strcmp(operand, "xor") == 0) {
            data[ptr++] = IXOR;
            char* reg1 = token_list[++i];
            checkNull(reg1);
            data[ptr++] = regIdentifier(reg1);
        } else if (strcmp(operand, "shl") == 0) {
            data[ptr++] = ISHL;
            char* reg1 = token_list[++i];
            checkNull(reg1);
            data[ptr++] = regIdentifier(reg1);
        } else if (strcmp(operand, "shr") == 0) {
            data[ptr++] = ISHR;
            char* reg1 = token_list[++i];
            checkNull(reg1);
            data[ptr++] = regIdentifier(reg1);
        } else if (strcmp(operand, "not") == 0) {
            data[ptr++] = INOT;
        } else if (strcmp(operand, "inc") == 0) {
            data[ptr++] = IINC;
        } else if (strcmp(operand, "dec") == 0) {
            data[ptr++] = IDEC;
        } else if (strcmp(operand, "cmp") == 0) {
            data[ptr++] = CMP;
            char* reg1 = token_list[++i];
            checkNull(reg1);
            data[ptr++] = regIdentifier(reg1);
        } else if (strcmp(operand, "jmp") == 0) {
            data[ptr++] = GOTO;
            char* label = token_list[++i];
            checkNull(label);
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
            char* label = token_list[++i];
            checkNull(label);
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
            char* label = token_list[++i];
            checkNull(label);
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
            char* label = token_list[++i];
            checkNull(label);
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
            char* label = token_list[++i];
            checkNull(label);
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
            char* label = token_list[++i];
            checkNull(label);
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
            char* label = token_list[++i];
            checkNull(label);
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
        } else if (strcmp(operand, "call") == 0) {
            data[ptr++] = IF_TRUE;
            char* label = token_list[++i];
            checkNull(label);
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
        } else if (strcmp(operand, "ret") == 0) {
            data[ptr++] = RETURN;
        } else if (strcmp(operand, "jz") == 0) {
            data[ptr++] = IF_NULL;
            char* label = token_list[++i];
            checkNull(label);
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
            char* label = token_list[++i];
            checkNull(label);
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
            i++;
            token_list[i]++;
            if (token_list[i][strlen(token_list[i]) - 1] == '"') {
                token_list[i][strlen(token_list[i]) - 1] = '\0';
            }
            for (int j = 0; j < strlen(token_list[i]); j++) {
                data[ptr++] = token_list[i][j];
            }
            while (token_list[i] != NULL) {
                i++;
                if (token_list[i] == NULL) {
                    break;
                }
                int len = strlen(token_list[i]);
                char* word = (char*) malloc(sizeof(char) * len);
                int k = 0;
                for (int j = 0; j < len; j++) {
                    if (token_list[i][j] == '\\') {
                        if (token_list[i][j+1] == 'n') {
                            word[k] = '\n';
                        } else if (token_list[i][j+1] == 't') {
                            word[k] = '\t';
                        } else if (token_list[i][j+1] == '\\') {
                            word[k] = '\\';
                        }
                        j++;
                    } else {
                        word[k] = token_list[i][j];
                    }
                    k++;
                }
                while (k < len) {
                    word[k] = 0;
                    k++;
                }
                if (word[strlen(word) - 1] == '"') {
                    word[strlen(word) - 1] = '\0';
                    data[ptr++] = ' ';
                    for (int j = 0; j < strlen(word); j++) {
                        data[ptr++] = word[j];
                    }
                    break;
                }
                data[ptr++] = ' ';
                for (int j = 0; j < strlen(word); j++) {
                    data[ptr++] = word[j];
                }
                free(word);
            }
            data[ptr++] = 0;
        }
    }

    uint8_t* code = (uint8_t*) malloc(sizeof(uint8_t) * ptr + HEADER_SIZE);
    
    code[0] = HEADER & 0xFF;
    code[1] = (HEADER >> 8) & 0xFF;
    code[2] = (HEADER >> 16) & 0xFF;
    code[3] = (HEADER >> 24) & 0xFF;

    uint32_t crc = crc32(data, ptr);
    code[4] = crc & 0xFF;
    code[5] = (crc >> 8) & 0xFF;
    code[6] = (crc >> 16) & 0xFF;
    code[7] = (crc >> 24) & 0xFF;
    
    char* mainLabel = "$_main";
    
    uint32_t _main = getAddressOfLabel(mainLabel);

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
    free(token_list);
    free(token);
    free(tokens);
    free(buffer);
    free(outFile);

    return 0;
}

uint32_t getAddressOfLabel(char* label) {
    if (label[0] == '$') {
        label++;
    } else {
        error("Invalid Label: %s", label);
        exit(-1);
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