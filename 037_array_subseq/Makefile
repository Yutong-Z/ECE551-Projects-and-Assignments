myProgram: test-subseq.o maxSeq.o
	gcc -o myProgram test-subseq.o maxSeq.o
test-subseq.o: test-subseq.c
	gcc -c -Wall -Werror -pedantic -std=gnu99 test-subseq.c
maxSeq.o: maxSeq.c
	gcc -c -Wall -Werror -pedantic -std=gnu99 maxSeq.c
