#include <stdio.h>
#include "RLEList.h"

struct RLEList_t{
    char value;
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
    while(list)
    {
        RLEList toDelete = list;
        list = list -> next;
        free(toDelete);
    }
}
