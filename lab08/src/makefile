CC = gcc
CFLAGS = -Wall -g

all: compile

compile:
	@$(CC) $(FLAGS) main.c -o main -lpthread

clean:
	@rm -f main

cleanall:
	@rm -f main times.txt

test: 
	./main 5 numbers fractal_tree.pgm ./negatives/fractal_tree_5.pgm
	./main 25 numbers fractal_tree.pgm ./negatives/fractal_tree_5.pgm
	./main 50 numbers fractal_tree.pgm ./negatives/fractal_tree_5.pgm
	./main 5 block fractal_tree.pgm ./negatives/fractal_tree_5.pgm
	./main 25 block fractal_tree.pgm ./negatives/fractal_tree_5.pgm
	./main 50 block fractal_tree.pgm ./negatives/fractal_tree_5.pgm