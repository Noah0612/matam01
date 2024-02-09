#include <stdio.h>
#include "AsciiArtTool.h"
#include <assert.h>
#include <string.h>
#include <malloc.h>


char invert(char ch){
    if (ch == ' ') {
        return '@';
    }
    else if (ch == '@'){
        return ' ';
    }
    return ch;
}

int main (int argc, char** argv) {
    if (argc != 4){
        puts("Invalid number of arguments, Usage: flag <source> <target>");
        return 0;
    }
    // argv[2] is source file, argv[3] is target file
    if (argv[2] == NULL || argv[3] == NULL){
        puts("Invalid file");
        return 0;
    }
    printf("Debug: running %s %s %s %s\n", argv[0], argv[1], argv[2], argv[3]);

    if (!strcmp(argv[1], "-e")){
        RLEList list = asciiArtRead(argv[2]);
        asciiArtPrintEncoded(list, argv[3]);
        RLEListDestroy(list);
        return 0;
    }
    else if (!strcmp(argv[1], "-i")){
        RLEList list = asciiArtRead(argv[2]);
        RLEListMap(list, invert);
        asciiArtPrint(list, argv[3]);
        RLEListDestroy(list);
        return 0;
    }
    else {
        puts("Incorrect flag");
        return 0;
    }


}
