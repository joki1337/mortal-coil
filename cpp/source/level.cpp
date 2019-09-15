
enum Walk_Direction {
    UP, DOWN, LEFT, RIGHT
};

struct Walk_Stack_Entry {
    int  count;
    Walk_Direction dir;
};

#define MAX_WALKSTACK_LEN (1024*1024)
struct Walk_Stack {
    int _pos = -1;
    Walk_Stack_Entry _data[MAX_WALKSTACK_LEN];

    void push(Walk_Stack_Entry to_push) { _data[++_pos] = to_push; assert(_pos < MAX_WALKSTACK_LEN); }
    Walk_Stack_Entry pop(void) { assert(_pos > -1); return _data[_pos--]; }
};

struct Level {
    int h, w, num_start_coords;
    char *data, *solved;
    Walk_Stack stack;

    #define MAX_START_COORDS (1024*1024)
    struct Coord {
        int row, col;
    } start_coords[MAX_START_COORDS];

    Level(void);
    void print    (void);
    char at       (int row, int col) { return data[row*w+col]; }
    void collect_viable_start_coords(void);
    bool is_solved(void) { return (memcmp(data, solved, w*h) == 0); };
    bool solve    (int, int);
    bool go_up    (int, int);
    bool go_down  (int, int);
    bool go_left  (int, int);
    bool go_right (int, int);

    void push_dir (Walk_Stack_Entry dir) { stack.push(dir); }

    Walk_Stack_Entry
    pop_dir       (void) { return stack.pop(); }
};

void Level::collect_viable_start_coords(void)
{
    int start_counter = 0;
    for (int row = 1; row < (h - 1); ++row)
        for (int col = 1; col < (w - 1); ++col) {
            if (data[row*w+col] == '.') {
                //printf("Viable start coordinate at Row: %d / Col: %d\n", row, col);
                assert(start_counter < MAX_START_COORDS);
                start_coords[start_counter++] = {row, col};
            }
        }
    printf("%d viable start coordinates found\n", start_counter);
    num_start_coords = start_counter;
}

void Level::print()
{
    //printf("Hight: %d - Width: %d\n", h, w);
    system("cls");
    for (int row = 0; row < h; ++row)
        printf("%.*s\n", w, data+(row*w));
    #if 0
    for (int row = 0; row < h; ++row)
        printf("%.*s\n", w, solved+(row*w));
    #endif
}

bool Level::solve(int row, int col)
{
    assert(at(row, col) == 'o');
    return (
        go_up(row, col) || go_down(row, col) || go_left(row, col) || go_right(row, col)
    );
}

bool Level::go_up(int row, int col)
{
    row--;
    if (at(row, col) != '.') return false;

    char *pos = data+(row*w+col);
    int step_count = 0, advance = -w;

    while (*pos == '.') {
        *pos = 'o';
        step_count++;
        
        //prepare for next step
        pos += advance;
        row--;
    }
    // goback to last valid step
    pos -= advance;
    row++;

    bool success = false;
    // can I go left or right?
    push_dir({step_count, Walk_Direction::UP});
    if (at(row, col-1) == '.' || at(row, col+1) == '.') {
        // push to stack and go deeper
        success = (go_left(row, col) || go_right(row, col));
    } else {
        // we cannot, so lets check if we have solved it
        success = (is_solved());
    }
    
    if (success)
        return true;

    // we did not, so we have to undo whatever we did
    pop_dir();
    while (step_count--) {
        assert(*pos == 'o');
        *pos = '.';
        pos -= advance;
    }

    return false;
}

bool Level::go_down(int row, int col)
{
    row++;
    if (at(row, col) != '.') return false;

    char *pos = data+(row*w+col);
    int step_count = 0, advance = w;

    while (*pos == '.') {
        *pos = 'o';
        step_count++;
        
        //prepare for next step
        pos += advance;
        row++;
    }
    // goback to last valid step
    pos -= advance;
    row--;

    bool success = false;
    // can I go left or right?
    push_dir({step_count, Walk_Direction::DOWN});
    if (at(row, col-1) == '.' || at(row, col+1) == '.') {
        // push to stack and go deeper
        success = (go_left(row, col) || go_right(row, col));
    } else {
        // we cannot, so lets check if we have solved it
        success = is_solved();
    }

    if (success)
        return true;

    // we did not, so we have to undo whatever we did
    pop_dir();
    while (step_count--) {
        assert(*pos == 'o');
        *pos = '.';
        pos -= advance;
    }

    return false;
}

bool Level::go_left(int row, int col)
{
    col--;
    if (at(row, col) != '.') return false;

    char *pos = data+(row*w+col);
    int step_count = 0, advance = -1;

    while (*pos == '.') {
        *pos = 'o';
        step_count++;
        
        //prepare for next step
        pos += advance;
        col--;
    }
    // goback to last valid step
    pos -= advance;
    col++;

    bool success = false;
    // can I go up or down?
    push_dir({step_count, Walk_Direction::LEFT});
    if (at(row-1, col) == '.' || at(row+1, col) == '.') {
        // push to stack and go deeper
        success = (go_up(row, col) || go_down(row, col));
    } else {
        // we cannot, so lets check if we have solved it
        success = is_solved();
    }

    if (success)
        return true;

    // we did not, so we have to undo whatever we did
    pop_dir();
    while (step_count--) {
        assert(*pos == 'o');
        *pos = '.';
        pos -= advance;
    }

    return false;
}

bool Level::go_right(int row, int col)
{
    col++;
    if (at(row, col) != '.') return false;

    char *pos = data+(row*w+col);
    int step_count = 0, advance = 1;

    while (*pos == '.') {
        *pos = 'o';
        step_count++;
        
        //prepare for next step
        pos += advance;
        col++;
    }
    // goback to last valid step
    pos -= advance;
    col--;

    bool success = false;
    // can I go up or down?
    push_dir({step_count, Walk_Direction::RIGHT});
    if (at(row-1, col) == '.' || at(row+1, col) == '.') {
        // push to stack and go deeper
        success = (go_up(row, col) || go_down(row, col));
    } else {
        // we cannot, so lets check if we have solved it
        success = is_solved();
    }

    if (success)
        return true;

    // we did not, so we have to undo whatever we did
    pop_dir();
    while (step_count--) {
        assert(*pos == 'o');
        *pos = '.';
        pos -= advance;
    }

    return false;
}

/*
This proc only has to work the first time being called.
*/
void make_level_from_stdin(Level *l)
{
    //
    // 1. read and parse level data from stdin
    static char input_buffer[1024*1024];
    int bytes_read = fread(input_buffer, 1, sizeof(input_buffer), stdin);
    assert(bytes_read < sizeof(input_buffer));
    input_buffer[bytes_read+1] = 0;

    int h, w;
    char *walls = 0;
    extract_level_data(input_buffer, &h, &w, &walls);
    //printf("Height: %d\nWidth : %d\nWalls : %s\nSize in bytes: %d\n", h, w, walls, (h+2)*(w+2));

    //
    // 2. create internal representation of level
    // - encircle level with extra "walls" for less edge cases --> additional two bytes for each dim
    // - create a solved repr of the level for easy checking of success
    l->h = h+2;
    l->w = w+2;
    l->data = (char*)malloc(l->h*l->w);
    l->solved = (char*)malloc(l->h*l->w);
    assert(l->data && l->solved);
    // create encircled level
    memset(l->data, (int)'#', l->w*l->h);
    for (int row = 1; row < l->h-1; ++row)
        for (int col = 1; col < l->w-1; ++col)
            l->data[row*l->w+col] = *walls++;
    // create solved version
    memcpy(l->solved, l->data, l->w*l->h);
    for (int row = 1; row < l->h-1; ++row)
        for (int col = 1; col < l->w-1; ++col)
            if (l->solved[row*l->w+col] == '.')
                l->solved[row*l->w+col] = 'o';

}

Level::Level(void)
{
    //
    // 1. read and parse level data from stdin
    static char input_buffer[1024*1024];
    int bytes_read = fread(input_buffer, 1, sizeof(input_buffer), stdin);
    assert(bytes_read < sizeof(input_buffer));
    input_buffer[bytes_read+1] = 0;

    int _h, _w;
    char *walls = 0;
    extract_level_data(input_buffer, &_h, &_w, &walls);
    //printf("Height: %d\nWidth : %d\nWalls : %s\nSize in bytes: %d\n", h, w, walls, (h+2)*(w+2));

    //
    // 2. create internal representation of level
    // - encircle level with extra "walls" for less edge cases --> additional two bytes for each dim
    // - create a solved repr of the level for easy checking of success
    h = _h+2;
    w = _w+2;
    data = (char*)malloc(h*w);
    solved = (char*)malloc(h*w);
    assert(data && solved);
    // create encircled level
    memset(data, (int)'#', w*h);
    for (int row = 1; row < h-1; ++row)
        for (int col = 1; col < w-1; ++col)
            data[row*w+col] = *walls++;
    // create solved version
    memcpy(solved, data, w*h);
    for (int row = 1; row < h-1; ++row)
        for (int col = 1; col < w-1; ++col)
            if (solved[row*w+col] == '.')
                solved[row*w+col] = 'o';

    collect_viable_start_coords();


}
