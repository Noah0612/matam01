#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RLEList.h"

struct RLEList_t{
    char val;
    int repetitions;
    struct RLEList_t *next;
};

typedef struct RLEList_t *RLEList;

RLEList RLEListCreate(){
    RLEList ptr = (RLEList)malloc(sizeof(RLEList));
    if(ptr == NULL){
        return NULL;
    }
    ptr -> next = NULL;/* <------------------------------------------- IMPORTANT????*/
    return ptr;
}

void RLEListDestroy(RLEList list){
    while(list != NULL){
        RLEList toDelete = list;
        list = list -> next;
        free(toDelete);
    }
}


RLEListResult RLEListAppend(RLEList list, char value){
    if(list == NULL){ /*list == NULL*/
        return RLE_LIST_NULL_ARGUMENT;
    }
    RLEList nextNode = list -> next;
    while(nextNode){ /*going to the last node */
        list = nextNode;
        nextNode = list -> next;
    } /* list is the last node */
    if(list -> val == value){ /*checking if need to create new node*/
        list -> repetitions ++;
        return RLE_LIST_SUCCESS;
    }
    RLEList newNode = malloc(sizeof(RLEList));
    if(!newNode){
        return RLE_LIST_OUT_OF_MEMORY;
    }
    newNode -> val = value;
    newNode -> repetitions = 1;
    newNode -> next = NULL;
    list -> next = newNode;
    return RLE_LIST_SUCCESS;

    /*
        --------------------------------------------what if list is empty?????
    */
}

int RLEListSize(RLEList list){
    if(list == NULL){
        return -1;
    }
    int sum = 0;
    while(list != NULL){
        sum += list -> repetitions;
        list = list -> next;
    }
    return sum;
}

RLEListResult RLEListRemove(RLEList list, int index){
    if(list == NULL){
        return RLE_LIST_NULL_ARGUMENT;
    }
    if(index < 0 || index > RLEListSize(list)){ /*------------------also checking if index < 0 */
        return RLE_LIST_INDEX_OUT_OF_BOUNDS;
    }
    int range = list -> repetitions;
    if(index >= 0 && index <= range){ /* first time*/
        if(list -> repetitions == 1){ /* removes entire node ----> CANNOT DELETE ENTIRE NODE BECAUSE GETTING A POINTER AND NOT DOUBLE POINTER!*/
            RLEList toDelete = list;
            list = list -> next;
            free(toDelete);
        }
        else{
            (list -> repetitions)--;
        }
        index = -1; /*get out of the loop*/
    } 
    while((list -> next) != NULL && index >= 0){
        if(index >= range && index <= range + ((list -> next) -> repetitions)){ /*found the node of the index*/
            if((list -> next) -> repetitions == 1){ /* removes entire node*/
                RLEList ptr = list -> next;
                list -> next = (list -> next) -> next;
                free(ptr); 
            }
            else{
                ((list -> next) -> repetitions)--;
            }
            index = -1; /*get out of the loop*/
        }
        range = range + ((list -> next) -> repetitions);
        list = list -> next;
    }
}

char RLEListGet(RLEList list, int index, RLEListResult *result){
    RLEListResult resultRle;
    if(list == NULL){
        resultRle = RLE_LIST_NULL_ARGUMENT;
        result = &resultRle;
        return 0;
    }
    if(index < 0 || index > RLEListSize(list)){
        resultRle = RLE_LIST_INDEX_OUT_OF_BOUNDS;
        result = &resultRle;
        return 0;
    }
    int range = list -> repetitions;
    while(list != NULL){
        if(index >= range && index <= range + (list -> repetitions)){ /*found the node of the index*/
            resultRle = RLE_LIST_SUCCESS;
            result = &resultRle;
            return list -> val;
        }
        range = range + (list -> repetitions);
        list = list -> next;
    }
    return -1; /*THERE IS A PROBLEM IF IT GET HERE*/
}

char* RLEListExportToString(RLEList list, RLEListResult* result){
    RLEListResult resultRle;
    if(list == NULL){
        resultRle = RLE_LIST_NULL_ARGUMENT;
        result = &resultRle;
        return NULL;
    }

    char *returnString = malloc(4*sizeof(char));
    if(returnString == NULL){
        resultRle = RLE_LIST_OUT_OF_MEMORY;
        result = &resultRle;
        return NULL;
    }
    while(list != NULL){
        char *repetitionsStringZero = malloc(12*sizeof(char));
        sprintf(repetitionsStringZero, "%d", list -> repetitions);
        int stringLength = 0;
        int i = 11;
        while(stringLength == 0){
            if(repetitionsStringZero[i] != 0){
                stringLength = i;
            }
        } 
        char *repetitionsString = malloc((stringLength + 3)*sizeof(char));
        repetitionsString[0] = list -> val;
        for(int j = 0; j < stringLength; j++){
            repetitionsString[j+1] = repetitionsStringZero[j+stringLength];
        }
        repetitionsString[1+stringLength] = '\\';
        repetitionsString[2+stringLength] = 'n';

        returnString = realloc(returnString,(strlen(returnString)+stringLength)*sizeof(char));
        if(returnString == NULL){
            resultRle = RLE_LIST_NULL_ARGUMENT;
            result = &resultRle;
            return NULL;
        }
        strcat(returnString, repetitionsString);
    }
    
    resultRle = RLE_LIST_SUCCESS;
    result = &resultRle;
    return returnString;
}