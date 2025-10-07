`objdump -d a.out > file` to disassemble, search for `do_thing` routine

`g++ -O1 main.cpp` for the cmove inline assembly\
`g++ -O2 main.cpp` for normal branching (comment out inline asm and uncomment the loop)\
`g++ -march=native -O3 main.cpp` for branchless, auto-vectorized (comment out inline asm and uncomment the loop)

```
AMD Ryzen 7 3700X (16) @ 4.100GHz
gcc version 13.3.0 (Ubuntu 13.3.0-6ubuntu2~24.04)
[random data / predictable data]

[357ms / 50ms] normal branch, hardcoded add 69
[54ms / 55ms] conditional mov (lea, cmp, cmove plus 2 unnecessary instructions..)
[26ms / 26ms] branchless, auto-vectorized
[17ms / 17ms] cheesy auto-vectorized sum (AVX2 256-bit simd)
```

```
11th Gen Intel i5-1135G7 (8) @ 4.200GHz
g++ (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0

[378ms / 44ms] normal branch, hardcoded add 69
[100ms / 100ms] conditional mov (lea, cmp, cmove plus 2 unnecessary instructions..)
[26ms / 27ms] branchless, auto-vectorized
[24 / 26ms] cheesy auto-vectorized sum (AVX2 256-bit simd)
```
