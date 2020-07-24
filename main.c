#include <stdio.h>
#include <stdlib.h>

typedef enum {
    DED = 0,
    ALIVE = 1,
} Cell;

#define WIDTH 5
#define HEIGHT 5

typedef struct {
    Cell cells[HEIGHT][WIDTH];
} Board;

int modi(int x, int y)
{
    return (x % y + y) % y;
}

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
            } else {
                next->cells[y][x] = neighbors == 3 ? ALIVE : DED;
            }
        }
    }
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

    return 0;
}
