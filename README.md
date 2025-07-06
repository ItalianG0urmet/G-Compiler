# G Compiler ![](https://img.shields.io/badge/Platform-Linux-blue) ![InDev](https://img.shields.io/badge/Status-Development%20Phase-red) ![LLVM-15](https://img.shields.io/badge/LLVM-15%2B-orange)

## **Description**

This is a simple compiler for a custom-made language that I am creating. It will be a C-like language with some modern features.
You may ask why this project? It's simple: I noticed how today's technology has spoiled us in the programming world, so my goal is to build this project using plain C++, without any IDE. I'm using Vim without autocomplete or error checking — just me and my code.

## Update

After some reflection, I’ve decided to migrate this project from C to C++. My expectations and goals have evolved, and I believe C++ will offer better tools and abstractions to accelerate development and maintainability. 
From now on, I will gradually port existing code and new features to C++, while preserving the simplicity and performance focus of the original design.


## **Build & Run**

```bash
git clone https://github.com/ItalianG0urmet/gourmet-compiler.git
cd gourmet-compiler/
mkdir build
cd build
cmake ..
make
./compiler
```

## **Tech Stack**

| Component       | Purpose                          |
|-----------------|----------------------------------|
| LLVM 15+        | IR generation & optimization     |
| Clang           | Object linking                  |
| CMake 3.10+     | Build system                    |


## **Future Language Example**
```c
@import "std";

#define LIMIT 5

@unsafe
int getTwo() {
    return 2;
}

(int, bool) getTuple() {
    int* ptr = allocate(sizeof(int), 1) ?: {
        "Allocation failed\n";  // Implicit print on error
        return (0, false);
    };
    *ptr = 5;
    return (*ptr, true);
}

void main() {
    // Tuple unpacking with type inference
    let (a, b) = getTuple();
    if (!b) {
        exit(-1);
    }
    print("A: {}, B: {}\n", a, b);

    int array[LIMIT] = {1, 2, 3, 4, 5};
    // Accessing out-of-bounds elements will cause a runtime error
    // int x = array[10];

    "Hello, how are you\n"; // Implicitly prints the string

    return;
}

```

## **Development Roadmap**

- [x] Basic tokenizer
- [x] AST for variables/functions
- [ ] Migrate to C++
- [ ] Control flow (if/while)
- [ ] Complex types (structs)
- [ ] Memory management
