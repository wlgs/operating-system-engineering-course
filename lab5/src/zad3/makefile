CC = gcc
CFLAGS = -Wall

all:
	$(CC) $(CFLAGS) producer.c -o producer
	$(CC) $(CFLAGS) consumer.c -o consumer
	$(CC) $(CFLAGS) main.c -o main

clean:
	rm -f consumer
	rm -f producer
	rm -f main
	rm -f *.o

test: clean all
	./main