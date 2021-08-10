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

# Turn on the address sanitizer
LDFLAGS = -fsanitize=address

# Flags needed for the check library
CHECK_LDFLAGS = $(LDFLAGS) `pkg-config --libs check`

PROG = infix2rpn
TESTS = check_stack

all: $(PROG)

valgrind: LDFLAGS=
valgrind: CFLAGS=-Wall
valgrind: $(PROG)

stack.o: stack.c stack.h

infix2rpn: infix2rpn.o stack.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *~ *.o $(PROG) $(TESTS)

tarball: infix2rpn_submit.tar.gz

infix2rpn_submit.tar.gz: infix2rpn.c stack.c stack.h Makefile
	tar -czf $@ $^

check_stack: check_stack.o stack.o
	$(CC) -o $@ $^ $(CHECK_LDFLAGS)

check: all $(TESTS)
	@echo "Testing the stack implementation..."
	./check_stack
	@echo "\nTesting the infix2rpn implementation.."
	./check_infix2rpn.sh
	@echo "\nRunning any additional tests provided..."
	./test_expressions.sh
