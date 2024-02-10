#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RLEList.h"

#define ASCII_ZERO 48
#define NOT_INITIALIZED -1

typedef enum Boolean{False, True}Boolean;

typedef struct RLEListNode{
    char value;
    int repetitions;
    Boolean isDummy;
    struct RLEListNode *next;
    struct RLEListNode *prev;
} RLEListNode_t;

struct RLEList_t {
    RLEListNode_t* first;
    RLEListNode_t* last;
    int size;
    int count; 
};

char* intToString(int num);
void updateList(RLEList list);
int numOfNodes(RLEList list);
int numOfDigits(RLEList list);

RLEListNode_t* createNode(){
    RLEListNode_t* node = (RLEListNode_t*) malloc(sizeof (RLEListNode_t));
    if (!node){
        return NULL;
    }
    node->value = NOT_INITIALIZED;
    node->next = NULL;
    node->prev = NULL;
    node->isDummy = False;
    return node;
}

RLEList RLEListCreate(){
    //Creates a 'dummy' node followed by a real node
    RLEList list = (RLEList) malloc(sizeof(struct RLEList_t));

    RLEListNode_t* dummyPtr = createNode();
    if(!dummyPtr){
        return NULL;
    }
    dummyPtr->isDummy = True;

    list->first = dummyPtr;
    list->last = dummyPtr;
    list->size = 0;
    list->count = 0;

    return list;
}

void RLEListDestroy(RLEList list){
    RLEListNode_t* pos = list->first;
    RLEListNode_t* temp;

    while(pos != NULL){
        temp = pos;
        pos = pos->next;
        free(temp);
    }
}

RLEListResult RLEListAppend(RLEList list, char value){
    RLEListNode_t* node = NULL;

    if (list->last->value == value) {
        list->last->repetitions++;
        list->size++; // Added another value
        return RLE_LIST_SUCCESS;
    }

    node = createNode();
    if (node == NULL) {
        return RLE_LIST_OUT_OF_MEMORY;
    }

    node->value = value;
    node->prev = list->last;
    node->repetitions = 1;

    list->size++;
    list->count++;
    list->last->next = node;
    list->last = node;
    
    return RLE_LIST_SUCCESS;
}

int RLEListSize(RLEList list){
    return list->size;
}

RLEListNode_t* GetNodeByIndex(RLEList list, int index, RLEListResult* error) {
    RLEListNode_t* pos = NULL;
    RLEListNode_t* ret = NULL;
    int cur_index = 0;

    if(list == NULL){
        *error = RLE_LIST_NULL_ARGUMENT;
        return NULL;
    }

    //Checks if the index is out of bounds
    if(index < 0 || index >= list->size){
        *error = RLE_LIST_INDEX_OUT_OF_BOUNDS;
        return NULL;
    }

    // Skip dummy
    pos = list->first->next;
    //Find the node to delete from
    while(pos) {
        if (index >= cur_index && index < cur_index + pos->repetitions) {
            ret = pos;
            break;
        }

        cur_index += pos->repetitions;
        pos = pos->next;
    }

    *error = RLE_LIST_SUCCESS;
    return ret;
}

// Assumes node is in the list (MUST BE IN THE LIST)
void RemoveNode(RLEList list, RLEListNode_t* node) {
    list->count--;
    if (node == list->last) {
        list->last = node->prev;
        list->last->next = NULL;
        free(node);
    }
    else {
        node->prev->next = node->next;
        free(node);
    }
}


RLEListResult RLEListRemove(RLEList list, int index){
    RLEListNode_t* to_delete = NULL;
    RLEListResult error = RLE_LIST_ERROR;

    if(list == NULL){
        return RLE_LIST_NULL_ARGUMENT;
    }

    to_delete = GetNodeByIndex(list, index, &error);
    
    // Not supposed to happen, sanity check
    if (!to_delete || error != RLE_LIST_SUCCESS)
        return error;

    to_delete->repetitions--;
    list->size--;

    if (to_delete->repetitions == 0) {
        RemoveNode(list, to_delete);
    }

    updateList(list);

    return RLE_LIST_SUCCESS;
}

char RLEListGet(RLEList list, int index, RLEListResult *result){
    RLEListNode_t* node = NULL;
    RLEListResult error = RLE_LIST_ERROR;

    if(list == NULL){
        if(result != NULL){
            *result = RLE_LIST_NULL_ARGUMENT;
            return 0;
        }
        return 0;
    }
    
    node = GetNodeByIndex(list, index, &error);
    if (!node || error != RLE_LIST_SUCCESS) {
        *result = error;
        return 0;
    }

    *result = RLE_LIST_SUCCESS;
    return node->value;
}




// MAX INT is 2^32 -> 10 decimal digits
#define MAX_INT_SIZE (10)

int WriteDigitsToBuffer(char* ptr, int value) {
    char buffer[MAX_INT_SIZE] = {0};
    int i = 0;
    int len = 0;

    while (value) {
        buffer[i] = (value % 10) + ASCII_ZERO;
        i++;
        value /= 10;
    }

    len = i;

    for (i = i-1; i >= 0; i--) {
        *ptr = buffer[i];
        ptr++;
    }

    return len;
}

char* RLEListExportToString(RLEList list, RLEListResult* result){
    // +1 since there is a \n for each row
    int str_len = list->count * (MAX_INT_SIZE + 1);
    char* str = NULL;
    char* ptr = NULL;
    RLEListNode_t* pos = list->first->next;

    if(list == NULL){
        if(result != NULL){
            *result = RLE_LIST_NULL_ARGUMENT;
            return NULL;
        }
        return NULL;
    }

    // Null terminated string
    str = malloc(str_len * sizeof(char) + 1);
    ptr = str;

    // Check in each iteration that there is enough space in the buffer to write
    while (pos && ptr + MAX_INT_SIZE + 1 < (str + str_len)) {
        *ptr = pos->value;
        ptr++;
        ptr += WriteDigitsToBuffer(ptr, pos->repetitions);
        *ptr = '\n';
        ptr++;
        pos = pos->next;
    }
    *ptr = 0;
    *result = RLE_LIST_SUCCESS;
    return str;
}

RLEListResult RLEListMap(RLEList list, MapFunction map_function){
    RLEListNode_t* pos = NULL;
    
    if(list == NULL || map_function == NULL){
        return RLE_LIST_NULL_ARGUMENT;
    }

    pos = list->first->next;
    while(pos){
        pos->value = map_function(pos->value);
        pos = pos->next;
    }

    updateList(list);

    return RLE_LIST_SUCCESS;
}

//This function runs through the list and update nodes if neccessary
void updateList(RLEList list){
    RLEListNode_t* pos = NULL;

    if(list == NULL){
        return;
    }

    pos = list->first->next;

    while(pos->next) {
        if (pos->value == pos->next->value) {
            pos->repetitions += pos->next->repetitions;
            RemoveNode(list, pos->next);
            continue;
        }
        pos = pos->next;
    }

    return;
}

void debugPrintList(RLEList list) {
    RLEListNode_t* pos = list->first->next;

    for (; pos; pos = pos->next) {
        printf("(%02x, %d)-->", pos->value, pos->repetitions);
    }

    puts("");
}