#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_LEN(xs) (sizeof(xs) / sizeof(xs[0]))

typedef enum {
    DED = 0,
    ALIVE = 1,
} Cell;

#define WIDTH 5
#define HEIGHT 5

typedef struct {
    Cell cells[HEIGHT][WIDTH];
} Board;

typedef struct {
    int y, x;
} Coord;

void println_coord(Coord coord)
{
    printf("(%d, %d)\n", coord.y, coord.x);
}

int modi(int x, int y)
{
    return (x % y + y) % y;
}

static Coord neighbor_index_dirs[8] = {
    {-1, -1},
    {-1, 0},
    {-1, 1},
    {0, -1},
    {0, 1},
    {1, -1},
    {1, 0},
    {1, 1}
};

void board_display(const Board *board, FILE *stream)
{
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            fputc(' ', stream);
            fputc(board->cells[y][x] == ALIVE ? '#' : '.', stream);
        }
        fputc('\n', stream);
    }
}

int board_neighbors(const Board *board, int y, int x)
{
    int result = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dy != 0 || dx != 0) {
                if (board->cells[modi(y + dy, HEIGHT)][modi(x + dx, WIDTH)] == ALIVE) {
                    result += 1;
                }
            }
        }
    }
    return result;
}

void board_next_gen(const Board *prev, Board *next)
{
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            int neighbors = board_neighbors(prev, y, x);
            if (prev->cells[y][x] == ALIVE) {
                next->cells[y][x] = neighbors == 2 || neighbors == 3 ? ALIVE : DED;
            } else {                // DED
                next->cells[y][x] = neighbors == 3 ? ALIVE : DED;
            }
        }
    }
}

void board_prev_cell(const Board *next, Board *prev, int y, int x)
{
    assert(next->cells[y][x] == ALIVE);
    prev->cells[y][x] = DED;
#define N ARRAY_LEN(neighbor_index_dirs)
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            for (int k = j + 1; k < N; ++k) {
                // TODO: it's probably looks like an algorithm that computes the binomial coeficient
                // https://cp-algorithms.com/combinatorics/binomial-coefficients.html
                // https://en.wikipedia.org/wiki/Binomial_coefficient
            }
        }
    }
#undef N
}

Board board[2] = {
    {
        .cells = {
            {1, 1, 1, 0, 0},
            {1, 0, 0, 0, 0},
            {0, 1, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
        }
    },
    {}
};

int main(int argc, char *argv[])
{
    generate_neighbor_index_dirs();
    for (int i = 0; i < ARRAY_LEN(neighbor_index_dirs); ++i) {
        println_coord(neighbor_index_dirs[i]);
    }

#if 0
    int fb = 0;
    board_display(&board[fb], stdout);
    fputs("------------------------------\n", stdout);

#define GLIDER_CYCLES 4
#define STEPS (GLIDER_CYCLES * 5)
    for (int i = 0; i < STEPS; ++i) {
        int bb = 1 - fb;
        board_next_gen(&board[fb], &board[bb]);
        fb = bb;
        board_display(&board[fb], stdout);
        fputs("------------------------------\n", stdout);
    }
#endif

    return 0;
}

// 0  1 2
// 0 -1 1
// index = 0 + 1
