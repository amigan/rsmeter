# rsmeter makefile
LDFLAGS=-lncurses
CFLAGS=-Wall -g -ansi
CC=cc
all: rsmeter rsmeter_non checkrts
rsmeter: main.o drawing.o
	${CC} ${LDFLAGS} ${CFLAGS} -o rsmeter drawing.o main.o
drawing.o: rsmeter.h
	${CC} ${CFLAGS}  -DSLOWWAY -c -o drawing.o drawing.c
main.o: rsmeter.h
	${CC} ${CFLAGS} -DUSEOLDWAY -c -o main.o main.c
rsmeter_non: rsmeter_non.c
	${CC} ${CFLAGS} -o rsmeter_non rsmeter_non.c
checkrts: checkrts.c
	${CC} ${CFLAGS} -o checkrts checkrts.c
clean:
	rm -f *.o rsmeter rsmeter_non *.core checkrts
