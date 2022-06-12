CC = gcc -Wall


all: catcher.c sender.c
	$(CC) catcher.c -o catcher
	$(CC) sender.c -o sender

clean:
	rm -f catcher
	rm -f sender
	rm -f *.o
	rm -f *.exe
	if pgrep catcher; then pkill --signal 9 catcher; fi
	if pgrep sender; then pkill --signal 9 sender; fi