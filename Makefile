home := ~

.PHONY: all
all: sw cw getmonitor

sw: sw.c cw getmonitor.o
	$(CC) sw.c -o sw

cw: cw.c getmonitor.o
	$(CC) cw.c getmonitor.o -o cw

getmonitor.o: getmonitor.c
	$(CC) -c getmonitor.c -o getmonitor.o

getmonitor: getmon.c getmonitor.o
	$(CC) getmon.c getmonitor.o -o getmonitor

