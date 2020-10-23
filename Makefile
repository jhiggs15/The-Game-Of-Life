LIB=-lpthread -lrt
CC=gcc
CCPP=g++

all: life
life: life.C
	$(CCPP) life.C message.C board.C -o life $(LIB)
clean:
	/bin/rm -f *.o core life
