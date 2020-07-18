CFLAGS = -c -Wall
CC = gcc
LIBS = -lm

all: main

main: main.o 
	${CC} main.o GraphAPI/graph_jrb.c libfdr/libfdr.a -o main

main.o: main.c
	${CC} ${CFLAGS} main.c

clean:
	rm -f *.o *~ main
