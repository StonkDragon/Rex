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

#include "headers/cstring.h"
#include "headers/register.h"
#include "headers/opcodes.h"
#include "headers/error.h"
#include "headers/crc32.h"
#include "headers/memoryengine.h"

#define incAddr(amount)      currentAddress += opLength(amount)
#define MAX_LABEL_COUNT      512
#define checkNull(reg, inst) if (reg == NULL) { syntax_error("Missing operand. (Instruction: %s)\n", inst); } if (strlen(reg) == 0) { syntax_error("Missing operand. (Instruction: %s)\n", inst); }
#define regIdentifier(reg)   atoi(reg + 1)

typedef struct {
    string label;
    uint32_t address;
} rasm_label_t;

int           opLength(int op);
uint32_t      getAddressOfLabel(string label);
void          addInstruction(uint8_t inst, uint8_t reg1, uint8_t reg2, uint8_t reg3, uint32_t address);
uint32_t      labelToInt(string label);

rasm_label_t* labels = (rasm_label_t*) malloc(sizeof(rasm_label_t) * MAX_LABEL_COUNT);
uint8_t*      data = (uint8_t*)malloc(sizeof(uint8_t) * HEAP_SIZE + HEADER_SIZE);
uint32_t      labelCount = 0;
uint32_t      ptr = 0;

int main(int argc, string argv[]) {
    if (argc != 2) {
        usage_error("%s <file>\n", argv[0]);
    }
    FILE* file = fopen(argv[1], "r");
    if (file == 0) {
        native_error("Could not open file %s\n", argv[1]);
    }
    string outFile = (string) malloc(strlen(argv[1]) - 3);
    strncpy(outFile, argv[1], strlen(argv[1]) - 5);
    strcat(outFile, ".rx");
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    string file_buffer = (string)malloc(size);
    fread(file_buffer, size, 1, file);
    fclose(file);

    string token = strtok(file_buffer, " \n");
    string token_buffer = (string)malloc(sizeof(char) * size);
    string buffer = (string)malloc(sizeof(char) * size);
    int i = 0;
    uint32_t byteAlignment = 16;
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
    string operand = strtok(token_buffer, " ");
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
            operand = strtok(NULL, " ");
            if (operand == NULL) {
                syntax_error("Missing label after .asciiz\n");
            }
            if (strlen(operand) == 0) {
                syntax_error("Missing label after .asciiz\n");
            }
            string str = (string) malloc(sizeof(char) * MAX_STRING_LENGTH);
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
            string word = (string) malloc(sizeof(char) * MAX_STRING_LENGTH);
            const string space = " ";

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
            string word2 = (string) malloc(sizeof(char) * MAX_STRING_LENGTH);
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

    FILE* out = fopen(outFile, "wb");
    if (out == 0) {
        native_error("Could not open file %s\n", outFile);
    }

    memset(data, 0, sizeof(uint8_t) * HEAP_SIZE + HEADER_SIZE);

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
            addInstruction(BREAKPOINT, 0, 0, 0, 0);
        } else if (strcmp(operand, "nop") == 0) {
            addInstruction(NOP, 0, 0, 0, 0);
        } else if (strcmp(operand, "mov") == 0) {
            string reg1 = strtok(NULL, " ");
            string reg2 = strtok(NULL, " ");
            checkNull(reg1, operand);
            checkNull(reg2, operand);

            addInstruction(MOVE, regIdentifier(reg1), regIdentifier(reg2), 0, 0);
        } else if (strcmp(operand, "psi") == 0) {
            string imm = strtok(NULL, " ");
            checkNull(imm, operand);
            int num = atoi(imm);

            addInstruction(STORE, 0, 0, 0, num);
        } else if (strcmp(operand, "psh") == 0) {
            string imm = strtok(NULL, " ");
            checkNull(imm, operand);

            addInstruction(PUSH, regIdentifier(imm), 0, 0, 0);
        } else if (strcmp(operand, "st") == 0) {
            string address = strtok(NULL, " ");
            string reg = strtok(NULL, " ");
            checkNull(address, operand);
            checkNull(reg, operand);
            int num = atoi(address);

            addInstruction(STORE, regIdentifier(reg), 0, 0, num);
        } else if (strcmp(operand, "ldi") == 0) {
            string reg = strtok(NULL, " ");
            checkNull(reg, operand);
            string imm = strtok(NULL, " ");
            checkNull(imm, operand);
            uint32_t num = labelToInt(imm);

            addInstruction(LOAD_IMM, regIdentifier(reg), 0, 0, num);
        } else if (strcmp(operand, "ld") == 0) {
            string reg = strtok(NULL, " ");
            checkNull(reg, operand);
            string address = strtok(NULL, " ");
            checkNull(address, operand);
            uint32_t num = labelToInt(address);

            addInstruction(LOAD, regIdentifier(reg), 0, 0, num);
        } else if (strcmp(operand, "pop") == 0) {
            string reg = strtok(NULL, " ");
            checkNull(reg, operand);

            addInstruction(POP, regIdentifier(reg), 0, 0, 0);
        } else if (strcmp(operand, "dup") == 0) {
            addInstruction(DUP, 0, 0, 0, 0);
        } else if (strcmp(operand, "swap") == 0) {
            addInstruction(SWAP, 0, 0, 0, 0);
        } else if (strcmp(operand, "add") == 0) {
            string reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);

            addInstruction(IADD, regIdentifier(reg1), 0, 0, 0);
        } else if (strcmp(operand, "sub") == 0) {
            string reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);

            addInstruction(ISUB, regIdentifier(reg1), 0, 0, 0);
        } else if (strcmp(operand, "mul") == 0) {            
            string reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);

            addInstruction(IMUL, regIdentifier(reg1), 0, 0, 0);
        } else if (strcmp(operand, "div") == 0) {
            string reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);
            
            addInstruction(IDIV, regIdentifier(reg1), 0, 0, 0);
        } else if (strcmp(operand, "mod") == 0) {
            string reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);

            addInstruction(IREM, regIdentifier(reg1), 0, 0, 0);
        } else if (strcmp(operand, "neg") == 0) {
            addInstruction(INEG, 0, 0, 0, 0);
        } else if (strcmp(operand, "and") == 0) {
            string reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);

            addInstruction(IAND, regIdentifier(reg1), 0, 0, 0);
        } else if (strcmp(operand, "or") == 0) {
            string reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);

            addInstruction(IOR, regIdentifier(reg1), 0, 0, 0);
        } else if (strcmp(operand, "xor") == 0) {
            string reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);

            addInstruction(IXOR, regIdentifier(reg1), 0, 0, 0);
        } else if (strcmp(operand, "shl") == 0) {
            string reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);

            addInstruction(ISHL, regIdentifier(reg1), 0, 0, 0);
        } else if (strcmp(operand, "shr") == 0) {
            string reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);

            addInstruction(ISHR, regIdentifier(reg1), 0, 0, 0);
        } else if (strcmp(operand, "not") == 0) {
            addInstruction(INOT, 0, 0, 0, 0);
        } else if (strcmp(operand, "inc") == 0) {
            addInstruction(IINC, 0, 0, 0, 0);
        } else if (strcmp(operand, "dec") == 0) {
            addInstruction(IDEC, 0, 0, 0, 0);
        } else if (strcmp(operand, "cmp") == 0) {
            string reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);

            addInstruction(CMP, regIdentifier(reg1), 0, 0, 0);
        } else if (strcmp(operand, "goto") == 0) {
            string label = strtok(NULL, " ");
            checkNull(label, operand);
            uint32_t add = labelToInt(label);

            addInstruction(GOTO, 0, 0, 0, add);
        } else if (strcmp(operand, "jeq") == 0) {
            string label = strtok(NULL, " ");
            checkNull(label, operand);
            uint32_t add = labelToInt(label);

            addInstruction(IF_EQ, 0, 0, 0, add);
        } else if (strcmp(operand, "jne") == 0) {
            string label = strtok(NULL, " ");
            checkNull(label, operand);
            uint32_t add = labelToInt(label);

            addInstruction(IF_NE, 0, 0, 0, add);
        } else if (strcmp(operand, "jlt") == 0) {
            string label = strtok(NULL, " ");
            checkNull(label, operand);
            uint32_t add = labelToInt(label);

            addInstruction(IF_LT, 0, 0, 0, add);
        } else if (strcmp(operand, "jgt") == 0) {
            string label = strtok(NULL, " ");
            checkNull(label, operand);
            uint32_t add = labelToInt(label);

            addInstruction(IF_GT, 0, 0, 0, add);
        } else if (strcmp(operand, "jle") == 0) {
            string label = strtok(NULL, " ");
            checkNull(label, operand);
            uint32_t add = labelToInt(label);

            addInstruction(IF_LE, 0, 0, 0, add);
        } else if (strcmp(operand, "jge") == 0) {
            string label = strtok(NULL, " ");
            checkNull(label, operand);
            uint32_t add = labelToInt(label);

            addInstruction(IF_GE, 0, 0, 0, add);
        } else if (strcmp(operand, "syscall") == 0) {
            addInstruction(SYSTEM, 0, 0, 0, 0);
        } else if (strcmp(operand, "jsr") == 0) {
            string label = strtok(NULL, " ");
            checkNull(label, operand);
            uint32_t add = labelToInt(label);

            addInstruction(IF_TRUE, 0, 0, 0, add);
        } else if (strcmp(operand, "rts") == 0) {    
            addInstruction(RETURN, 0, 0, 0, 0);
        } else if (strcmp(operand, "jz") == 0) {
            string label = strtok(NULL, " ");
            checkNull(label, operand);
            uint32_t add = labelToInt(label);

            addInstruction(IF_NULL, 0, 0, 0, add);
        } else if (strcmp(operand, "jnz") == 0) {
            string label = strtok(NULL, " ");
            checkNull(label, operand);
            uint32_t add = labelToInt(label);

            addInstruction(IF_NOTNULL, 0, 0, 0, add);
        } else if (strcmp(operand, ".asciiz") == 0) {
            operand = strtok(NULL, " ");
            if (operand == NULL) {
                syntax_error("Missing label after .asciiz\n");
            }
            if (strlen(operand) == 0) {
                syntax_error("Missing label after .asciiz\n");
            }
            string str = (string) malloc(sizeof(char) * MAX_STRING_LENGTH);
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
            string word = (string) malloc(sizeof(char) * MAX_STRING_LENGTH);
            const string space = " ";

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
            string word2 = (string) malloc(sizeof(char) * MAX_STRING_LENGTH);
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
            ptr = pos;
        }
        #ifdef REX_FLOAT_EXT
        else if (strcmp(operand, "fldi") == 0) {
            string reg = strtok(NULL, " ");
            checkNull(reg, operand);
            string imm = strtok(NULL, " ");
            checkNull(imm, operand);
            float* num = (float*)malloc(sizeof(float));
            *num = atof(imm);
            int num_int = *(int*)num;

            addInstruction(LOAD_IMM_FLOAT, regIdentifier(reg1), 0, 0, num_int);
        } else if (strcmp(operand, "fst") == 0) {
            string reg = strtok(NULL, " ");
            checkNull(reg, operand);
            string address = strtok(NULL, " ");
            checkNull(address, operand);
            float* num = atof(address);
            int num_int = *(int*)num;
            
            addInstruction(STORE_FLOAT, regIdentifier(reg1), 0, 0, num_int);
        } else if (strcmp(operand, "fld") == 0) {
            string reg = strtok(NULL, " ");
            checkNull(reg, operand);
            string address = strtok(NULL, " ");
            checkNull(address, operand);
            float* num = atof(address);
            int num_int = *(int*)num;

            addInstruction(LOAD_FLOAT, regIdentifier(reg1), 0, 0, num_int);
        } else if (strcmp(operand, "fadd") == 0) {
            string reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);

            addInstruction(FADD, regIdentifier(reg1), 0, 0, 0);
        } else if (strcmp(operand, "fsub") == 0) {
            string reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);
            
            addInstruction(FSUB, regIdentifier(reg1), 0, 0, 0);
        } else if (strcmp(operand, "fmul") == 0) {
            string reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);
            
            addInstruction(FMUL, regIdentifier(reg1), 0, 0, 0);
        } else if (strcmp(operand, "fdiv") == 0) {
            string reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);
            
            addInstruction(FDIV, regIdentifier(reg1), 0, 0, 0);
        } else if (strcmp(operand, "fmod") == 0) {
            string reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);
            
            addInstruction(FREM, regIdentifier(reg1), 0, 0, 0);
        } else if (strcmp(operand, "fneg") == 0) {
            addInstruction(FNEG, 0, 0, 0, 0);
        } else if (strcmp(operand, "fcmp") == 0) {
            string reg1 = strtok(NULL, " ");
            checkNull(reg1, operand);
            
            addInstruction(FCMP, regIdentifier(reg1), 0, 0, 0);
        } else if (strcmp(operand, "finc") == 0) {
            addInstruction(FINC, 0, 0, 0, 0);
        } else if (strcmp(operand, "fdec") == 0) {
            addInstruction(FDEC, 0, 0, 0, 0);
        }
        #endif
        operand = strtok(NULL, " ");
    }

    while ((ptr + HEADER_SIZE) % byteAlignment != 0) {
        data[ptr + HEADER_SIZE] = 0;
        ptr++;
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

    uint32_t version = REX_COMPILER_VER;
    code[12] = version & 0xFF;
    code[13] = (version >> 8) & 0xFF;
    code[14] = (version >> 16) & 0xFF;
    code[15] = (version >> 24) & 0xFF;
    
    for (int i = 0; i < (ptr + HEADER_SIZE); i++) {
        code[i + HEADER_SIZE] = data[i];
    }

    for (int i = 0; i < (ptr + HEADER_SIZE); i++) {
        fprintf(out, "%c", code[i]);
    }
    
    #ifdef DEBUG
    string debug = (string)malloc(sizeof(char) * strlen(outFile) + 5);
    strcpy(debug, outFile);
    strcat(debug, ".dsym");
    FILE* dsym = fopen(debug, "w");
    for (int i = 0; i < labelCount; i++) {
        fprintf(dsym, "%08x:%s\n", labels[i].address, labels[i].label);
    }
    fclose(dsym);
    free(debug);
    #endif

    fclose(out);
    free(data);
    free(token);
    free(buffer);
    free(outFile);

    return 0;
}

uint32_t getAddressOfLabel(string label) {
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

int opLength(int opcode) {
    switch (opcode) {
        default:
            return 8;
    }
}

void addInstruction(uint8_t inst, uint8_t reg1, uint8_t reg2, uint8_t reg3, uint32_t address) {
    data[ptr++] = inst;
    data[ptr++] = reg1;
    data[ptr++] = reg2;
    data[ptr++] = reg3;
    data[ptr++] = address & 0xFF;
    data[ptr++] = (address >> 8) & 0xFF;
    data[ptr++] = (address >> 16) & 0xFF;
    data[ptr++] = (address >> 24) & 0xFF;
}

uint32_t labelToInt(string label) {
    uint32_t num = 0;
    if (label[0] == '$') {
        num = getAddressOfLabel(label);
    } else {
        num = atoi(label);
    }
    return num;
}

#ifdef __cplusplus
}
#endif