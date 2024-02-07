#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RLEList.h"

typedef enum Boolean{False, True}Boolean;

struct RLEList_t{
    char val;
    int repetitions;
    Boolean isDummy;
    struct RLEList_t *next;
};

typedef struct RLEList_t *RLEList;

char* intToString(int num);
RLEListResult updateList(RLEList list);

RLEList RLEListCreate(){ 
    //creates a 'dummy' node followed by a real node
    RLEList dummyPtr = (RLEList)malloc(sizeof(RLEList));
    if(dummyPtr == NULL){
        return NULL;
    }
    dummyPtr -> isDummy = True;

    //creates the real node
    RLEList ptr = (RLEList)malloc(sizeof(RLEList));
    if(ptr == NULL){
        return NULL;
    }
    dummyPtr -> next = ptr;
    //puts -1 in (ptr -> val) to help us know if the node is initialized
    //note that -1 is not a possible value for a char
    ptr -> val = -1;
    return dummyPtr;
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
    //going to the last node
    while((list -> next) != NULL){
        list = list -> next;
    }
    //if last node is empty, put values in it
    if(list -> val == -1){
        list -> val = value;
        list -> repetitions = 1;
        return RLE_LIST_SUCCESS;
    }            
    //Check if we need to add a new node or not
    else if(list -> val == value){                      
        list -> repetitions ++;
        return RLE_LIST_SUCCESS;
    }
    //Create a new node
    RLEList newNode = malloc(sizeof(RLEList));
    if(newNode == NULL){
        return RLE_LIST_OUT_OF_MEMORY;
    }
    //add the new node to the end of the list
    newNode -> val = value;
    newNode -> repetitions = 1;
    newNode -> next = NULL;
    list -> next = newNode;
    //node added successfully
    return RLE_LIST_SUCCESS;
}

int RLEListSize(RLEList list){
    if(list == NULL){
        return -1;
    }
    //going through the list and sum the number of repetitons of each node
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

    //checks if the index is out of bounds
    if(index < 0 || index >= RLEListSize(list)){
        return RLE_LIST_INDEX_OUT_OF_BOUNDS;
    }

    //make a copy of the list for later use (when reorganizing the list at the end)
    RLEList listCopy = list;

    // looking one node ahead for us to be able to delete it
    //we will start with the first 'real' node and not the 'dummy' node
    RLEList currentNode = list -> next; 
    //search for the index in each node range
    int rangeBeginning = 0;
    int rangeEnd = currentNode -> repetitions;
    while(currentNode != NULL){
        //search for index in range
        if(index >= rangeBeginning && index < rangeEnd){
            //removes currentNode if necessary
            if(currentNode -> repetitions == 1){
                list -> next = currentNode -> next;
                free(currentNode); 
            }
            else{
                (currentNode -> repetitions)--;
            }
            //reorganize the list before returning
            RLEListResult result = updateList(listCopy);
            return RLE_LIST_SUCCESS;
        }
        //countinue looking in the next node
        currentNode = currentNode -> next;
        list = list -> next;
        rangeBeginning = rangeEnd;
        rangeEnd += currentNode -> repetitions;
    }

    //return unreachable value
    return -1;
}

char RLEListGet(RLEList list, int index, RLEListResult *result){
    if(list == NULL){
        *result = RLE_LIST_NULL_ARGUMENT;
        return 0;
    }
    if(index < 0 || index >= RLEListSize(list)){
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
        char *repetitionsString = intToString(list -> repetitions);

        int stringLength = strlen(repetitionsString); /* returns the size of the numer (list -> repetitions) */

        char *nodeString = malloc((stringLength + 2)*sizeof(char));
        if(nodeString == NULL){
            *result = RLE_LIST_NULL_ARGUMENT;
            return NULL;
        }
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

RLEListResult RLEListMap(RLEList list, MapFunction map_function){ /* typedef char (*MapFunction)(char); */
    if(list == NULL || map_function == NULL){
        return RLE_LIST_NULL_ARGUMENT;
    }
    while(list != NULL){
        list -> val = map_function(list -> val);
        while(list -> next != NULL && list -> val == map_function((list -> next) -> val)){
            list -> repetitions += ( list -> next ) -> repetitions;
            RLEList toDelete = list -> next; 
            list -> next = (list -> next) -> next;
            free(toDelete);
        }
        list = list -> next;
    }
    return RLE_LIST_SUCCESS; 
}

/*
    this function return a char array with the number num as a string
*/
char* intToString(int num){
    int intSize = 1;
    int sizeCheck = 10;
    while(num >= sizeCheck){
        intSize++;
        sizeCheck *= 10;
    }

    sizeCheck = sizeCheck/10; /* the length of sizeCheck is equal to the length of intSize */

    char *returnString = malloc(intSize*sizeof(char));
    if(returnString == NULL){
        return NULL;
    }
    for(int i = 0; i < intSize; i++){
        returnString[i] = (num - (num % sizeCheck))/sizeCheck + 48; /* found the i digit from the left*/
        num -= (returnString[i]-48)*sizeCheck;
        sizeCheck = sizeCheck/10;  
    }

    return returnString;
}

RLEListResult updateList(RLEList list){
    if (list == NULL){
        return RLE_LIST_NULL_ARGUMENT;
    }
    RLEList thisNode = list;
    RLEList nextNode = thisNode -> next;
    if (nextNode == NULL){
        return RLE_LIST_SUCCESS;
    }
    
    while ((thisNode -> next) != NULL){
        // Runs through all List and combines two consecutive nodes with the same value
        if ((thisNode -> val) == (nextNode -> val)){
            thisNode -> repetitions += nextNode -> repetitions;
            thisNode -> next = nextNode -> next;
            // Deletes next node
            free(nextNode);
            continue; // Checks if values are different with new "nextNode"
        }
        thisNode = nextNode;
        nextNode = nextNode -> next;
    }
    return RLE_LIST_SUCCESS;
}