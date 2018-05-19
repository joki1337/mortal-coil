/*
I'm livin' on the edge - only mallocs no frees ;)
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
// #include <unistd.h>
// #include <signal.h>

#include "extract.cpp"
#include "level.cpp"

static Level level;

int main(void)
{
    level.print();

    for (int i = 0; i <= level.num_start_coords; ++i) {
        int row = level.start_coords[i].row;
        int col = level.start_coords[i].col;
        printf("Start coord - row %d / col %d\n", row, col);
        assert(level.data[row*level.w+col] == '.');

        level.data[row*level.w+col] = 'o';

        if (level.solve(row, col)) {
            printf("Yey! This one works ;) \n");
            exit(0); // Fertig!
        }

        level.data[row*level.w+col] = '.';
    }

    exit(1);
}
