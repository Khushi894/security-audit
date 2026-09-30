# Compiler Warning Comparison

Compiler: GCC (version recorded in `tool-versions.txt`)
Flags:

`-std=c11 -O2 -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Wformat=2 -Wstrict-prototypes -fstack-protector-strong -D_FORTIFY_SOURCE=2`

## Before — original module

The original module produced **7 compiler warnings**:

1. `s->count = count` — signed-to-unsigned conversion (`-Wsign-conversion`)
2. `malloc(count * sizeof(int))` — signed-to-unsigned conversion (`-Wsign-conversion`)
3. `for (int i = 0; i <= s->count; i++)` — signed/unsigned comparison (`-Wsign-compare`)
4. `for (int i = 0; i < s->count; i++)` — signed/unsigned comparison (`-Wsign-compare`)
5. `realloc(s->marks, new_count * sizeof(int))` — signed-to-unsigned conversion (`-Wsign-conversion`)
6. `s->count = new_count` — signed-to-unsigned conversion (`-Wsign-conversion`)
7. `for (int i = 0; i < s->count; i++)` in `display_student` — signed/unsigned comparison (`-Wsign-compare`)

## After — remediated module

**0 compiler warnings** under the same warning and hardening flags.

The remediation uses `size_t` for counts/indices and explicit checked size arithmetic, removing the signed/unsigned mismatches instead of suppressing them.
