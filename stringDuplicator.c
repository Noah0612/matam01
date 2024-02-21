#include <stdlib.h>
#include <string.h>
#include <assert.h>

char* stringDuplicator(char* str, int times) {
    assert(str);
    assert(times > 0);
    int len;
    char *pos;

    len = strlen(str);

    char *out = malloc(len * sizeof(char) * times + 1);
    if (!out) {
        return NULL;
    }
    pos = out;
    for (int i = 0; i < times; i++) {
        strcpy(pos, str);
        pos = pos + len;
    }
    return out;
}

