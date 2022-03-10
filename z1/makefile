static:
	make clear
	gcc -Wall -c libz1.c #compile only to .o file
	ar crs libz1.a libz1.o
	gcc -Wall main.c -o main -L. -lz1
	# cleaning up the .o files...
	rm -f *.o

clear:
	# cleaning up files...
	rm -f *.o *.a *.so main