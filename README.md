# 🐣 Fork & Memory Copy Demo in C (Process Isolation)

A simple C program demonstrating how `fork()` creates a child process with a **copy** of the parent's memory space — not shared memory. This example highlights how changes in dynamically allocated memory by the child do **not** affect the parent.

---

## 📋 Features

- 🧠 Demonstrates process memory isolation after `fork()`.
- 📦 Uses `malloc()` to allocate a memory block shared virtually but **copied on write**.
- 👶 Child process modifies the memory block without affecting the parent.
- 👴 Parent waits for child termination (`waitpid`) before printing its own unchanged data.
- 🖨️ Clear console output showing distinct values in child and parent processes.

---

## 🔍 How it works

- Allocate 8 bytes via `malloc()` and store the string `"pai"`.
- Call `fork()` to create a child process.
- Child writes `"filho"` to the allocated memory and prints it.
- Parent waits for the child to finish and prints the original string `"pai"`.
- Demonstrates that parent and child have separate memory after `fork()` despite pointers having same virtual address.

---

## 🛠️ Requirements

- Unix-like OS (Linux, macOS, BSD)
- C compiler (gcc, clang)
- Basic terminal to compile and run

---

## ⚙️ Compilation & Execution

```bash
gcc -o fork_mem_copy fork_mem_copy.c
./fork_mem_copy
```

## ⚠️ Academic Disclaimer

This project was developed as an academic work and may contain errors or be incomplete. Use and adapt with caution.
