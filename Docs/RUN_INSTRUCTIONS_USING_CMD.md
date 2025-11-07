# How to Run MapDriver.cpp 🚀

This guide explains how to compile and run your map testing script on macOS (zsh shell).

## Prerequisites

- C++17 or newer compiler (e.g., g++)
- All source files in the project directory

## Steps

### 1. Open Terminal

Navigate to your project directory:

```sh
cd /Users/achrafcheniti/Desktop/FALL\ 2025/COMP-345-PROJECT
```

### 2. Compile the Code

Run the following command to compile `MapDriver.cpp` and its dependencies:

```sh
g++ -std=c++17 -o MapDriver MapDriver.cpp Models/Map.cpp
```

Add other source files if needed (e.g., `MapLoader.cpp`).

### 3. Run the Program

Execute the compiled program:

```sh
./MapDriver
```

### 4. View Results

You will see emoji-enhanced output for each map file found and tested in the `Tests` directory!

---

## Troubleshooting

- If you get errors about missing headers, make sure you have the correct includes in your `.h` and `.cpp` files.
- If you get linker errors, ensure all required source files are included in the compile command.
- If you see no output, check that your `Tests` directory contains `.map` files.
