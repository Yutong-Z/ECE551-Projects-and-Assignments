myProgram: test-power.o power.o
	gcc -o myProgram test-power.o power.o
test-power.o: test-power.c
	gcc -c -Wall -Werror -pedantic -std=gnu99 test-power.c
power.o: power.c
	gcc -c -Wall -Werror -pedantic -std=gnu99 power.c
