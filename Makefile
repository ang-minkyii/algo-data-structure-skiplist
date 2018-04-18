
all:
	gcc -Wall -std=c99 -g -o eq main.c linkedlist.c tasks.c eq_util.c -lm