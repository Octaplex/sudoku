#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define ERR_BADFILE_FMT "error: file '%s' is not a valid Sudoku configuration\n"
#define ERR_BADDIM_FMT "error: a Sudoku board can't be %zux%zu\n"

#define USAGE "usage: sudoku FILE\n"

#define SPACE '.'

typedef unsigned int value_t;

typedef struct {
    value_t value;
    bool fixed;
} cell_t;

typedef struct {
    size_t width;
    size_t height;

    size_t bwidth;
    size_t bheight;

    cell_t **cells;
} grid_t;


/**
 * Get the sum of a particular row in a grid.
 */
value_t row_sum(grid_t *grid, size_t row) {
    value_t result = 0;
    size_t col;
    for (col = 0; col < grid->width; col++) {
        result += grid->cells[row][col].value;
    }

    return result;
}

/**
 * Get the sum of a particular column in a grid.
 */
value_t col_sum(grid_t *grid, size_t col) {
    value_t result = 0;
    size_t row;
    for (row = 0; row < grid->height; row++) {
        result += grid->cells[row][col].value;
    }

    return result;
}

/**
 * Get the sum of a particular block in a grid.
 */
value_t block_sum(grid_t *grid, size_t block) {
    size_t vrow = grid->bwidth * (block % grid->bwidth);
    size_t vcol = grid->bheight * (block % grid->bheight);

    value_t result = 0;
    size_t row, col;
    for (size_t row = 0; row < grid->bheight; row++) {
        for (size_t col = 0; col < grid->bwidth; col++) {
            result += grid->cells[vrow + row][vcol + col].value;
        }
    }

    return result;
}

/**
 * Determine if the given grid is in a goal state.
 */
bool is_goal(grid_t *grid) {
    size_t row, col, block;
    cell_t cell;

    value_t goal = row_sum(grid, 0);
    for (row = 1; row < grid->height; row++) {
        if (row_sum(grid, row) != goal) {
            return false;
        }
    }

    for (col = 0; col < grid->width; col++) {
        if (col_sum(grid, col) != goal) {
            return false;
        }
    }

    for (block = 0; block < grid->bwidth * grid->bheight; block++) {
        if (block_sum(grid, block) != goal) {
            return false;
        }
    }

    return true;
}

/**
 * Determine whether or not a dimension is square.
 */
bool is_perfect(size_t dim) {
    double s = sqrt((double)dim);
    return (int)s == s;
}

grid_t *read_grid(char *fn) {
    FILE *f;
    if ((f = fopen(fn, "r")) == NULL) {
        perror("sudoku");
        exit(1);
    }

    size_t width, height;
    if (fscanf(f, "%zu,%zu\n", &width, &height) < 2) {
        fprintf(stderr, ERR_BADFILE_FMT, fn);
        exit(1);
    }

    if (!is_perfect(width) || !is_perfect(height)) {
        fprintf(stderr, ERR_BADDIM_FMT, width, height);
        exit(1);
    }

    return NULL;

    //grid_t *grid = malloc(sizeof(grid_t));
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, USAGE);
        return 1;
    }

    grid_t *grid = read_grid(argv[1]);
}
