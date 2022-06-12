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
	$(CC) main.c -o main -L. -lz1 -Wl,-rpath="./"
	rm -f *.o

dynamic:
	make shared
	$(CC) -fPIC -L. -o main main.c -ldl -D DYNAMIC
	rm -f *.o

clean:
	rm -f *.o *.a *.so *.exe main

runtest:
	echo "./main create_table 1000000 wc_files ../test_files/small.txt ../test_files/medium.txt ../test_files/large.txt remove_block 0 wc_files ../test_files/large.txt ../test_files/large.txt ../test_files/large.txt ../test_files/large.txt" >> raport_generated.txt
	./main create_table 1000000 wc_files ../test_files/small.txt ../test_files/medium.txt ../test_files/large.txt remove_block 0 wc_files ../test_files/large.txt ../test_files/large.txt ../test_files/large.txt ../test_files/large.txt
	echo "./main create_table 99999999 wc_files ../test_files/large.txt wc_files ../test_files/large.txt wc_files ../test_files/large.txt remove_block 1 remove_block 0 wc_files ../test_files/large.txt" >> raport_generated.txt
	./main create_table 99999999 wc_files ../test_files/large.txt wc_files ../test_files/large.txt wc_files ../test_files/large.txt remove_block 1 remove_block 0 wc_files ../test_files/large.txt
	echo "./main create_table 10 wc_files ../test_files/large.txt remove_block 0 wc_files ../test_files/large.txt remove_block 0 wc_files ../test_files/large.txt remove_block 0" >> raport_generated.txt
	./main create_table 10 wc_files ../test_files/large.txt remove_block 0 wc_files ../test_files/large.txt remove_block 0 wc_files ../test_files/large.txt remove_block 0
	#
	#
	# RESULT GENERATED IN raport_generated.txt

runtests:
	rm -f raport_generated.txt
	make clean
	make static
	echo "======= S T A T I C =======" >> raport_generated.txt
	make runtest

	make clean
	make shared
	echo "======= S H A R E D =======" >> raport_generated.txt
	make runtest

	make clean
	make dynamic
	echo "======= D Y N A M I C =======" >> raport_generated.txt
	make runtest

	make clean
	#
	#
	#TEST ENDED SEE RESULT IN raport_generated.txt