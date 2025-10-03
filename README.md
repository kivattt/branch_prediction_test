`objdump -d a.out > file` to disassemble, search for `do_thing` routine

`g++ -O1 main.cpp` for the cmove inline assembly\
`g++ -O2 main.cpp` for normal branching (comment out inline asm and uncomment the loop)\
`g++ -O3 main.cpp` for branchless, auto-vectorized (comment out inline asm and uncomment the loop)

```
[random data / predictable data]

[357ms / 50ms] normal branch, hardcoded add 69
[54ms / 55ms] conditional mov (lea, cmp, cmove plus 2 unnecessary instructions..)
[26ms / 26ms] branchless, vectorized
```
