//
// Created by noahl on 05/02/2024.
//

#include "AsciiArtTool.h"
#include "RLEList.h"
#include <assert.h>
#include <malloc.h>

#define BUFFER_SIZE 256

RLEList asciiArtRead(char* fileName){
    FILE* fp = NULL;
    RLEList list = NULL;
    unsigned char c = 0;

    assert(fileName);

    list = RLEListCreate();
    if (list == NULL || fileName == NULL){
        return NULL;
    }

    fp = fopen(fileName, "r");
    if (fp == NULL) {
        printf("Error opening %s\n", fileName);
        RLEListDestroy(list);
        return NULL;
    }

    printf("Starts Reading\n");
    while (fread(&c, sizeof(char), 1, fp)) {
        RLEListAppend(list, c);
        printf("%c", c);
    }
    //
    //
    /*
    while (fgets(buffer, BUFFER_SIZE, in_stream)) {
        for (int i = 0; (i < BUFFER_SIZE) && (buffer[i] != 0); i++) {
            RLEListResult result =  RLEListAppend(list, buffer[i]);
            if (result == RLE_LIST_NULL_ARGUMENT || result == RLE_LIST_OUT_OF_MEMORY){
                return NULL;
            }
        }
        printf("Added buffer: %s", buffer);
    }
    fclose(in_stream);
    //
    //
    printf("Read from file");

    return list;
    */
//    char ch;
//    while ((fscanf(fp, "%c", &ch)) != EOF){
//        // Runs through all characters in file
//        RLEListResult result =  RLEListAppend(list, ch);
//        //
//        //
//        printf("%c", ch);
//        //
//        //
//        /*
//        if (result == RLE_LIST_NULL_ARGUMENT || result == RLE_LIST_OUT_OF_MEMORY){
//            return NULL;
//        }
//        if (feof(in_stream)){
//            break;
//        }
//         */
//    }
//    printf("Finished reading");
    fclose(fp);
    return list;

}

RLEListResult asciiArtPrint(RLEList list, char* fileName){
    if (list == NULL || fileName == NULL){
        return RLE_LIST_NULL_ARGUMENT;
    }
    FILE* out_stream = fopen(fileName, "w+");
    int size = RLEListSize(list);
    if (size < 0){
        return RLE_LIST_NULL_ARGUMENT;
    }
    char ch;
    RLEListResult *result = RLE_LIST_SUCCESS;
    for (int i = 0; i < size; ++i) {
        //Runs through each character and prints it in the file
        ch = RLEListGet(list, i, result);
        if (ch == 0){
            fclose(out_stream);
            return *result;
        }
        fprintf(out_stream, "%c", ch);
    }
    fclose(out_stream);
    return RLE_LIST_SUCCESS;
}

RLEListResult asciiArtPrintEncoded(RLEList list, char* fileName){
    if (list == NULL || fileName == NULL){
        return RLE_LIST_NULL_ARGUMENT;
    }
    RLEListResult *result = RLE_LIST_SUCCESS;
    char *strToPrint = RLEListExportToString(list, result);

    if (*result != RLE_LIST_SUCCESS){
        return *result;
    }

    FILE* out_stream = fopen(fileName, "w+");

    fputs(strToPrint, out_stream);

    fclose(out_stream);
    free(strToPrint);
    //
    //
    printf("Printed to file");
    //
    //
    return RLE_LIST_SUCCESS;
}
