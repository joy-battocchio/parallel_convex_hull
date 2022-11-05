CC = mpicc
CFLAGS = -std=c99 -g -Wall -fstack-protector -lm -I.
DEPS = convex_hull.h
OBJ = parallel_test.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: parallel_test

parallel_test: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

run: parallel_test
	./parallel_test

.PHONY: clean

clean:
	rm -f *.o *.o* *.e* *~ parallel_test
