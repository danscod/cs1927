# COMP1927 14s2 Week 12 Lab

CC=gcc
CFLAGS=-Wall -Werror -pg

words: words.o HashTable.o List.o Item.o
	$(CC) $(CFLAGS) -o words words.o HashTable.o List.o Item.o

HashTable.o: HashTable.c HashTable.h
List.o: List.c List.h
Item.o: Item.c Item.h

clean:
	rm -fr words *.o core *.dSYM gmon.out
