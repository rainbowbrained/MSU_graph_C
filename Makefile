CC=gcc --coverage -O2 -ftrapv -fsanitize=undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -std=gnu11 -lm

all: test1.o test2.o test3.o test4.o graphlib.o
	$(CC) test2.o graphlib.o -o test2
	$(CC) test1.o graphlib.o -o test1
	$(CC) test3.o graphlib.o -o test3
	$(CC) test4.o graphlib.o -o test4

test1.o: test1.c
	$(CC) -O0 -g -c test1.c

test2.o: test2.c 
	$(CC) -O0 -g -c test2.c 

test3.o: test3.c
	$(CC) -O0 -g -c test3.c

test4.o: test4.c
	$(CC) -O0 -g -c test4.c

graphlib.o: graphlib.c graphlib.h
	$(CC) -O0 -g -c graphlib.c

test1:
	./test1

test2:
	./test2

test4: 
	./test4

test3: 
	./test3

clean:
	rm -rf *.o *.gcda *.gcno test1 test2 test3 test4
