CC=clang++-14
CFLAGS=-std=c++17 -Wall -Wextra -Wpedantic
LDLIBS=

CMP_SOURCES=cmp.cpp
CMP_OBJECTS=$(CMP_SOURCES:.cpp=.o)

COPY_SOURCES=copy.cpp
COPY_OBJECTS=$(COPY_SOURCES:.cpp=.o)

.PHONY: all clean

all: $(CMP_EXECUTABLE) $(COPY_EXECUTABLE)

cmp: $(CMP_OBJECTS)
	$(CC) $(CFLAGS) $(LDLIBS) $(CMP_OBJECTS) -o $@

copy: $(COPY_OBJECTS)
	$(CC) $(CFLAGS) $(LDLIBS) $(COPY_OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(CMP_OBJECTS) cmp $(COPY_OBJECTS) copy text3.txt
