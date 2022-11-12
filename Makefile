CC = mpicc
CC2 = gcc
CFLAGS = -std=c99 -g -Wall -fstack-protector -lm -I.
DEPS = convex_hull.h
OBJ = parallel_test.o
OBJ2 = serial_test.o

parallel_test.o: parallel_test.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

serial_test.o: serial_test.c $(DEPS)
	$(CC2) -c -o $@ $< $(CFLAGS)

all: parallel_test

parallel_test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

serial_test: $(OBJ2)
	$(CC2) -o $@ $^ $(CFLAGS)

run: run_parallel

run_parallel: parallel_test
	./parallel_test

run_serial: serial_test
	./serial_test

sub_parallel: 
	qsub parallel_test.sh

sub_serial: 
	qsub serial_test.sh

.PHONY: clean

clean:
	rm -f *.o *.o* *.e* *~ parallel_test serial_test
