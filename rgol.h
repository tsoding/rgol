#ifndef RGOL_H_
#define RGOL_H_

#include <stdint.h>
#include <string.h>

#define WIDTH 5
#define HEIGHT 5

#define ARRAY_LEN(xs) (sizeof(xs) / sizeof(xs[0]))

typedef enum {
    DED = 0,
    ALIVE = 1,
} Cell;

typedef struct {
    Cell cells[HEIGHT][WIDTH];
} Board;

typedef struct {
    int y, x;
} Coord;

Coord coord(int y, int x)
{
    return (Coord) {y, x};
}

Coord coord_add(Coord a, Coord b)
{
    return (Coord) {a.y + b.y, a.x + b.x};
}

void println_coord(Coord coord)
{
    printf("(%d, %d)\n", coord.y, coord.x);
}

int modi(int x, int y)
{
    return (x % y + y) % y;
}

static Coord nbor_dirs[8] = {
    {-1, -1},
    {-1, 0},
    {-1, 1},
    {0, -1},
    {0, 1},
    {1, -1},
    {1, 0},
    {1, 1}
};

#define NBORS (ARRAY_LEN(nbor_dirs))

Cell board_get_cell(const Board *board, Coord coord)
{
    return board->cells[modi(coord.y, HEIGHT)][modi(coord.x, WIDTH)];
}

void board_update_cell(Board *board, Coord coord, Cell cell)
{
    board->cells[modi(coord.y, HEIGHT)][modi(coord.x, WIDTH)] = cell;
}

void board_fill(Board *board, Cell cell)
{
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            board_update_cell(board, coord(y, x), cell);
        }
    }
}

int board_equals(const Board *board1, const Board *board2)
{
    return memcmp(board1->cells, board2->cells,
                  sizeof(Cell) * HEIGHT * WIDTH) == 0;
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

int board_nbors(const Board *board, Coord pivot)
{
    int result = 0;
    for (int index = 0; index < NBORS; ++index) {
        Coord nbor = coord_add(pivot,
                                   nbor_dirs[index]);
        if (board_get_cell(board, nbor) == ALIVE) {
            result += 1;
        }
    }

    return result;
}

void board_next_gen(const Board *prev, Board *next)
{
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            Coord current = coord(y, x);
            int nbors = board_nbors(prev, current);
            if (board_get_cell(prev, current) == ALIVE) {
                board_update_cell(
                    next, current,
                    nbors == 2 || nbors == 3 ? ALIVE : DED);
            } else {                // DED
                board_update_cell(
                    next, current,
                    nbors == 3 ? ALIVE : DED);
            }
        }
    }
}

typedef uint8_t Nbor_Mask;

const uint8_t nbor3_masks[] = {
    7, 11, 19, 35, 67, 131, 13, 21, 37, 69, 133, 25, 41,
    73, 137, 49, 81, 145, 97, 161, 193, 14, 22, 38, 70,
    134, 26, 42, 74, 138, 50, 82, 146, 98, 162, 194, 28,
    44, 76, 140, 52, 84, 148, 100, 164, 196, 56, 88, 152,
    104, 168, 200, 112, 176, 208, 224
};

void board_apply_nbor_mask(Board *board,
                           Nbor_Mask mask,
                           Coord coord)
{
    for (int i = 0; i < NBORS; ++i) {
        board_update_cell(
            board,
            coord_add(coord, nbor_dirs[i]),
            (mask & (1 << i)) ? ALIVE : DED);
    }
}

Nbor_Mask board_scrap_nbor_mask(const Board *board, Coord coord)
{
    Nbor_Mask mask = 0;
    for (int i = 0; i < NBORS; ++i) {
        if (board_get_cell(board, coord_add(coord, nbor_dirs[i])) == ALIVE) {
            mask |= 1 << i;
        }
    }
    return mask;
}


#endif  // RGOL_H_
