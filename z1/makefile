CC = gcc -Wall

static:
	make clean
	$(CC) -c libz1.c
	ar crs libz1.a libz1.o
	$(CC) main.c -o main -L. -lz1

shared:
	make clean
	$(CC) -fPIC -c libz1.c
	$(CC) -shared -fPIC -o libz1.so libz1.o
	$(CC) main.c -o main -L. --lz1 -Wl,-rpath=`pwd`

clean:
	rm -f *.o *.a *.so *.exe main

run:
	make static
	./main.exe test1.txt test2.txt