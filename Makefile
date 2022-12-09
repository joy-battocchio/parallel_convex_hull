CC = mpicc
CC2 = gcc
CFLAGS = -std=c99 -g -fopenmp -Wall -fstack-protector -lm  -I.
DEPS = convex_hull.c
OBJ = parallel.o
OBJ2 = serial.o

parallel_test.o: parallel.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

serial.o: serial.c $(DEPS)
	$(CC2) -c -o $@ $< $(CFLAGS)

all: parallel

parallel: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

serial: $(OBJ2)
	$(CC2) -o $@ $^ $(CFLAGS)

run: run_parallel

run_parallel: parallel
	./parallel

run_serial: serial
	./serial

sub_parallel: 
	qsub parallel.sh

sub_benchmark: 
	qsub parallel.sh
	qsub parallel.sh
	qsub parallel.sh
	qsub parallel.sh
	qsub parallel.sh
	qsub parallel.sh
	qsub parallel.sh
	qsub parallel.sh
	qsub parallel.sh
	qsub parallel.sh

sub_serial: 
	qsub serial.sh

.PHONY: clean

clean:
	rm -f *.o *.o* *.e* *~ parallel serial
	rm -r output
	mkdir output
