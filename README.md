# Rex Bytecode Instruction Set (Version `RBIA-3`)
## Instruction Layout
`<instruction byte> <arg0 imm/addr> <arg1 imm/addr>`

Instruction Arguments can be of type:
- `byte`: 8-bit unsigned integer
- `int`: 32-bit unsigned integer

Instruction Arguments can be:
- `reg`: Register Identifier
- `imm`: Immediate Value
- `addr`: Address

## Instructions
| Hex | Name | C-Macro (Defined in `opcodes.h`) | Description | Argument Type |
|-----|------|--------------------------------|-------------|---------------|
| `0x00` | `nop` | `NOP` | No Operation | `none` |
| `0x01` | `mov` | `MOVE` | Copies the Value in `reg arg1` to `reg arg0` | `byte, byte` |
| `0x02` | `psh` | `PUSH` | Pushes a 32 Bit Integer onto the Stack | `int` |
| `0x03` | `st` | `STORE` | Stores the Value in `reg arg1` at `addr arg0` | `int, byte` |
| `0x04` | `ldi` | `LOAD_IMM` | Loads `imm arg1` into `reg arg0` | `byte, int` |
| `0x05` | `ld` | `LOAD` | Loads the Value at `addr arg1` into `reg arg0` | `byte, int` |
| `0x1C` | `pop` | `POP` | Pops the Value on the Stack and stores it in `reg arg0` | `byte` |
| `0x1D` | `dup` | `DUP` | Duplicate the Value on the Stack | `none` |
| `0x1E` | `swap` | `SWAP` | Swaps the top two Values on the Stack | `none` |
| `0x1F` | `add` | `IADD` | Adds the Value in `reg arg0` to r0 | `byte` |
| `0x20` | `sub` | `ISUB` | Subtracts the Value in `reg arg0` from r0 | `byte` |
| `0x21` | `mul` | `IMUL` | Multiplies the Value in `reg arg0` with r0 | `byte` |
| `0x22` | `div` | `IDIV` | Divides the Value in r0 by `reg arg0` | `byte` |
| `0x23` | `mod` | `IREM` | Modulo the Value in r0 by `reg arg0` | `byte` |
| `0x24` | `neg` | `INEG` | Negates the Value in r0 | `none` |
| `0x25` | `and` | `IAND` | Logical AND the Value in `reg arg0` and r0 | `byte` |
| `0x26` | `or` | `IOR` | Logical OR the Value in `reg arg0` and r0 | `byte` |
| `0x27` | `xor` | `IXOR` | Logical XOR the Value in `reg arg0` and r0 | `byte` |
| `0x28` | `shl` | `ISHL` | Shifts the Value in r0 left by `reg arg0` | `byte` |
| `0x29` | `shr` | `ISHR` | Shifts the Value in r0 right by `reg arg0` | `byte` |
| `0x2A` | `not` | `INOT` | Inverts the Value in r0 | `none` |
| `0x2B` | `inc` | `IINC` | Increments the Value in r0 by 1 | `byte` |
| `0x2C` | `dec` | `IDEC` | Decrements the Value in r0 by 1 | `byte` |
| `0x2D` | `cmp` | `CMP` | Compares the Value in r0 and `reg arg0` | `byte` |
| `0x2E` | `jmp` | `GOTO` | Jumps to `addr arg0` | `int` |
| `0x2F` | `jeq` | `IF_EQ` | Jumps to `addr arg0` if last Comparison evaluated to `equal` | `int` |
| `0x30` | `jne` | `IF_NE` | Jumps to `addr arg0` if last Comparison evaluated to `not equal` | `int` |
| `0x31` | `jlt` | `IF_LT` | Jumps to `addr arg0` if last Comparison evaluated to `less than` | `int` |
| `0x32` | `jgt` | `IF_GT` | Jumps to `addr arg0` if last Comparison evaluated to `greater than` | `int` |
| `0x33` | `jle` | `IF_LE` | Jumps to `addr arg0` if last Comparison evaluated to `not greater than` | `int` |
| `0x34` | `jge` | `IF_GE` | Jumps to `addr arg0` if last Comparison evaluated to `not less than` | `int` |
| `0x35` | `syscall` | `SYSTEM` | Calls the System Function specified by r15 | `none` |
| `0x36` | `call` | `IF_TRUE` | Calls the Function specified by `addr arg0` | `int` |
| `0x37` | `ret` | `RET` | Returns from the current Function | `none` |
| `0x38` | `jz` | `IF_NULL` | Jumps to `addr arg0` if Value on the Stack is 0 | `int` |
| `0x39` | `jnz` | `IF_NOT_NULL` | Jumps to `addr arg0` if Value on the Stack is not 0 | `int` |

## Memory Layout
The Program is copied into Memory starting at Address 0. All other Memory is initialized to 0.

The Default Memory Size is 131072 (`0x20000`) Cells with each Cell being 32-bit wide.

### Reserved Memory Locations:
| Address | Name | Description |
|---------|------|-------------|
| `0x1FFFF` | `r0` | Register 0 |
| `0x1FFFE` | `r1` | Register 1 |
| `0x1FFFD` | `r2` | Register 2 |
| `0x1FFFC` | `r3` | Register 3 |
| `0x1FFFB` | `r4` | Register 4 |
| `0x1FFFA` | `r5` | Register 5 |
| `0x1FFF9` | `r6` | Register 6 |
| `0x1FFF8` | `r7` | Register 7 |
| `0x1FFF7` | `r8` | Register 8 |
| `0x1FFF6` | `r9` | Register 9 |
| `0x1FFF5` | `r10` | Register 10 |
| `0x1FFF4` | `r11` | Register 11 |
| `0x1FFF3` | `r12` | Register 12 |
| `0x1FFF2` | `r13` | Register 13 |
| `0x1FFF1` | `r14` | Register 14 |
| `0x1FFF0` | `r15` | Register 15 |
| `0x1FFEF` | `ip` | Instruction Pointer |
| `0x1FFEE` | `sp` | Stack Pointer |
| `0x1FFED` | `fr` | Flags Register |
| `0x1FFEC` | `addr` | Instruction Address Register (Used to temporarily store `int` Arguments) |


## System Calls
| ID | C-Macro (Defined in `rexcall.h`) | Description |
|------|-------|-------------|
| `1` | `SC_EXIT` | Exit with exit code from r0 |
| `2` | `SC_READ_CON` | Read a Line from stdin and store it at addr in r0 |
| `3` | `SC_WRITE_CON` | Write a null (`\0`) terminated String from addr in r0 to stdout |
| `4` | `SC_SLEEP` | Sleep for Value in r0 Seconds |
| `5` | `SC_OPEN` | Open File by Name stored at addr in r0 with flags stored in r1 |
| `6` | `SC_CLOSE` | Close File by File Descriptor stored in r0 |
| `7` | `SC_READ` | Read from File by File Descriptor stored in r0 with length stored in r1 to addr in r2 |
| `8` | `SC_WRITE` | Write to File by File Descriptor stored in r0 with length stored in r1 from String at addr in r1 |
| `9` | `SC_EXEC` | Execute System Command stored at addr in r0 and push the Return Value onto the Stack |
