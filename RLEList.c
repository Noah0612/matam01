#include <stdio.h>
#include "RLEList.h"

struct RLEList_t{
    char val;
    int repetitions;
    struct RLEList_t *next;
};

typedef struct RLEList_t *RLEList;

RLEList RLEListCreate(){
    RLEList ptr = (RLEList)malloc(sizeof(RLEList_t));
    if(!ptr){
        return NULL;
    }
    return ptr;
}

void RLEListDestroy(RLEList list){
    while(list){
        RLEList toDelete = list;
        list = list -> next;
        free(toDelete);
    }
}


RLEListResult RLEListAppend(RLEList list, char value){
    if(!list){ /*list == NULL*/
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
}