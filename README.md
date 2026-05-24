
```markdown
# Multi-Process Synchronization using System V Semaphores

A low-level Linux systems programming project demonstrating concurrent process synchronization, resource management, and race condition prevention using **System V Semaphores** in C.

## 📌 Project Overview
This program creates a user-defined number of concurrent child processes (`num_of_procs`) that coordinate with each other to print an ordered sequence of numbers up to a specified maximum value (`max_num`). 

Without proper synchronization, executing multiple independent processes would result in non-deterministic output and race conditions. This project solves that by utilizing a semaphore set acting as a strict turn-taking mechanism.

## 🛠️ Technical Concepts Covered
* **Process Creation & Management:** Utilizing `fork()` to spawn independent execution paths and `wait()` to ensure clean parent process cleanup.
* **Concurrency & IPC:** Implementing Inter-Process Communication via System V semaphores (`semget`, `semctl`, `semop`).
* **Turn-Taking Synchronization Architecture:** Designing a mathematical approach to initialize and manipulate an array of semaphores, forcing strict sequential execution across distinct processes.

---

## 🔬 How the Synchronization Works

1. **Initialization:** The program initializes a semaphore set containing a semaphore for each child process. Each semaphore is pre-loaded with a value that controls when its corresponding process can run:
   ```c
   semarg.array[i] = num_of_procs - 1 - i;

```

2. **The Lock (Down Operation):** Before printing, process `k` blocks itself by attempting to decrease its specific semaphore by `-(num_of_procs - 1)` via `semop()`. It can only pass this block when its semaphore value reaches exactly `num_of_procs - 1`.
3. **The Release (Up Operation):** After printing its current number, process `k` loops through all other processes' semaphores and increments each of them by `+1`. This effectively shifts the system state, unblocking the next process in queue.

---

## 💻 How to Run the Project

### Prerequisites

* Linux-based operating system (or WSL)
* GCC Compiler

### Compilation

Compile the C file using standard `gcc`:

```bash
gcc -o process_sync semapore.c

```

### Execution

Run the compiled executable by providing two arguments:

1. `max_num` (The number to count up to)
2. `num_of_procs` (The number of parallel child processes to spawn)

```bash
# Example: Count up to 10 using 3 separate processes
./process_sync 10 3

```

### Expected Output

The processes will perfectly synchronize to output numbers sequentially:

```text
1
2
3
4
5
6
7
8
9
10

```

## 🧹 Resource Cleanup

To prevent memory and kernel IPC leaks, the parent process actively monitors children lifecycle using `wait()` blocks, and completely deallocates the semaphore system from the kernel space upon execution completion via `IPC_RMID`.
