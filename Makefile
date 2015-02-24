CC=gcc
CFLAGS=-Wall -g -O0 -std=c99
TARGET=madmin

.PHONY: all

all: $(TARGET)

$(TARGET): municipality.o list.o route_search.o visit_table.o madmin.c
	$(CC) $(CFLAGS) -o $(TARGET) $^

visit_table_test: visit_table.o visit_table_test.c
	$(CC) $(CFLAGS) -o visit_table_test $^

municipality.o: municipality.c municipality.h debug.h
	$(CC) $(CFLAGS) -c $<

list.o: list.c list.h debug.h
	$(CC) $(CFLAGS) -c $<

route_search.o: route_search.c route_search.h debug.h
	$(CC) $(CFLAGS) -c $<

visit_table.o: visit_table.c visit_table.h debug.h
	$(CC) $(CFLAGS) -c $<

.PHONY: clean

clean:
	$(RM) $(TARGET){,.exe} *.o
