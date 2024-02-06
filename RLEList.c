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
    if(list == NULL){
        return RLE_LIST_NULL_ARGUMENT;
    }
    while((list -> next) != NULL){                               /*going to the last node */
        list = list -> next;
    }                                              /* list is the last node */
    if(list -> val == value){                      /*checking if need to create new node*/
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

    if(index >= 0 && index < range){ /* first time*/
        printf("index >= 0 && index <= range\n");
        if(list -> repetitions == 1){ 
            /*
            *                           PROBLEM!
                what happend if the index is in the first node? I cannot change
                the (RLEList list) to list -> next beacuse it is not a pointer to a pointer.
            */
            RLEList toDelete = list;
            list = list -> next;
            free(toDelete);
        }
        else{
            (list -> repetitions)--;
        }
        return RLE_LIST_SUCCESS;
    } 

    while((list -> next) != NULL && index >= 0){
        if(index >= range && index < range + ((list -> next) -> repetitions)){ /*found the node of the index*/
            if((list -> next) -> repetitions == 1){ /* removes entire node */
                RLEList ptr = list -> next;
                list -> next = (list -> next) -> next;
                free(ptr); 
            }
            else{
                ((list -> next) -> repetitions)--;
            }
            return RLE_LIST_SUCCESS;
        }
        range = range + ((list -> next) -> repetitions);
        list = list -> next;
    }
    return -1; /* SHOULD NEVER BE HERE */
}

char RLEListGet(RLEList list, int index, RLEListResult *result){
    if(list == NULL){
        *result = RLE_LIST_NULL_ARGUMENT;
        return 0;
    }
    if(index < 0 || index > RLEListSize(list)){
        *result = RLE_LIST_INDEX_OUT_OF_BOUNDS;
        return 0;
    }
    int range = 0;
    while(list != NULL){
        if(index >= range && index < range + (list -> repetitions)){ /*found the node of the index*/
            *result = RLE_LIST_SUCCESS;
            return list -> val;
        }
        range = range + (list -> repetitions);
        list = list -> next;
    }
    return -1; /* SHOULD NEVER BE HERE */
}

char* RLEListExportToString(RLEList list, RLEListResult* result){ /*fix return value!!!*/
    if(list == NULL){
        *result = RLE_LIST_NULL_ARGUMENT;
        return NULL;
    }

    char *returnString = malloc(0);
    if(returnString == NULL){
        *result = RLE_LIST_OUT_OF_MEMORY;
        return NULL;
    }

    while(list != NULL){
        char *repetitionsString = malloc(12*sizeof(char)); /* maximum length of an int is 12 ~ 4 BILLION */
        sprintf(repetitionsString, "%d", list -> repetitions);

        int stringLength = strlen(repetitionsString); /* returns the size of the numer (list -> repetitions) */

        char *nodeString = malloc((stringLength + 2)*sizeof(char));
        nodeString[0] = list -> val;
        for(int j = 0; j < stringLength; j++){
            nodeString[j+1] = repetitionsString[j]; /* nodeString is j+1 because it has (list -> val) in place 0 */
        }
        nodeString[1+stringLength] = '\n'; 

        returnString = realloc(returnString,(strlen(returnString)+strlen(nodeString))*sizeof(char)); /* reallocate memory right to the returnString memory*/
        if(returnString == NULL){
            *result = RLE_LIST_NULL_ARGUMENT;
            return NULL;
        }
        strcat(returnString, nodeString); /* combining the strings */
        list = list -> next; /* moving with the while loop */
    }
    
    result = RLE_LIST_SUCCESS;
    return returnString;
}

RLEListResult RLEListMap(RLEList list, MapFunction map_function){
    if(list == NULL || map_function == NULL){
        return RLE_LIST_NULL_ARGUMENT;
    }
    while(list != NULL){
        list -> val = map_function(list -> val);
        list = list -> next;
    }
    return RLE_LIST_SUCCESS;
}