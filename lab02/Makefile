# COMP1927 13s1 Week 02 Lab

CC=gcc
CFLAGS=-Wall -Werror

all: uil rl

uil: useIntList.o IntList.o
	$(CC) -o uil IntList.o useIntList.o

useIntList.o: useIntList.c IntList.h
	$(CC) -c $(CFLAGS) useIntList.c

IntList.o: IntList.c IntList.h
	$(CC) -c $(CFLAGS) IntList.c

rl: randList.c
	$(CC) -o rl randList.c

clean:
	rm -f uil IntList.o useIntList.o rl
