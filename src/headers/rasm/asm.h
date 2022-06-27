#ifndef RASM_ASM_H
#define RASM_ASM_H

#include "binary.h"

#define  regIdentifier(reg) atoi(reg + 1)

uint8_t*  asm_data;
uint64_t  asm_ptr = 0;

void asm_finalize() {
    free(asm_data);
}

void asm_addInstruction(uint8_t inst, uint8_t reg1, uint8_t reg2, uint64_t address) {
    asm_data[asm_ptr++] = inst;
    asm_data[asm_ptr++] = reg1 << 4 | reg2;
    if ((inst & 0x80) != 0) {
        asm_data[asm_ptr++] = address & 0xFF;
        asm_data[asm_ptr++] = (address >> 8) & 0xFF;
        asm_data[asm_ptr++] = (address >> 16) & 0xFF;
        asm_data[asm_ptr++] = (address >> 24) & 0xFF;
        asm_data[asm_ptr++] = (address >> 32) & 0xFF;
        asm_data[asm_ptr++] = (address >> 40) & 0xFF;
        asm_data[asm_ptr++] = (address >> 48) & 0xFF;
        asm_data[asm_ptr++] = (address >> 56) & 0xFF;
    }
}

uint64_t asm_writeData(string buffer, uint64_t size) {
    asm_data = (uint8_t*) malloc(HEAP_SIZE);
    string operand = strtok(buffer, " ");
    for (uint64_t i = 0; i < size; i++) {
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
            asm_addInstruction(BREAKPOINT, 0, 0, 0);
        } else if (strcmp(operand, "nop") == 0) {
            asm_addInstruction(NOP, 0, 0, 0);
        } else if (strcmp(operand, "mov") == 0) {
            string reg1 = strtok(NULL, " ");
            string reg2 = strtok(NULL, " ");
            bin_checkNull(reg1, operand);
            bin_checkNull(reg2, operand);
            bin_checkIsRegister(reg1, operand);
            bin_checkIsRegister(reg2, operand);

            asm_addInstruction(MOVE, regIdentifier(reg1), regIdentifier(reg2), 0);
        } else if (strcmp(operand, "psi") == 0) {
            string imm = strtok(NULL, " ");
            bin_checkNull(imm, operand);
            uint64_t num = bin_labelToInt(imm, operand);

            asm_addInstruction(PUSH_IMM, 0, 0, num);
        } else if (strcmp(operand, "psh") == 0) {
            string imm = strtok(NULL, " ");
            bin_checkNull(imm, operand);
            bin_checkIsRegister(imm, operand);

            asm_addInstruction(PUSH, regIdentifier(imm), 0, 0);
        } else if (strcmp(operand, "st") == 0) {
            string address = strtok(NULL, " ");
            string reg = strtok(NULL, " ");
            bin_checkNull(address, operand);
            bin_checkIsNumber(address, operand);
            bin_checkNull(reg, operand);
            bin_checkIsRegister(reg, operand);
            uint64_t num = bin_labelToInt(address, operand);

            asm_addInstruction(STORE, regIdentifier(reg), 0, num);
        } else if (strcmp(operand, "ldi") == 0) {
            string reg = strtok(NULL, " ");
            bin_checkNull(reg, operand);
            bin_checkIsRegister(reg, operand);
            string imm = strtok(NULL, " ");
            bin_checkNull(imm, operand);
            uint64_t num = bin_labelToInt(imm, operand);

            asm_addInstruction(LOAD_IMM, regIdentifier(reg), 0, num);
        } else if (strcmp(operand, "ld") == 0) {
            string reg = strtok(NULL, " ");
            bin_checkNull(reg, operand);
            bin_checkIsRegister(reg, operand);
            string address = strtok(NULL, " ");
            bin_checkNull(address, operand);
            uint64_t num = bin_labelToInt(address, operand);

            asm_addInstruction(LOAD, regIdentifier(reg), 0, num);
        } else if (strcmp(operand, "pop") == 0) {
            string reg = strtok(NULL, " ");
            bin_checkNull(reg, operand);
            bin_checkIsRegister(reg, operand);

            asm_addInstruction(POP, regIdentifier(reg), 0, 0);
        } else if (strcmp(operand, "dup") == 0) {
            asm_addInstruction(DUP, 0, 0, 0);
        } else if (strcmp(operand, "swap") == 0) {
            asm_addInstruction(SWAP, 0, 0, 0);
        } else if (strcmp(operand, "add") == 0) {
            string reg1 = strtok(NULL, " ");
            bin_checkNull(reg1, operand);
            bin_checkIsRegister(reg1, operand);

            asm_addInstruction(IADD, regIdentifier(reg1), 0, 0);
        } else if (strcmp(operand, "sub") == 0) {
            string reg1 = strtok(NULL, " ");
            bin_checkNull(reg1, operand);
            bin_checkIsRegister(reg1, operand);

            asm_addInstruction(ISUB, regIdentifier(reg1), 0, 0);
        } else if (strcmp(operand, "mul") == 0) {            
            string reg1 = strtok(NULL, " ");
            bin_checkNull(reg1, operand);
            bin_checkIsRegister(reg1, operand);

            asm_addInstruction(IMUL, regIdentifier(reg1), 0, 0);
        } else if (strcmp(operand, "div") == 0) {
            string reg1 = strtok(NULL, " ");
            bin_checkNull(reg1, operand);
            bin_checkIsRegister(reg1, operand);
            
            asm_addInstruction(IDIV, regIdentifier(reg1), 0, 0);
        } else if (strcmp(operand, "mod") == 0) {
            string reg1 = strtok(NULL, " ");
            bin_checkNull(reg1, operand);
            bin_checkIsRegister(reg1, operand);

            asm_addInstruction(IREM, regIdentifier(reg1), 0, 0);
        } else if (strcmp(operand, "neg") == 0) {
            asm_addInstruction(INEG, 0, 0, 0);
        } else if (strcmp(operand, "and") == 0) {
            string reg1 = strtok(NULL, " ");
            bin_checkNull(reg1, operand);
            bin_checkIsRegister(reg1, operand);

            asm_addInstruction(IAND, regIdentifier(reg1), 0, 0);
        } else if (strcmp(operand, "or") == 0) {
            string reg1 = strtok(NULL, " ");
            bin_checkNull(reg1, operand);
            bin_checkIsRegister(reg1, operand);

            asm_addInstruction(IOR, regIdentifier(reg1), 0, 0);
        } else if (strcmp(operand, "xor") == 0) {
            string reg1 = strtok(NULL, " ");
            bin_checkNull(reg1, operand);
            bin_checkIsRegister(reg1, operand);

            asm_addInstruction(IXOR, regIdentifier(reg1), 0, 0);
        } else if (strcmp(operand, "shl") == 0) {
            string reg1 = strtok(NULL, " ");
            bin_checkNull(reg1, operand);

            asm_addInstruction(ISHL, regIdentifier(reg1), 0, 0);
        } else if (strcmp(operand, "shr") == 0) {
            string reg1 = strtok(NULL, " ");
            bin_checkNull(reg1, operand);
            bin_checkIsRegister(reg1, operand);

            asm_addInstruction(ISHR, regIdentifier(reg1), 0, 0);
        } else if (strcmp(operand, "not") == 0) {
            asm_addInstruction(INOT, 0, 0, 0);
        } else if (strcmp(operand, "inc") == 0) {
            asm_addInstruction(IINC, 0, 0, 0);
        } else if (strcmp(operand, "dec") == 0) {
            asm_addInstruction(IDEC, 0, 0, 0);
        } else if (strcmp(operand, "cmp") == 0) {
            string reg1 = strtok(NULL, " ");
            bin_checkNull(reg1, operand);
            bin_checkIsRegister(reg1, operand);

            asm_addInstruction(CMP, regIdentifier(reg1), 0, 0);
        } else if (strcmp(operand, "goto") == 0) {
            string label = strtok(NULL, " ");
            bin_checkNull(label, operand);
            uint64_t add = bin_labelToInt(label, operand);

            asm_addInstruction(GOTO, 0, 0, add);
        } else if (strcmp(operand, "jeq") == 0) {
            string label = strtok(NULL, " ");
            bin_checkNull(label, operand);
            uint64_t add = bin_labelToInt(label, operand);

            asm_addInstruction(JEQ, 0, 0, add);
        } else if (strcmp(operand, "jne") == 0) {
            string label = strtok(NULL, " ");
            bin_checkNull(label, operand);
            uint64_t add = bin_labelToInt(label, operand);

            asm_addInstruction(JNE, 0, 0, add);
        } else if (strcmp(operand, "jlt") == 0) {
            string label = strtok(NULL, " ");
            bin_checkNull(label, operand);
            uint64_t add = bin_labelToInt(label, operand);

            asm_addInstruction(JLT, 0, 0, add);
        } else if (strcmp(operand, "jgt") == 0) {
            string label = strtok(NULL, " ");
            bin_checkNull(label, operand);
            uint64_t add = bin_labelToInt(label, operand);

            asm_addInstruction(JGT, 0, 0, add);
        } else if (strcmp(operand, "jle") == 0) {
            string label = strtok(NULL, " ");
            bin_checkNull(label, operand);
            uint64_t add = bin_labelToInt(label, operand);

            asm_addInstruction(JLE, 0, 0, add);
        } else if (strcmp(operand, "jge") == 0) {
            string label = strtok(NULL, " ");
            bin_checkNull(label, operand);
            uint64_t add = bin_labelToInt(label, operand);

            asm_addInstruction(JGE, 0, 0, add);
        } else if (strcmp(operand, "syscall") == 0) {
            asm_addInstruction(SYSTEM, 0, 0, 0);
        } else if (strcmp(operand, "jsr") == 0) {
            string label = strtok(NULL, " ");
            bin_checkNull(label, operand);
            uint64_t add = bin_labelToInt(label, operand);

            asm_addInstruction(JMP, 0, 0, add);
        } else if (strcmp(operand, "rts") == 0) {    
            asm_addInstruction(RETURN, 0, 0, 0);
        } else if (strcmp(operand, "jz") == 0) {
            string label = strtok(NULL, " ");
            bin_checkNull(label, operand);
            uint64_t add = bin_labelToInt(label, operand);

            asm_addInstruction(JZ, 0, 0, add);
        } else if (strcmp(operand, "jnz") == 0) {
            string label = strtok(NULL, " ");
            bin_checkNull(label, operand);
            uint64_t add = bin_labelToInt(label, operand);

            asm_addInstruction(JNZ, 0, 0, add);
        } else if (strcmp(operand, ".asciiz") == 0) {
            string str = (string) malloc(MAX_STRING_LENGTH);
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
            
            size_t strLen = strlen(str);

            for (size_t i = 0; i < strLen; i++) {
                asm_data[asm_ptr++] = str[i];
            }

            asm_data[asm_ptr++] = '\0';

            free(str);
        } else if (strcmp(operand, ".at") == 0) {
            operand = strtok(NULL, " ");
            if (operand == NULL) {
                syntax_error("Missing label after .at\n");
            }
            if (strlen(operand) == 0) {
                syntax_error("Missing label after .at\n");
            }
            uint64_t pos = atoi(operand);
            if (pos < 0) {
                syntax_error("Invalid position: %llu\n", pos);
            }
            asm_ptr = pos;
        } else if (strcmp(operand, "fldi") == 0) {
            string reg = strtok(NULL, " ");
            bin_checkNull(reg, operand);
            bin_checkIsRegister(reg, operand);
            string imm = strtok(NULL, " ");
            bin_checkNull(imm, operand);
            double* num = (double*) malloc(8);
            *num = strtof(imm, NULL);
            uint64_t num_uint64_t = *(int*)num;

            asm_addInstruction(LOAD_IMM_FLOAT, regIdentifier(reg), 0, num_uint64_t);
        } else if (strcmp(operand, "fst") == 0) {
            string address = strtok(NULL, " ");
            bin_checkNull(address, operand);
            string reg = strtok(NULL, " ");
            bin_checkNull(reg, operand);
            bin_checkIsRegister(reg, operand);
            uint64_t num = bin_labelToInt(address, operand);
            
            asm_addInstruction(STORE_FLOAT, regIdentifier(reg), 0, num);
        } else if (strcmp(operand, "fld") == 0) {
            string reg = strtok(NULL, " ");
            bin_checkNull(reg, operand);
            bin_checkIsRegister(reg, operand);
            string address = strtok(NULL, " ");
            bin_checkNull(address, operand);
            uint64_t num = bin_labelToInt(address, operand);

            asm_addInstruction(LOAD_FLOAT, regIdentifier(reg), 0, num);
        } else if (strcmp(operand, "fadd") == 0) {
            string reg1 = strtok(NULL, " ");
            bin_checkNull(reg1, operand);

            asm_addInstruction(FADD, regIdentifier(reg1), 0, 0);
        } else if (strcmp(operand, "fsub") == 0) {
            string reg1 = strtok(NULL, " ");
            bin_checkNull(reg1, operand);
            
            asm_addInstruction(FSUB, regIdentifier(reg1), 0, 0);
        } else if (strcmp(operand, "fmul") == 0) {
            string reg1 = strtok(NULL, " ");
            bin_checkNull(reg1, operand);
            
            asm_addInstruction(FMUL, regIdentifier(reg1), 0, 0);
        } else if (strcmp(operand, "fdiv") == 0) {
            string reg1 = strtok(NULL, " ");
            bin_checkNull(reg1, operand);
            
            asm_addInstruction(FDIV, regIdentifier(reg1), 0, 0);
        } else if (strcmp(operand, "fneg") == 0) {
            asm_addInstruction(FNEG, 0, 0, 0);
        } else if (strcmp(operand, "fcmp") == 0) {
            string reg1 = strtok(NULL, " ");
            bin_checkNull(reg1, operand);
            
            asm_addInstruction(FCMP, regIdentifier(reg1), 0, 0);
        } else if (strcmp(operand, "finc") == 0) {
            asm_addInstruction(FINC, 0, 0, 0);
        } else if (strcmp(operand, "fdec") == 0) {
            asm_addInstruction(FDEC, 0, 0, 0);
        }
        operand = strtok(NULL, " ");
    }

    while (asm_ptr % byteAlignment != 0) {
        asm_data[asm_ptr] = 0;
        asm_ptr++;
    }

    return asm_ptr;
}

#endif
