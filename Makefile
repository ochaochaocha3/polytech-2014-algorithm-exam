CC=gcc
CFLAGS=-Wall -std=c99
TARGET=madmin

.PHONY: all

all: $(TARGET)

$(TARGET): municipality.o list.o madmin.c
	$(CC) $(CFLAGS) -o $(TARGET) $^

municipality.o: municipality.c municipality.h debug.h
	$(CC) $(CFLAGS) -c $<

list.o: list.c list.h debug.h
	$(CC) $(CFLAGS) -c $<

.PHONY: clean

clean:
	$(RM) madmin{,.exe} *.o
