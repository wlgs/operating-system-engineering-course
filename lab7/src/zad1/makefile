CC = gcc -Wall


all: main.c
	touch oven
	touch table
	$(CC) main.c -o main
	$(CC) courier.c -o courier
	$(CC) cook.c -o cook

clean:
	rm -f main
	rm -f cook
	rm -f oven
	rm -f table
	rm -f courier
