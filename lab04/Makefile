# COMP1927 Week 05 Lab

CC=gcc
CFLAGS=-Wall -Werror -g
LDFLAGS=

.PHONY: clean

all: sorter testQ

sorter : sorter.c

testQ : testQ.o Queue.o

Queue.o : Queue.c Queue.h

clean:
	rm -f testQ sorter *.o
