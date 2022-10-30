CC = gcc
CFLAGS = -std=c99 -g -Wall -fstack-protector -I.
DEPS = vector.h serial_dei.c
OBJ = testing.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

default: testing

testing: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o *~