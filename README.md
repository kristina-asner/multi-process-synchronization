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
   $$\text{semarg.array}[i] = \text{num\_of\_procs} - 1 - i$$
2. **The Lock (Down Operation):** Before printing, process $k$ blocks itself by attempting to decrease its specific semaphore by $-(\text{num\_of\_procs} - 1)$ via `semop()`. It can only pass this block when its semaphore value reaches exactly $\text{num\_of\_procs} - 1$.
3. **The Release (Up Operation):** After printing its current number, process $k$ loops through all other processes' semaphores ($j \neq k$) and increments each of them by $+1$. This effectively shifts the system state, unblocking the next process in queue.

---

## 💻 How to Run the Project

### Prerequisites
* Linux-based operating system (or WSL)
* GCC Compiler

### Compilation
Compile the C file using standard `gcc`:
```bash
gcc -o process_sync main.c