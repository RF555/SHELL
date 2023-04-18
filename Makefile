CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99
LDFLAGS = -shared
LIBS = -lm

TARGETS_A = cmp copy
TARGETS_B = encode decode
LIBS_TARGETS_B = libcodecA.so libcodecB.so
HEADERS_B = codecA.h codecB.h

default: partA

partA: $(TARGETS_A)

partB: $(TARGETS_B)

cmp: cmp.o
	$(CC) -o $@ $^

copy: copy.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

libcodecA.so: codecA.c $(HEADERS_B)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

libcodecB.so: codecB.c $(HEADERS_B)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

encode: encode.c $(HEADERS_B) $(LIBS_TARGETS_B)
	$(CC) $(CFLAGS) -o $@ $< -L. -Wl,-rpath='$$ORIGIN' -lcodecA -lcodecB

decode: decode.c $(HEADERS) $(LIBS_TARGETS_B)
	$(CC) $(CFLAGS) -o $@ $< -L. -Wl,-rpath='$$ORIGIN' -lcodecA -lcodecB

clean:
	rm -f $(TARGETS_A) $(TARGETS_B) $(LIBS_TARGETS_B) text3.txt *.o

.PHONY: all clean
