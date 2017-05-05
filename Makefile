sudoku: sudoku.c
	gcc -std=c99 -Wall -Wextra -pedantic $? -o $@ -lm
