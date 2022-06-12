# Rex Bytecode Instruction Set
## Instruction Layout
`<instruction byte> <arg imm/addr>`

Instruction Argument can be of type:
- `byte`: 8-bit unsigned integer
- `short`: 16-bit unsigned integer
- `int`: 32-bit unsigned integer

## Instructions
| Hex | Name | Macro (Defined in `opcodes.h`) | Description | Argument Type |
|-----|------|--------------------------------|-------------|---------------|
| `0x00` | `nop` | `NOP` | No Operation |  |
| `0x01` | `phn` | `PUSH_NULL` | Push Null onto Stack |  |
| `0x02` | `pho` | `PUSH_ONE` | Push 1 onto Stack |  |
| `0x03` | `phz` | `PUSH_ZERO` | Push 0 onto Stack |  |
| `0x04` | `pht` | `PUSH_TWO` | Push 2 onto Stack |  |
| `0x05` | `phth` | `PUSH_THREE` | Push 3 onto Stack |  |
| `0x06` | `phf` | `PUSH_FOUR` | Push 4 onto Stack |  |
| `0x07` | `phfi` | `PUSH_FIVE` | Push 5 onto Stack |  |
| `0x08` | `phno` | `PUSH_NEG_ONE` | Push -1 onto Stack |  |
| `0x09` | `phs` | `IPUSH` | Push 32 Bit Integer onto Stack | `int imm` |
| `0x0C` | `ld` | `GETLOCAL` | Gets the Integer at addr in ``local-buffer`` | `short addr` |
| `0x0D` | `st` | `SETLOCAL` | Sets the Integer at addr in ``local-buffer`` | `short imm` |
| `0x0E` | `lds` | `GETSTATIC` | Gets the Static Constant at addr | `short addr` |
| `0x0F` | `sts` | `SETSTATIC` | Sets the Static Constant at addr | `short imm` |
| `0x12` | `ist0` | `ISTORE_0` | Store imm in r0 | `int imm` |
| `0x13` | `ist1` | `ISTORE_1` | Store imm in r1 | `int imm` |
| `0x14` | `ist2` | `ISTORE_2` | Store imm in r2 | `int imm` |
| `0x15` | `ist3` | `ISTORE_3` | Store imm in r3 | `int imm` |
| `0x16` | `sst0` | `SSTORE_0` | Store stack[top] in r0 |  |
| `0x17` | `sst1` | `SSTORE_1` | Store stack[top] in r1 |  |
| `0x18` | `sst2` | `SSTORE_2` | Store stack[top] in r2 |  |
| `0x19` | `sst3` | `SSTORE_3` | Store stack[top] in r3 |  |
| `0x40` | `ild0` | `ILOAD_0` | Push r0 onto Stack |  |
| `0x41` | `ild1` | `ILOAD_1` | Push r1 onto Stack |  |
| `0x42` | `ild2` | `ILOAD_2` | Push r2 onto Stack |  |
| `0x43` | `ild3` | `ILOAD_3` | Push r3 onto Stack |  |
| `0x1A` | `pop` | `POP` | Pop stack[top] |  |
| `0x1B` | `pop2` | `POP2` | Pop stack[top] and stack[top-1] |  |
| `0x1C` | `dup` | `DUP` | Duplicate stack[top] |  |
| `0x1D` | `dup2` | `DUP2` | Duplicate stack[top] and stack[top-1] |  |
| `0x1E` | `swap` | `SWAP` | Swap stack[top] and stack[top-1] |  |
| `0x1F` | `add` | `IADD` | Add stack[top] and stack[top-1] |  |
| `0x20` | `sub` | `ISUB` | Subtract stack[top-1] from stack[top] |  |
| `0x21` | `mul` | `IMUL` | Multiply stack[top] by stack[top-1] |  |
| `0x22` | `div` | `IDIV` | Divide stack[top] by stack[top-1] |  |
| `0x23` | `rem` | `IREM` | Modulo stack[top] with stack[top-1] |  |
| `0x24` | `neg` | `INEG` | Negate stack[top] Value |  |
| `0x2B` | `inc` | `IINC` | Increment stack[top] Value |  |
| `0x2C` | `dec` | `IDEC` | Decrement stack[top] Value |  |
| `0x25` | `and` | `IAND` | Bitwise And of stack[top] and stack[top-1] |  |
| `0x26` | `or` | `IOR` | Bitwise Or of stack[top] and stack[top-1] |  |
| `0x27` | `xor` | `IXOR` | Bitwise Xor of stack[top] and stack[top-1] |  |
| `0x28` | `shl` | `ISHL` | Bitwise Shift Left of stack[top] Value |  |
| `0x29` | `shr` | `ISHR` | Bitwise Shift Right of stack[top] Value |  |
| `0x2A` | `not` | `INOT` | Bitwise Not of stack[top] Value |  |
| `0x2D` | `i2s` | `I2S` | Convert stack[top] Value to Short |  |
| `0x2E` | `s2i` | `S2I` | Convert stack[top] Value to Integer |  |
| `0x2F` | `cmp` | `CMP` | Compare stack[top] and stack[top-1] |  |
| `0x30` | `jmp` | `GOTO` | Jump to addr | `int addr` |
| `0x31` | `jeq` | `IF_EQ` | Jump to addr if stack[top] and stack[top-1] are equal | `int addr` |
| `0x32` | `jne` | `IF_NE` | Jump to addr if stack[top] and stack[top-1] are not equal | `int addr` |
| `0x33` | `jlt` | `IF_LT` | Jump to addr if stack[top] is less than stack[top-1] | `int addr` |
| `0x34` | `jgt` | `IF_GT` | Jump to addr if stack[top] is greater than stack[top-1] | `int addr` |
| `0x35` | `jle` | `IF_LE` | Jump to addr if stack[top] is less than or equal to stack[top-1] | `int addr` |
| `0x36` | `jge` | `IF_GE` | Jump to addr if stack[top] is greater than or equal to stack[top-1] | `int addr` |
| `0x3C` | `jz` | `IF_NULL` | Jump to addr if stack[top] is 0 | `int addr` |
| `0x3D` | `jnz` | `IF_NOTNULL` | Jump to addr if stack[top] is not 0 | `int addr` |
| `0x37` | `calls/syscall` | `CALLS` | Execute Syscall imm | `byte imm` |
| `0x38` | `call` | `CALL` | Call a subroutine | `int addr` |
| `0x3A` | `sret` | `SRETURN` | Exit with exit code from stack[top] |  |
| `0x39` | `iret` | `IRETURN` | Exit with exit code from imm | `int imm` |
| `0x3B` | `ret` | `RETURN` | Exit with exit code 0 |  |
| `0x3F` | `subr` | `SUBRETURN` | Return from `call` |  |

&nbsp;

&nbsp;

## System Calls
| ID | Macro (Defined in `rexcall.h`) | Description |
|------|-------|-------------|
| `1` | `SC_EXIT` | Exit with exit code from stack[top] |
| `2` | `SC_READ_CON` | Read a single char from stdin and push it onto the Stack |
| `3` | `SC_WRITE_CON` | Write a single char from stack[top] to stdout |
| `4` | `SC_SLEEP` | Sleep for stack[top] Seconds |
