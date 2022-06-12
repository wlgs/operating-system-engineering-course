CC= gcc -Wall


all: mainA.c mainB.c
	# Compiling mainA.c and mainB.c ...
	$(CC) mainA.c -o mainA
	$(CC) mainB.c -o mainB

A: mainA.c
	# Compiling mainA.c
	$(CC) mainA.c -o mainA

B: mainB.c
	# Compiling mainA.c
	$(CC) mainB.c -o mainB

clean:
	# Cleaning executable ...
	rm -f mainA
	rm -f mainB
	rm -f main