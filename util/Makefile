# utility makefile
# $Amigan: rsmeter/util/Makefile,v 1.1 2004/10/27 23:56:19 dcp1990 Exp $
LDFLAGS=-lncurses
CFLAGS=-Wall -g -ansi
CC=cc
all: rsmeter_non checkrts
rsmeter_non: rsmeter_non.c
	${CC} ${CFLAGS} -o rsmeter_non rsmeter_non.c
checkrts: checkrts.c
	${CC} ${CFLAGS} -o checkrts checkrts.c
clean:
	rm -f *.o rsmeter_non checkrts *.core
