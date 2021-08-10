# Change this to change the compiler used
# CC = clang
CC = gcc

# Use C11, debugging and warnings flags
define CFLAGS
-std=c11 \
-g3 \
-Og \
-fsanitize=address \
-Wpedantic \
-Wall \
-Wextra \
-Wformat=2 \
-Wsign-conversion \
-Wswitch-enum \
-Wno-switch-default \
-Wunused \
-Wfloat-equal \
-Wconversion \
-Wpointer-arith \
-Wtype-limits \
-Wcast-qual \
-Wenum-compare \
-Wsizeof-pointer-memaccess \
-Wstrict-prototypes
endef

# Turn on the address sanitizer and link math library
LDFLAGS = -fsanitize=address -lm

# Flags needed for the check library
CHECK_LDFLAGS = $(LDFLAGS) `pkg-config --libs check`

PROG = mysort
TESTS = check_list malloc-test

all: $(PROG)

valgrind: LDFLAGS=
valgrind: CFLAGS=-Wall
valgrind: $(PROG)

list.o: list.c list.h

mysort: main.o list.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o $(PROG) $(TESTS)

tarball: insertion_sort_submit.tar.gz

insertion_sort_submit.tar.gz: main.c list.c list.h Makefile
	tar -czf $@ $^

check_list: check_list.o list.o
	$(CC) -o $@ $^ $(CHECK_LDFLAGS)

malloc-test: LDFLAGS=$(shell pkg-config --libs check) -ldl -fsanitize=address
malloc-test: CFLAGS=-std=c11 -g3 -Wall -fsanitize=address
malloc-test: malloc-test.o list.o
	$(CC) -o $@ $^ $(LDFLAGS)

check: all $(TESTS)
	@echo "Checking linked list implementation..."
	./check_list
	@echo "\nChecking insertion sort implementation..."
	./check_sort.sh
	@echo "\nTesting if errors on functions are checked"
	./malloc-test
