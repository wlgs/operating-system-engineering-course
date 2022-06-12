CC= gcc -Wall


all:
	make sys
	make lib

sys: mainsys.c
	# Compiling main.c ...
	$(CC) mainsys.c -o mainsys

lib: mainlib.c
	# Compiling main.c ...
	$(CC) mainlib.c -o mainlib

clean:
	# Cleaning executable ...
	rm -f mainlib
	rm -f mainsys