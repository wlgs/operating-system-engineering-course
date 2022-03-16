CC = gcc -Wall

static:
	make clean
	$(CC) -c libz1.c
	ar crs libz1.a libz1.o
	$(CC) main.c -o main -L. -lz1
	rm -f *.o *.a

shared:
	make clean
	$(CC) -fPIC -c libz1.c
	$(CC) -shared -fPIC -o libz1.so libz1.o
	$(CC) main.c -o main -L. -lz1 -Wl,-rpath=`pwd`
	rm -f *.o

dynamic:
	make shared
	$(CC) -fPIC -L. -o main main.c -ldl -D DYNAMIC
	rm -f *.o

clean:
	rm -f *.o *.a *.so *.exe main

runtests:
	./main create_table 1000000 wc_files small.txt medium.txt large.txt remove_block 0 wc_files large.txt large.txt large.txt large.txt
	./main create_table 99999999 wc_files large.txt wc_files large.txt wc_files large.txt remove_block 1 remove_block 0 wc_files large.txt
	./main create_table 10 wc_files large.txt remove_block 0 wc_files large.txt remove_block 0 wc_files large.txt remove_block 0
