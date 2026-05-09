<div align="center">

# ⚡ Shell-to-Executable⚡

### Convert Scripts Into Native Executables

<img src="https://readme-typing-svg.herokuapp.com?font=Fira+Code&size=24&duration=3000&pause=1000&color=00FFAA&center=true&vCenter=true&width=700&lines=.BAT+%E2%9E%9C+.EXE;.PS1+%E2%9E%9C+.EXE;.SH+%E2%9E%9C+LINUX+BINARIES;Powered+By+TinyCC+%2B+GCC;Author%3A+Dr+Sanne+Karibo" />

---

<img src="https://img.shields.io/badge/C-Language-blue?style=for-the-badge"/>
<img src="https://img.shields.io/badge/Linux-Supported-success?style=for-the-badge"/>
<img src="https://img.shields.io/badge/Windows-Supported-success?style=for-the-badge"/>
<img src="https://img.shields.io/badge/TinyCC-Embedded-orange?style=for-the-badge"/>
<img src="https://img.shields.io/badge/GCC-Native-red?style=for-the-badge"/>

</div>

---

# 🚀 About

**Shell-to-Executable** is a lightweight cross-platform utility written in pure C that transforms shell and command scripts into executable launcher binaries.

The project automatically:

- Reads script files
- Generates native wrapper code
- Compiles wrappers internally
- Produces launchable executables

---

# ✨ Features

## 🪟 Windows

Converts:

- `.bat` ➜ `.exe`
- `.cmd` ➜ `.exe`
- `.ps1` ➜ `.exe`

Uses:

- Embedded TinyCC (`libtcc`)

---

## 🐧 Linux

Converts:

- `.sh` ➜ native executable

Uses:

- GCC backend

---

# ⚡ Auto Folder Compilation

Place scripts beside:

```text
compile.exe
```

or:

```text
compile
```

Run:

```bash
./compile
```

or click:

```text
compile.exe
```

Automatically converts all supported scripts in the current directory.

---

# 🎯 Single File Mode

Compile only one script:

```bash
compile hello.bat
```

```bash
./compile run.sh
```

---

# 🧠 How It Works

```text
Script File
    ↓
Read Into Memory
    ↓
Generate Wrapper C Code
    ↓
Compile Wrapper
    ↓
Executable Output
```

---

# 🏗 Architecture

## Windows

```text
Script
   ↓
TinyCC (libtcc)
   ↓
EXE Launcher
```

---

## Linux

```text
Script
   ↓
Generated C Wrapper
   ↓
GCC
   ↓
Linux Executable
```

---

# 📂 Supported Extensions

| Extension | Platform | Output |
|---|---|---|
| `.bat` | Windows | `.exe` |
| `.cmd` | Windows | `.exe` |
| `.ps1` | Windows | `.exe` |
| `.sh` | Linux | executable |

---

# 🔥 Example

## Input

```bat
echo Hello World
pause
```

## Output

```text
hello.exe
```

Double click it like a normal application.

---

# ⚙️ Building

## 🐧 Linux

```bash
gcc compile.c -o compile
```

---

## 🪟 Windows

```bash
gcc compile.c -ltcc -o compile.exe
```

---

# 📦 TinyCC Setup

Clone TinyCC:

```bash
git clone https://github.com/TinyCC/tinycc.git
```

Build:

```bash
cd tinycc
./configure
make
sudo make install
```

---

# 🛡 Important Note

This project wraps scripts into executable launchers.

It does NOT directly compile shell syntax into machine code.

Interpreters are still required:

| Script | Interpreter |
|---|---|
| `.bat` | CMD |
| `.ps1` | PowerShell |
| `.sh` | Bash |

---

# 🌌 Future Plans

- GUI Version
- Script Encryption
- Embedded Resources
- Self Extracting Binaries
- Multi Script Bundles
- Icon Injection
- Background Execution
- Process Hiding
- Native PE/ELF Packing

---

# 👨‍💻 Author

## Dr Sanne Karibo

Cross Platform Systems Developer  
C / Linux / Windows / Audio Systems

---

<div align="center">

# ⭐ Star The Project

Built with pure C and low-level systems engineering.

</div>
