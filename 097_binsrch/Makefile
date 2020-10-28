CFLAGS= -Wall -Werror -std=gnu++98 -pedantic -ggdb3


test: test-bsearch.o search.o
	g++ -o test test-bsearch.o search.o
test-bsearch.o: test-bsearch.cpp function.h
	g++ $(CFLAGS) -c test-bsearch.cpp
search.o: search.cpp function.h
	g++ $(CFLAGS) -c search.cpp
