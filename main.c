#include <stdio.h>
#include "AsciiArtTool.h"
#include <assert.h>


int main (int argc, char** argv) {
    if (argc != 3){
        puts("Invalid number of arguments, Usage: flag <source> <target>");
        return stdout;
    }
    if (argv[1] == NULL || argv[2] == NULL){
        puts("Invalid file");
        return RLE_LIST_NULL_ARGUMENT;
    }

    FILE* source = fopen(argv[1], "r");
    FILE* target = fopen(argv[1], "w");

    if (argv[0] == '-e'){
        RLEList list = asciiArtRead(source);
        RLEListResult result = asciiArtPrintEncoded(list, target);
        RLEListDestroy(list);
        return result;
    }
    else if (argv[0] == '-i'){
        RLEList list = asciiArtRead(source);
        RLEListMap(list, '-i'); // WHAT TO PUT AS ARG FOR map_function :'(
        RLEListResult result = asciiArtPrint(list, target);
        RLEListDestroy(list);
        return result;
    }
    else {
        puts("Incorrect flag");
        return stdout;
    }

}
