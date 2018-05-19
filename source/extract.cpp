#define TOKEN_WIDTH  "<width>"
#define TOKEN_HEIGHT "<height>"
#define TOKEN_WALLS  "<walls>"

#define TOKEN_CMP(str, tok) (strncmp(str, tok, strlen(tok)) == 0)

int _extract_int_and_advance(const char *at, const char *token)
{
    at += strlen(token);
    int result = atoi(at);
    return result;
}

void extract_level_data(const char *at, int *out_h, int *out_w, char **out_walls)
{
    char *walls = 0;
    int w = 0, h = 0;
    bool extracting = true;
    while (extracting && *at) {
        if (*at == '<') {
            if (TOKEN_CMP(at, TOKEN_WIDTH)) {
                w = _extract_int_and_advance(at, TOKEN_WIDTH);

            } else if (TOKEN_CMP(at, TOKEN_HEIGHT)) {
                h = _extract_int_and_advance(at, TOKEN_HEIGHT);

            } else if (TOKEN_CMP(at, TOKEN_WALLS)) {
                at += strlen(TOKEN_WALLS);
                walls = (char*)malloc((h * w) + 1);
                assert(walls);
                char *tmp = walls;
                while (*at != '<') {
                    if (*at == '.' || *at == 'X')
                        *(tmp++) = *at;
                    at++;
                }
                *tmp = '\0';
                extracting = false;

            } else {
                //printf("Unused\n");
            }
        }
        ++at;
    }
    *out_h = h;
    *out_w = w;
    assert(walls);
    *out_walls = walls;
}