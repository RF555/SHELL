CC=clang-14

CFLAGS=-Wall -Wextra -Wpedantic
LDFLAGS=

all: cmp copy

cmp: cmp.o
	$(CC) $(LDFLAGS) -o $@ $^

copy: copy.o
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f cmp copy text3.txt *.o
