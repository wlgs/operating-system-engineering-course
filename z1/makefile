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

runtest:
	echo "./main create_table 1000000 wc_files small.txt medium.txt large.txt remove_block 0 wc_files large.txt large.txt large.txt large.txt" >> report.txt
	./main create_table 1000000 wc_files small.txt medium.txt large.txt remove_block 0 wc_files large.txt large.txt large.txt large.txt
	echo "./main create_table 99999999 wc_files large.txt wc_files large.txt wc_files large.txt remove_block 1 remove_block 0 wc_files large.txt" >> report.txt
	./main create_table 99999999 wc_files large.txt wc_files large.txt wc_files large.txt remove_block 1 remove_block 0 wc_files large.txt
	echo "./main create_table 10 wc_files large.txt remove_block 0 wc_files large.txt remove_block 0 wc_files large.txt remove_block 0" >> report.txt
	./main create_table 10 wc_files large.txt remove_block 0 wc_files large.txt remove_block 0 wc_files large.txt remove_block 0

runtests-all:
	make clean
	make static
	echo "======= S T A T I C =======" >> report.txt
	make runtest
	make clean
	make shared
	echo "======= S H A R E D =======" >> report.txt
	make runtest
	make clean
	make dynamic
	echo "======= D Y N A M I C =======" >> report.txt
	make runtest
	rm -f *.so
	#TEST ENDED SEE RESULT IN report.txt

