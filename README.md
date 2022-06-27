# Rex Bytecode Instruction Set (Version `RBIA-8`)
## Instruction Layout
Each instruction has a fixed length either of `2` or `10` bytes.

Layout:

    <byte instruction> <reg0> <reg1> <long addr/imm>

Instruction Arguments can be of type:
- `byte`: 8-bit unsigned integer
- `long`: 64-bit unsigned integer

Instruction Arguments can be:
- `reg`: Register Identifier
- `imm`: Immediate Value
- `addr`: Address

## Instructions
| Name | C-Macro (Defined in `opcodes.h`) | Description |
|------|--------------------------------|-------------|
| `nop` | `NOP` | No Operation |
| `mov` | `MOVE` | Copies the Value in `reg1` to `reg0` |
| `psh` | `PUSH` | Pushes the Value in `reg0` onto the Stack |
| `st` | `STORE` | STOREs the Value in `reg1` at `addr` |
| `ldi` | `LOAD_IMM` | LOADs `imm` into `reg0` |
| `ld` | `LOAD` | LOADs the Value at `addr` into `reg0` |
| `psi` | `PUSH_IMM` | Pushes `imm` onto the Stack |
| `pop` | `POP` | Pops the Value on the Stack and STOREs it in `reg0` |
| `dup` | `DUP` | Duplicate the Value on the Stack |
| `swap` | `SWAP` | Swaps the top two Values on the Stack |
| `add` | `IADD` | Adds the Value in `reg0` to r0 |
| `sub` | `ISUB` | Subtracts the Value in `reg0` from r0 |
| `mul` | `IMUL` | Multiplies the Value in `reg0` with r0 |
| `div` | `IDIV` | Divides the Value in r0 by `reg0` |
| `mod` | `IREM` | Modulo the Value in r0 by `reg0` |
| `neg` | `INEG` | Negates the Value in r0 |
| `and` | `IAND` | Logical AND the Value in `reg0` and r0 |
| `or` | `IOR` | Logical OR the Value in `reg0` and r0 |
| `xor` | `IXOR` | Logical XOR the Value in `reg0` and r0 |
| `shl` | `ISHL` | Shifts the Value in r0 left by `reg0` |
| `shr` | `ISHR` | Shifts the Value in r0 right by `reg0` |
| `not` | `INOT` | Inverts the Value in r0 |
| `inc` | `IINC` | Increments the Value in r0 by 1 |
| `dec` | `IDEC` | Decrements the Value in r0 by 1 |
| `cmp` | `CMP` | Compares the Value in r0 and `reg0` |
| `goto` | `GOTO` | Set the Instruction Pointer to `addr` |
| `jeq` | `JEQ` | Push the current Instruction Pointer onto the stack and jump to `addr` if last Comparison evaluated to `equal` |
| `jne` | `JNE` | Push the current Instruction Pointer onto the stack and jump to `addr` if last Comparison evaluated to `not equal` |
| `jlt` | `JLT` | Push the current Instruction Pointer onto the stack and jump to `addr` if last Comparison evaluated to `less than` |
| `jgt` | `JGT` | Push the current Instruction Pointer onto the stack and jump to `addr` if last Comparison evaluated to `greater than` |
| `jle` | `JLE` | Push the current Instruction Pointer onto the stack and jump to `addr` if last Comparison evaluated to `not greater than` |
| `jge` | `JGE` | Push the current Instruction Pointer onto the stack and jump to `addr` if last Comparison evaluated to `not less than` |
| `syscall` | `SYSTEM` | Calls the System Function specified by r15 |
| `jsr` | `JMP` | Push the current Instruction Pointer onto the stack and jump to `addr` |
| `ret` | `RET` | Returns from the current Function |
| `jz` | `JZ` | Jumps to `addr` if r0 is 0 |
| `jnz` | `IF_NOT_NULL` | Jumps to `addr` if r0 is not 0 |
| `fldi` | `LOAD_IMM_FLOAT` | LOAD the floating point `imm` into `reg0` |
| `fst` | `STORE_FLOAT` | STORE the floating point value in `reg0` at `addr` |
| `fld` | `LOAD_FLOAT` | LOAD the floating point value at `addr` into `reg0` |
| `fadd` | `FADD` | Add the floating point value in `reg0` to f0 |
| `fsub` | `FSUB` | Subtract the floating point value in `reg0` from f0 |
| `fmul` | `FMUL` | Multiply the floating point value in `reg0` with f0 |
| `fdiv` | `FDIV` | Divide the floating point value in `reg0` by f0 |
| `fneg` | `FNEG` | Negate the floating point value in f0 |
| `fcmp` | `FCMP` | Compare the floating point value in f0 and `reg0` |
| `finc` | `FINC` | Increment the floating point value in f0 by 1.0 |
| `fdec` | `FDEC` | Decrement the floating point value in f0 by 1.0 |

## File Header
The File Header has a length of `16` bytes.

    <int magic> <int checksum> <long start>

magic: `0xCEBACEBA` (File type identifier for Rex Bytecode)

checksum: `<varies>` (Calculated by generating a crc32 checksum of the code)

start: `<varies>` (Address of the first instruction)

## Memory Layout
The Program is copied into Memory starting at Address 0. All other Memory is initialized to 0.

The Default Memory Size is 16777216 (`0x1000000`) Bytes.

## Registers

There are 16 General Purpose 64-Bit Registers (`r0` - `r15`).

Furthermore, there are 8 64-Bit Floating Point Registers (`f0` - `f7`).

## System Calls

| ID | C-Macro (Defined in `rexcall.h`) | Description |
|------|-------|-------------|
| `1` | `SC_EXIT` | Exit with exit code from r0 |
| `2` | `SC_READ` | Read from File by File Descriptor stored in r0 to addr in r2 |
| `3` | `SC_WRITE` | Write to File by File Descriptor stored in r0 from addr in r2 |
| `4` | `SC_SLEEP` | Sleep for Value in r0 Seconds |
| `5` | `SC_OPEN` | Open File by Name STOREd at addr in r0 with flags STOREd in r1 |
| `6` | `SC_CLOSE` | Close File by File Descriptor STOREd in r0 |
| `7` | `SC_EXEC` | Execute System Command STOREd at addr in r0 and push the Return Value onto the Stack |
