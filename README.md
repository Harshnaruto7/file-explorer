# File Explorer CLI

A simple and lightweight command-line file explorer built in C++. This project enables users to navigate directories, manage files, and perform common file operations directly from the terminal.

---

## Features

- Display and navigate through files and directories  
- Create, remove, copy, or move files and folders  
- Retrieve file information and permissions  
- Search files or directories recursively  
- Color-coded terminal output  
- Cross-platform support (Windows, Linux, macOS)  

---


## Prerequisites

Before running the File Explorer CLI, make sure your system satisfies the following requirements:

1. **Operating System**  
   - Linux, macOS, or Windows (via WSL2 or Linux VM)

2. **C++ Compiler (g++ with C++17 support)**  
   - Verify installation:  
     ```
     g++ --version
     ```
   - Recommended: g++ 7 or higher  

3. **Terminal or Command Line Interface**  
   - Must support basic shell commands (bash, zsh, or similar)

4. **File Permissions**  
   - Ensure read and write access to the target directories

5. **Optional: Git (for cloning the repository)**  
   - Check installation:  
     ```
     git --version
     ```

6. **Storage Requirements**  
   - Minimal (less than 10 MB for typical builds)

7. **Internet Connection (Optional)**  
   - Only required if cloning the repository or fetching updates

---

## 🛠️ Installation & Compilation

Follow these steps to set up and run the File Explorer CLI:

#### 1. Clone the Repository

Clone the project repository from GitHub:

```bash
git clone <your-repo-link>
```

#### 2. Ensure Prerequisites Are Installed

Make sure you have g++ installed and it supports C++17:
```bash
g++ --version
```

#### 3. Compile the Source Code

Use g++ with C++17 standard to compile `main.cpp`:

```bash
g++ main.cpp -std=c++17 -o explorer
```
#### 4. Run the code
```bash
./explorer
```


























