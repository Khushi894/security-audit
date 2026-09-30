# C/C++ Memory & Integer Security Audit

This package contains the original faculty snippet, a remediated implementation, the defect table, compiler warning comparison, and sanitizer/Valgrind verification records.

## Build with hardening and warnings

```bash
make clean
make
```

Flags include `-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Wformat=2 -Wstrict-prototypes -fstack-protector-strong -D_FORTIFY_SOURCE=2`.

## ASan/UBSan

```bash
gcc -std=c11 -O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer remediated/faculty_module_fixed.c -o remediated_asan_ubsan
./remediated_asan_ubsan
```

## Valgrind

```bash
valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=99 ./remediated/faculty_module_fixed
```

Valgrind was unavailable in the current execution environment; see `reports/valgrind-report.txt`.

## Git history

The repository is intended to preserve the original baseline separately from the remediated source and audit evidence.
