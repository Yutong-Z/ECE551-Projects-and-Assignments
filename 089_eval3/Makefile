# Cite Makefile: 060eval_2/Makefile
CCFLAGS=--std=gnu++98 -pedantic -Wall -Werror -ggdb3
PROGS=cyoa-step1 cyoa-step2 cyoa-step3 cyoa-step4
OBJS=$(patsubst %,%.o,$(PROGS)) cyoa.o
all: $(PROGS)
cyoa-step%: cyoa-step%.o cyoa.o
	g++ -o $@ $^
%.o: %.cpp
	g++ $(CFLAGS) -c $<
.PHONY: clean
clean:
	rm -f *~ $(PROGS) $(OBJS)
cyoa.o: cyoa.h
