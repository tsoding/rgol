#include <stdio.h>
#include <stdlib.h>

#include "rgol.h"

int main(int argc, char *argv[])
{
    for (int i = 0; i < NBORS; ++i) {
        for (int j = i + 1; j < NBORS; ++j) {
            for (int k = j + 1; k < NBORS; ++k) {
                Nbor_Mask mask = 0;
                for (int q = 0; q < NBORS; ++q) {
                    if (q == i || q == j || q == k) {
                        mask |= 1 << q;
                    }
                }
                printf("%d\n", mask);
            }
        }
    }

    return 0;
}
