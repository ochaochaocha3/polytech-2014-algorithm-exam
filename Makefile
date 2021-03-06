CC=gcc
CFLAGS=-Wall -std=c99 -O2 # -g -O0
TARGET=madmin

.PHONY: all

all: $(TARGET)

$(TARGET): municipality.o list.o route_search.o visit_table.o graph.o madmin.c
	$(CC) $(CFLAGS) -o $(TARGET) $^

visit_table_test: visit_table.o visit_table_test.c
	$(CC) $(CFLAGS) -o visit_table_test $^

municipality.o: municipality.c municipality.h debug.h
	$(CC) $(CFLAGS) -c $<

list.o: list.c list.h debug.h
	$(CC) $(CFLAGS) -c $<

route_search.o: route_search.c route_search.h debug.h visit_table.o
	$(CC) $(CFLAGS) -c $<

visit_table.o: visit_table.c visit_table.h debug.h
	$(CC) $(CFLAGS) -c $<

graph.o: graph.c graph.h debug.h
	$(CC) $(CFLAGS) -c $<

.PHONY: clean

clean:
	$(RM) $(TARGET) $(TARGET).exe visit_table_test visit_table_test.exe *.o
