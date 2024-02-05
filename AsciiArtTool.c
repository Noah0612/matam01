//
// Created by noahl on 05/02/2024.
//

#include "AsciiArtTool.h"
#include "RLEList.h"
#include <assert.h>

RLEList asciiArtRead(FILE* in_stream){
    assert(in_stream);
    fopen(in_stream, "r");

    RLEList list = RLEListCreate();
    if (list == NULL){
        return RLE_LIST_OUT_OF_MEMORY;
    }
    fopen(in_stream, "r");
    char ch;
    while ((fscanf(in_stream, "%c", &ch)) != EOF){
        // Runs through all characters in file
        RLEListResult result =  RLEListAppend(list, ch);
        if (result == RLE_LIST_NULL_ARGUMENT || result == RLE_LIST_OUT_OF_MEMORY){
            return NULL;
        }
    }
    fclose(in_stream);
    return list;
}

RLEListResult asciiArtPrint(RLEList list, FILE *out_stream){
    if (list == NULL || out_stream == NULL){
        return RLE_LIST_NULL_ARGUMENT;
    }
    fopen(out_stream, "w+");
    int size = RLEListSize(list);
    if (size < 0){
        return RLE_LIST_NULL_ARGUMENT;
    }
    char ch;
    for (int i = 0; i < size; ++i) {
        //Runs through each character and prints it in the file
        ch = RLEListGet(list, i, ch);
        if (ch == '0'){ // Checks that we didn't get to the EOF
            break;
        }
        fprintf(out_stream, "%c", ch);
    }
    fclose(out_stream);
    return RLE_LIST_SUCCESS;
}

RLEListResult asciiArtPrintEncoded(RLEList list, FILE *out_stream){
    if (list == NULL || out_stream == NULL){
        return RLE_LIST_NULL_ARGUMENT;
    }
    char *strToPrint;
    RLEListExportToString(list, strToPrint);
    fopen(out_stream, "w+");
    fputs(strToPrint, out_stream);
    fclose(out_stream);
    return RLE_LIST_SUCCESS;
}
