# C/C++ Security Audit — Defect Table

Audited source: `original/faculty_module.c`

| # | Exact location | Type | Finding | Security impact |
|---|---|---|---|---|
| 1 | `create_student()`, `malloc(sizeof(Student))` | Unchecked allocation | The `Student` allocation is dereferenced without checking for `NULL`. | Allocation failure can cause a NULL-pointer dereference and process crash (DoS). |
| 2 | `create_student()`, `strlen(name)` | NULL-pointer dereference risk | `name` is not validated before calling `strlen`. | A NULL caller argument causes immediate undefined behavior/crash. |
| 3 | `create_student()`, `if (strlen(name) > MAX_NAME)` | Bounds validation failure | An overlong name only produces a warning; execution continues. | The subsequent copy can overflow the allocated destination; input-length policy is not enforced. |
| 4 | `create_student()`, `malloc(strlen(name))` | Off-by-one allocation / buffer overflow | Allocation omits one byte required for the terminating `\0`. | `strcpy` writes one byte beyond the allocation, causing heap-buffer overflow. |
| 5 | `create_student()`, `strcpy(s->name, name)` | Unsafe string copy | `strcpy` has no destination-size check. | Out-of-bounds write for oversized input; can corrupt heap metadata or adjacent data. |
| 6 | `create_student()`, `s->marks = malloc(count * sizeof(int))` | Unchecked allocation | The marks allocation is not checked before later writes. | Allocation failure can cause NULL dereference; large/invalid sizes can create allocation failures. |
| 7 | `create_student()`, `count` parameter and `s->count = count` | Signed/unsigned conversion; invalid negative count | A signed `int` count is assigned to unsigned `size_t` without validation. | Negative input becomes a very large count, potentially causing huge allocation attempts and later out-of-bounds access. |
| 8 | `create_student()`, `count * sizeof(int)` | Allocation-size arithmetic risk | Size calculation is not explicitly range-checked. | On platforms where the multiplication exceeds `SIZE_MAX`, the size can wrap and allocate less memory than required. |
| 9 | `calculate_total()`, `for (int i = 0; i <= s->count; i++)` | Off-by-one / out-of-bounds read | Loop accesses index `count`, but valid indices end at `count - 1`. | Reads one element beyond the marks allocation; undefined behavior and possible information disclosure/crash. |
| 10 | `calculate_total()`, `total += s->marks[i]` | Signed integer overflow | `int total` is accumulated without range checking. | Signed overflow is undefined behavior and can produce incorrect/security-sensitive calculations. |
| 11 | `add_bonus()`, `s->marks[i] += bonus` | Signed integer overflow | Adding an arbitrary `bonus` to an `int` mark is unchecked. | Signed overflow is undefined behavior and can corrupt program logic. |
| 12 | `resize_marks()`, `realloc(...)` | Unchecked realloc / lost allocation | `realloc` result is assigned directly to `s->marks`. | On failure, the original pointer is lost (memory leak) and `s->marks` becomes NULL. |
| 13 | `resize_marks()`, `new_count * sizeof(int)` | Allocation-size arithmetic risk | Reallocation size is not range-checked and `new_count` is signed. | Invalid/large counts can result in incorrect allocation sizes or huge allocation requests. |
| 14 | `resize_marks()`, `s->count = new_count` | Signed/unsigned conversion; invalid negative count | Negative `new_count` converts to a huge `size_t`. | Subsequent loops may access memory far beyond the allocated object. |
| 15 | `resize_marks()` followed by `display_student()` | Uninitialized memory read | Growing the allocation to 10 elements does not initialize elements 5–9, but display reads them. | Undefined behavior; can expose stale heap contents and produce nondeterministic output. |
| 16 | `display_student()`, loop index `int` vs `size_t count` | Signed/unsigned mismatch | The loop uses `int` while `count` is `size_t`. | Can cause incorrect comparisons for large counts and compiler diagnostics; contributes to unsafe bounds logic. |
| 17 | `destroy_student()`, `free(s->name)` twice | Double-free | `s->name` is freed two times. | Undefined behavior; may corrupt the heap and potentially enable memory-safety exploitation. |
| 18 | `main()`, `student->marks[0]` after `destroy_student(student)` | Use-after-free | `student` and its `marks` have already been freed. | Undefined behavior; may crash or expose/reuse freed memory. |
| 19 | `main()`, `student` after `destroy_student` | Dangling pointer | Pointer is not cleared after destruction. | Increases risk of accidental reuse of freed memory; directly enables the UAF shown next. |

## Remediation principles

- Validate pointers and input ranges before use.
- Use `size_t` for object/array counts.
- Check allocation and reallocation results before replacing owned pointers.
- Use `SIZE_MAX`-based checked multiplication for allocation sizes.
- Allocate `strlen(name) + 1` bytes and enforce `MAX_NAME`.
- Use bounded copying (`memcpy` after a validated exact size in the remediation).
- Use `<` rather than `<=` for array bounds.
- Range-check integer addition before performing it.
- Initialize newly allocated/grown elements.
- Maintain one ownership path and free each allocation exactly once.
- Do not access an object after destruction; clear owner pointers where useful.
