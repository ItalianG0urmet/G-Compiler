# G Compiler ![](https://img.shields.io/badge/Platform-Linux-blue) ![InDev](https://img.shields.io/badge/Status-Development%20Phase-red) ![LLVM-15](https://img.shields.io/badge/LLVM-15%2B-orange)

## **Description**

This is a simple compiler for a custom-made language that I am creating. It will be a C-like language with some modern features.
You may ask why this project? It's simple: I noticed how today's technology has spoiled us in the programming world, so my goal is to build this project using plain C, without any IDE. I'm using Vim without autocomplete or error checking — just me and my code.

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

int main() {
    // Tuple unpacking with type inference
    let (a, b) = getTuple();
    if (!b) {
        return -1;
    }
    print("A: {}, B: {}\n", a, b);

    int array[LIMIT] = {1, 2, 3, 4, 5};
    // Accessing out-of-bounds elements will cause a runtime error
    // int x = array[10];

    "Hello, how are you\n"; // Implicitly prints the string

    return 0;
}

```

## **Development Roadmap**

- [x] Basic tokenizer
- [x] AST for variables/functions
- [ ] Control flow (if/while)
- [ ] Complex types (structs)
- [ ] Optimization manager
- [ ] Memory management
- [ ] Annotation
- [ ] Memory safety
