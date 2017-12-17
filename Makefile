OBJS=main.o util.o preprocess.o parse.o 
CC=gcc
CFLAGS= -std=c99 -Wall -O -g
all: hpXmlParser fileSkipViewer testparse
.PHONY: clean
hpXmlParser: ${OBJS}
	${CC} ${OBJS} -o hpXmlParser
fileSkipViewer: fileSkipViewer.o util.o
	${CC} fileSkipViewer.o util.o -o fileSkipViewer
testparse: testparse.o parse.o
	${CC} testparse.o parse.o -o testparse
fileSkipViewer.o: fileSkipViewer.c
	${CC} ${CFLAGS} -c fileSkipViewer.c
testparse.o: testparse.c
	${CC} ${CFLAGS} -c testparse.c
main.o: main.c 
	${CC} ${CFLAGS} -c main.c
util.o: util.c
	${CC} ${CFLAGS} -c util.c
preprocess.o: preprocess.c preprocess.h
	${CC} ${CFLAGS} -c preprocess.c
parse.o: parse.c parse.h
	${CC} ${CFLAGS} -c parse.c

clean: 
	rm -f *.o
	rm -f hpXmlParser
	rm -f fileSkipViewer
	rm -f testparse