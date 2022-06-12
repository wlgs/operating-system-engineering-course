CC = gcc -Wall


all: main.c
	touch oven
	touch table
	$(CC) main.c -o main -lpthread -lrt
	$(CC) courier.c -o courier -lpthread -lrt
	$(CC) cook.c -o cook -lpthread -lrt

clean:
	rm -f main
	rm -f cook
	rm -f oven
	rm -f table
	rm -f courier
