CC=gcc
CFLAGS=-Wall -Werror -g
BINS=pl path

all : $(BINS)

pl : pl.o Places.o
pl.o : pl.c Places.h

path : path.o Map.o Places.o Queue.o
path.o : path.c Map.c Places.h 

Places.o : Places.c Places.h
Map.o : Map.c Map.h Places.h
Queue.o : Queue.c Queue.h Item.h
PQueue.o : PQueue.c PQueue.h Item.h
Stack.o : Stack.c Stack.h Item.h

clean :
	rm -f $(BINS) *.o core *.dSYM
