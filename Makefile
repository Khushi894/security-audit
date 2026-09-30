CC ?= gcc
WARN = -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Wformat=2 -Wstrict-prototypes
HARDEN = -fstack-protector-strong -D_FORTIFY_SOURCE=2
CFLAGS = -std=c11 -O2 $(WARN) $(HARDEN)

all: original remediated

original:
	$(CC) $(CFLAGS) original/faculty_module.c -o original/faculty_module

remediated:
	$(CC) $(CFLAGS) remediated/faculty_module_fixed.c -o remediated/faculty_module_fixed

clean:
	rm -f original/faculty_module remediated/faculty_module_fixed
