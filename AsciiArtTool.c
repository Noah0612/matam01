//
// Created by noahl on 05/02/2024.
//

#include "AsciiArtTool.h"
#include "RLEList.h"
#include <assert.h>
#include <malloc.h>

#define BUFFER_SIZE 256
#define SMALL_BUFF 3

/*
void addBufferToRLEList(RLEList list, char buff[BUFFER_SIZE]){
    assert(list);
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        RLEListAppend(list,buff[i]);
    }
}
 */

void printCharacterInFile(char ch, int recurrence, FILE* out_stream){
    // Assumes file is open for writing
    for (int i = 0; i < recurrence; ++i) {
        fputc(ch, out_stream);
    }
}

RLEList asciiArtRead(FILE* in_stream){
    assert(in_stream);
    fopen(in_stream, "r");

    RLEList list = RLEListCreate();
    if (!list){
        return NULL;
    }
    /*
    char buffer[BUFFER_SIZE];
    while(fgets(buffer, BUFFER_SIZE, in_stream) != NULL){
        addBufferToRLEList(list, buffer);
    }
    */

    char c = fgetc(in_stream);
    while (c != EOF){
        RLEListAppend(list, c);
        c = fgetc(in_stream);
    }


    fclose(in_stream);
    return list;
}


RLEListResult asciiArtPrint(RLEList list, FILE *out_stream){
    if (list == NULL || out_stream == NULL){
        return RLE_LIST_NULL_ARGUMENT;
    }
    FILE* tmpFile = fopen("tmp.txt", "w+");
    asciiArtPrintEncoded(list, tmpFile);
    fclose(tmpFile);
    fopen(tmpFile, "r");
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, tmpFile) != NULL){
        printCharacterInFile(buffer[0], (int)buffer[1], out_stream);
    }
    fclose(tmpFile);
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
