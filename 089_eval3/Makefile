CFLAGS = -Wall -Werror -std=gnu++98 -pedantic -ggdb3

cyoa-step1: cyoa-step1.o cyoa.o
	g++ -o cyoa-step1 cyoa-step1.o cyoa.o
cyoa-step1.o: cyoa-step1.cpp
	g++ -c $(CFLAGS) cyoa-step1.cpp
cyoa.o: cyoa.cpp cyoa.h
	g++ -c $(CFLAGS) cyoa.cpp

clean:
	rm -f *.o  *~ matrixTest
