SRC := test_endian.c 
OBJ := driver.o test_endian.o 

CC := gcc
CFLAGS := -Wall
driver: $(OBJ)
	$(CC) $(CFLAGS) -o driver $(OBJ)

test_endian.o: test_endian.c
	$(CC) $(CFLAGS) -o $@ -c $<

test_endian.o: test_endian.c test_endian.h

.PHONY: clean

clean:
	rm -f driver
	rm -f test_endian.o
