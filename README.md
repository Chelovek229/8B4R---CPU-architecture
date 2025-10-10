# 8B4R---CPU-architecture
A simple indie CPU architecture written in C++ and assembly features a minimalistic instruction set, custom memory model, and basic ALU operations. C++ handles simulation and I/O, while assembly defines low-level behavior. Ideal for learning pipelining, opcode decoding, and hardware-software integration.

## 🔧 8-bit Custom CPU Emulator

An emulator and toolchain for debugging and testing a custom 8-bit CPU architecture, developed in C++ using Visual Studio. The project includes:

- Implementation of a CPU architecture with 4 registers  
- Strict instruction set structure (ISA)  
- Console assembler `.asm → .bin`  
- Disassembler `.bin → instructions`  
- HEX dump and execution log  
- Extensible modular architecture  

---

## 🧠 CPU Architecture

### Registers

| Name  | Size  | Purpose                      |
|-------|-------|------------------------------|
| A     | 8 bit | Main accumulator             |
| B     | 8 bit | Auxiliary register           |
| C     | 8 bit | General-purpose register     |
| D     | 8 bit | General-purpose register     |
| PC    | 8 bit | Program Counter — points to current instruction |
| FLAGS | 8 bit *(planned)* | Flags: Zero, Carry, Sign, etc. *(in development)* |

---

### Memory

- **RAM**: 256 bytes  
- Addressable from `0x00` to `0xFF`  
- Program loads starting at address `0x00`  

---

### Supported Instructions (ISA)

| Mnemonic     | Opcode | Description                        |
|--------------|--------|------------------------------------|
| `NOP`        | 0x00   | No operation                       |
| `MOV A, B`   | 0x01   | Copies value from B to A           |
| `ADD A, B`   | 0x02   | Adds A and B, stores result in A   |
| `INC A`      | 0x03   | Increments A by 1                  |
| `HLT`        | 0xFF   | Halts execution                    |

> 💡 The architecture is easily extensible — you can add `JMP`, `SUB`, `MOV A, [addr]`, `CALL`, `RET`, and even I/O ports via `IN/OUT`.

---

## 📂 Project Structure

```
8B4R/
├── include/
│   ├── cpu.hpp
│   ├── instructions.hpp
│   ├── assembler.hpp
│   └── disassembler.hpp
├── src/
│   ├── main.cpp
│   ├── cpu.cpp
│   ├── instructions.cpp
│   ├── assembler.cpp
│   └── disassembler.cpp
├── tools/
│   ├── test_program.asm
│   ├── test_program.bin
│   └── generate_test_bin.cpp
├── output/
│   ├── log.txt
│   └── memory_dump.txt
└── README.md
```

---

## 🛠 Build Instructions

### 🔹 Visual Studio

1. Open `8B4R.sln` or create a new console project  
2. Add all `.cpp` and `include/*.hpp` files to the project  
3. Set the `include` folder as **additional include directories**  
4. Build and run the project  

---

## ⚙️ Usage

### Program Assembly

```asm
; tools/test_program.asm
MOV A, B
ADD A, B
INC A
HLT

### - Badges
![🟊 Curator](https://img.shields.io/badge/Role-Curator-yellow?style=flat-square)
![💻 Coder](https://img.shields.io/badge/Role-Coder-blue?style=flat-square)
