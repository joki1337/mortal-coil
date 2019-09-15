#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_BUFFER_LEN (1024*1024)

#define FREE    '.'
#define VISITED 'o'

bool go_up   (char *, int, char *);
bool go_down (char *, int, char *);
bool go_left (char *, int, char *);
bool go_right(char *, int, char *);

char *GLOBAL_DEBUG_LEVEL_SOLVED = 0;

bool is_solved(char *data)
{
    return memcmp(data, GLOBAL_DEBUG_LEVEL_SOLVED, strlen(data)) == 0;
}

bool go_up(char *data, int stride, char *pos)
{
    int steps = 0;

    if (*pos != FREE)
        return false;

    while (*pos == FREE) {
        *pos = VISITED;
        pos -= stride;
        ++steps;
    }

    // undo last step
    pos += stride;

    bool success = false;
    if (*(pos-1) == FREE || *(pos+1) == FREE)
        success = (
            go_left(data, stride, pos-1)
            || go_right(data, stride, pos+1)
        );
    else
        success = is_solved(data);

    if (!success)
        while (steps--) {
            *pos = FREE;
            pos += stride;
        }
    else
        while (steps--)
            printf("%c", 'u');

    return success;
}

bool go_down(char *data, int stride, char *pos)
{
    int steps = 0;

    if (*pos != FREE)
        return false;

    while (*pos == FREE) {
        *pos = VISITED;
        pos += stride;
        ++steps;
    }

    // undo last step
    pos -= stride;

    bool success = false;
    if (*(pos-1) == FREE || *(pos+1) == FREE)
        success = (
            go_left(data, stride, pos-1)
            || go_right(data, stride, pos+1)
        );
    else
        success = is_solved(data);

    if (!success)
        while (steps--) {
            *pos = FREE;
            pos -= stride;
        }
    else
        while (steps--)
            printf("%c", 'd');

    return success;
}

bool go_left(char *data, int stride, char *pos)
{
    int steps = 0;

    if (*pos != FREE)
        return false;

    while (*pos == FREE) {
        *(pos--) = VISITED;
        ++steps;
    }

    // undo last step
    ++pos;

    bool success = false;
    if (*(pos+stride) == FREE || *(pos-stride) == FREE)
        success = (
            go_up(data, stride, pos-stride)
            || go_down(data, stride, pos+stride)
        );
    else
        success = is_solved(data);

    if (!success)
        while (steps--) {
            *(pos++) = FREE;
        }
    else
        while (steps--)
            printf("%c", 'l');

    return success;
}

bool go_right(char *data, int stride, char *pos)
{
    int steps = 0;

    if (*pos != FREE)
        return false;

    while (*pos == FREE) {
        *(pos++) = VISITED;
        ++steps;
    }

    // undo last step
    --pos;

    bool success = false;
    if (*(pos+stride) == FREE || *(pos-stride) == FREE)
        success = (
            go_up(data, stride, pos-stride)
            || go_down(data, stride, pos+stride)
        );
    else
        success = is_solved(data);

    if (!success)
        while (steps--) {
            *(pos--) = FREE;
        }
    else
        while (steps--)
            printf("%c", 'r');

    return success;
}


int main(int argc, char const *argv[])
{
    static char level       [MAX_BUFFER_LEN];
    static char level_solved[MAX_BUFFER_LEN];
    static char workcopy    [MAX_BUFFER_LEN];
    static char inbuffer    [MAX_BUFFER_LEN];
    int w = 0, h = 0;

    {
        int c, idx = 0;
        while ((c = getchar()) != EOF) {
            if (isspace(c))
                continue;
            inbuffer[idx++] = c;
            assert(idx < MAX_BUFFER_LEN);
        }

        char *at = inbuffer;
        while (*at) {
            if (*at == '<') {
                if (strncmp(at, "<width>", 7) == 0) {
                    at += 7;
                    w = atoi(at);

                } else if (strncmp(at, "<height>", 8) == 0) {
                    at += 8;
                    h = atoi(at);

                } else if (strncmp(at, "<walls>", 7) == 0) {
                    assert(w && h);
                    at += 7;
                    w += 2;
                    h += 2;

                    memset(level, '#', w*h);
                    char (*target)[w] = (char (*)[w])level;
                    for (int row = 1; row < (h-1); ++row)
                        for (int col = 1; col < (w-1); ++col)
                            target[row][col] = *(at++);

                    assert(*at == '<');
                    break;

                } else
                    ;
            }
            ++at;
        }
    }

    strcpy(level_solved, level);

    {
        char *at = level_solved;
        while (*at) {
            if (*at == FREE)
                *at = VISITED;
            ++at;
        }
    }

    GLOBAL_DEBUG_LEVEL_SOLVED = level_solved;

    int sx = 1, sy = 1;
    bool success = false;
    while (!success) {

        printf("sx:%d / sy:%d\n", sx, sy);
        strcpy(workcopy, level);

        char *startpos = workcopy+(sy*w+sx);
        *startpos = VISITED;
        success = (
            go_right  (workcopy, w, startpos+1)
            || go_left(workcopy, w, startpos-1)
            || go_up  (workcopy, w, startpos-w)
            || go_down(workcopy, w, startpos+w)
        );

        if (success)
            break;

        if (sx < (w-2)) {
            ++sx;
        } else {
            sx = 1;
            ++sy;
            if (sy >= (h-1)) {
                printf("No solution found\n");
                return 1;
            }
        }
    }

    return 0;
}
