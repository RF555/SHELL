CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99
LDFLAGS = -shared
LIBS = -lm

TARGETS = encode decode
LIBS_TARGETS = libcodecA.so libcodecB.so
HEADERS = codecA.h codecB.h

all: $(TARGETS)

libcodecA.so: codecA.c $(HEADERS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

libcodecB.so: codecB.c $(HEADERS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)

encode: encode.c $(HEADERS) $(LIBS_TARGETS)
	$(CC) $(CFLAGS) -o $@ $< -L. -Wl,-rpath='$$ORIGIN' -lcodecA -lcodecB

decode: decode.c $(HEADERS) $(LIBS_TARGETS)
	$(CC) $(CFLAGS) -o $@ $< -L. -Wl,-rpath='$$ORIGIN' -lcodecA -lcodecB

clean:
	rm -f $(TARGETS) $(LIBS_TARGETS)

.PHONY: all clean
