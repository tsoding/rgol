#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rgol.h"

typedef int Coord_Index;

Coord coord_by_index(Coord_Index index)
{
    return coord(index / WIDTH, index % WIDTH);
}

void pepega_backtrace(const Board *next, Board *prev, Coord_Index index)
{
    // board_display(prev, stdout);
    // fputs("------------------------------\n", stdout);

    if (index < WIDTH * HEIGHT) {
        Coord coord = coord_by_index(index);
        board_update_cell(prev, coord, DED);
        pepega_backtrace(next, prev, index + 1);
        board_update_cell(prev, coord, ALIVE);
        pepega_backtrace(next, prev, index + 1);
    } else {
        static Board next1 = {};
        board_fill(&next1, DED);
        board_next_gen(prev, &next1);
        if (board_equals(next, &next1)) {
            board_display(prev, stdout);
            fputs("   |\n", stdout);
            fputs("   v\n", stdout);
            board_display(&next1, stdout);
            fputs("------------------------------\n", stdout);
        }
    }
}

void backtrace(const Board *next, Board *prev, Coord_Index index)
{
    // board_display(prev, stdout);
    // fputs("------------------------------\n", stdout);

    if (index < WIDTH * HEIGHT) {
        Coord coord = coord_by_index(index);

        if (board_get_cell(next, coord) == ALIVE) {
            if (board_get_cell(prev, coord) != DED) return;
            int nbors = board_nbors(prev, coord);
            if (nbors > 3) return;
            Nbor_Mask mask = board_scrap_nbor_mask(prev, coord);

            for (int i = 0; i < ARRAY_LEN(nbor3_masks); ++i) {
                if ((nbor3_masks[i] & mask) == mask) {
                    board_apply_nbor_mask(prev, nbor3_masks[i], coord);
                    backtrace(next, prev, index + 1);
                    board_apply_nbor_mask(prev, mask, coord);
                }
            }
        } else {
            backtrace(next, prev, index + 1);
        }
    } else {
        static Board next1 = {};
        board_fill(&next1, DED);
        board_next_gen(prev, &next1);
        if (board_equals(next, &next1)) {
            board_display(prev, stdout);
            fputs("|\n", stdout);
            fputs("v\n", stdout);
            board_display(&next1, stdout);
            fputs("------------------------------\n", stdout);
        }
    }
}

int main(int argc, char *argv[])
{
    const Board next = {
        .cells = {
            {0, 0, 0, 0, 0},
            {0, 1, 0, 1, 0},
            {0, 0, 0, 0, 0},
            {1, 0, 0, 0, 1},
            {0, 1, 1, 1, 0},
        }
    };

    Board prev = {};

    pepega_backtrace(&next, &prev, 0);

    return 0;
}
