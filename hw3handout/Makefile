CC := gcc
CFLAGS := -Wall
.PHONY:all

all: test

test.o: test.c float_i2f.h
	$(CC) $(CFLAGS) -c $<

float_i2f.o: float_i2f.c float_i2f.h
	$(CC) $(CFLAGS) -c $<

test: test.o float_i2f.o
	$(CC) $(CFLAGS) -o test test.o float_i2f.o

.PHONY:clean
clean:
	rm test
	rm float_i2f.o	
