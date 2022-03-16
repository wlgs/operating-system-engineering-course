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
	$(CC) main.c -o main -L. --lz1 -Wl,-rpath=`pwd`

clean:
	rm -f *.o *.a *.so *.exe main

run:
	make static
	./main.exe test1.txt test2.txt

benchmark:
	echo "WC SMALL FILE x5"
	time ./main.exe create_table 10 wc_files small.txt small.txt small.txt small.txt small.txt
	echo "WC MEDIUM FILE x5"
	time ./main.exe create_table 10 wc_files medium.txt medium.txt medium.txt medium.txt medium.txt
	echo "WC LARGE FILE x5"
	time ./main.exe create_table 10 wc_files large.txt large.txt large.txt large.txt large.txt