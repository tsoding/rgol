#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rgol.h"

int main(int argc, char *argv[])
{
    Board board = {};

    const Coord current = coord(1, 1);
    for (int i = 0; i < ARRAY_LEN(nbor_masks); ++i) {
        board_fill(&board, DED);
        board_apply_nbor_mask(&board, nbor_masks[i], current);
        Nbor_Mask scrapped_mask = board_scrap_nbor_mask(&board, current);
        if (scrapped_mask != nbor_masks[i]) {
            printf("Failed: expected %d but got %d\n", nbor_masks[i], scrapped_mask);
            abort();
        }
    }

    printf("OK\n");

    // const Coord current = coord(2, 2);
    // for (int i = 0; i < NBORS; ++i) {
    //     for (int j = i + 1; j < NBORS; ++j) {
    //         for (int k = j + 1; k < NBORS; ++k) {
    //             uint8_t x = 0;
    //             for (int q = 0; q < 8; ++q) {
    //                 if (q == i || q == j || q == k) {
    //                     x |= 1 << q;
    //                 }
    //             }
    //             printf("%d\n", x);
    //         }
    //     }
    // }


    return 0;
}
