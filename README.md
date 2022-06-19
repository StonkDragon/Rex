# Rex Bytecode Instruction Set (Version `RBIA-6`)
## Instruction Layout
Each instruction is has a fixed length of `8` bytes.

Layout:

    <byte instruction> <byte reg0> <byte reg1> <byte reg2> <int addr/imm>

Instruction Arguments can be of type:
- `byte`: 8-bit unsigned integer
- `int`: 32-bit unsigned integer

Instruction Arguments can be:
- `reg`: Register Identifier
- `imm`: Immediate Value
- `addr`: Address

## File Header
The File Header has a length of `16` bytes.

    <int magic> <int checksum> <int start> <int version>

magic: `0xCEBACEBA` (File type identifier for Rex Bytecode)
checksum: `<varies>` (Calculated by generating a crc32 checksum of the code)
start: `<varies>` (Address of the first instruction)
version: `<varies>` (Version of the compiler used to compile the code)

## Instructions
| Hex | Name | C-Macro (Defined in `opcodes.h`) | Description |
|-----|------|--------------------------------|-------------|
| `0x00` | `nop` | `NOP` | No Operation |
| `0x01` | `mov` | `MOVE` | Copies the Value in `reg1` to `reg0` |
| `0x02` | `psh` | `PUSH` | Pushes the Value in `reg0` onto the Stack |
| `0x03` | `st` | `STORE` | Stores the Value in `reg1` at `addr` |
| `0x04` | `ldi` | `LOAD_IMM` | Loads `imm` into `reg0` |
| `0x05` | `ld` | `LOAD` | Loads the Value at `addr` into `reg0` |
| `0x06` | `psi` | `PUSHI` | Pushes a 32 Bit Integer onto the Stack |
| `0x1C` | `pop` | `POP` | Pops the Value on the Stack and stores it in `reg0` |
| `0x1D` | `dup` | `DUP` | Duplicate the Value on the Stack |
| `0x1E` | `swap` | `SWAP` | Swaps the top two Values on the Stack |
| `0x1F` | `add` | `IADD` | Adds the Value in `reg0` to r0 |
| `0x20` | `sub` | `ISUB` | Subtracts the Value in `reg0` from r0 |
| `0x21` | `mul` | `IMUL` | Multiplies the Value in `reg0` with r0 |
| `0x22` | `div` | `IDIV` | Divides the Value in r0 by `reg0` |
| `0x23` | `mod` | `IREM` | Modulo the Value in r0 by `reg0` |
| `0x24` | `neg` | `INEG` | Negates the Value in r0 |
| `0x25` | `and` | `IAND` | Logical AND the Value in `reg0` and r0 |
| `0x26` | `or` | `IOR` | Logical OR the Value in `reg0` and r0 |
| `0x27` | `xor` | `IXOR` | Logical XOR the Value in `reg0` and r0 |
| `0x28` | `shl` | `ISHL` | Shifts the Value in r0 left by `reg0` |
| `0x29` | `shr` | `ISHR` | Shifts the Value in r0 right by `reg0` |
| `0x2A` | `not` | `INOT` | Inverts the Value in r0 |
| `0x2B` | `inc` | `IINC` | Increments the Value in r0 by 1 |
| `0x2C` | `dec` | `IDEC` | Decrements the Value in r0 by 1 |
| `0x2D` | `cmp` | `CMP` | Compares the Value in r0 and `reg0` |
| `0x2E` | `goto` | `GOTO` | Set the Instruction Pointer to `addr` |
| `0x2F` | `jeq` | `IF_EQ` | Push the current Instruction Pointer onto the stack and jump to `addr` if last Comparison evaluated to `equal` |
| `0x30` | `jne` | `IF_NE` | Push the current Instruction Pointer onto the stack and jump to `addr` if last Comparison evaluated to `not equal` |
| `0x31` | `jlt` | `IF_LT` | Push the current Instruction Pointer onto the stack and jump to `addr` if last Comparison evaluated to `less than` |
| `0x32` | `jgt` | `IF_GT` | Push the current Instruction Pointer onto the stack and jump to `addr` if last Comparison evaluated to `greater than` |
| `0x33` | `jle` | `IF_LE` | Push the current Instruction Pointer onto the stack and jump to `addr` if last Comparison evaluated to `not greater than` |
| `0x34` | `jge` | `IF_GE` | Push the current Instruction Pointer onto the stack and jump to `addr` if last Comparison evaluated to `not less than` |
| `0x35` | `syscall` | `SYSTEM` | Calls the System Function specified by r15 |
| `0x36` | `jsr` | `IF_TRUE` | Push the current Instruction Pointer onto the stack and jump to `addr` |
| `0x37` | `ret` | `RET` | Returns from the current Function |
| `0x38` | `jz` | `IF_NULL` | Jumps to `addr` if r0 is 0 |
| `0x39` | `jnz` | `IF_NOT_NULL` | Jumps to `addr` if r0 is not 0 |

## Memory Layout
The Program is copied into Memory starting at Address 0. All other Memory is initialized to 0.

The Default Memory Size is 16777216 (`0x1000000`) Bytes.

## System Calls
!!!Important System Calls that interact with the File System are not implemented yet.

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
