# rsmeter makefile
# $Amigan: rsmeter/src/Makefile,v 1.3 2004/10/27 23:57:41 dcp1990 Exp $
LDFLAGS=-lncurses
CFLAGS=-Wall -g -ansi
CC=cc
all: rsmeter
rsmeter: main.o drawing.o
	${CC} ${LDFLAGS} ${CFLAGS} -o rsmeter drawing.o main.o
drawing.o: rsmeter.h
	${CC} ${CFLAGS}  -DSLOWWAY -c -o drawing.o drawing.c
main.o: rsmeter.h
	${CC} ${CFLAGS} -DUSEOLDWAY -c -o main.o main.c
clean:
	rm -f *.o rsmeter  *.core 
