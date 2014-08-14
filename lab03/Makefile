# COMP1927 13s1 Week 03 Lab

CC=gcc
CFLAGS=-Wall -Werror -g
BINS=testL myed

all: $(BINS)

testL : testList.o List.o
	$(CC) -o testL testList.o List.o

myed : myed.o List.o
	$(CC) -o myed myed.o List.o

List.o : List.c List.h Item.h

myed.o : myed.c List.h Item.h

testList.o : testList.c List.h Item.h

clean:
	rm -f $(BINS) *.o core
