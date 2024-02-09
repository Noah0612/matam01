#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "RLEList.h"

#define ASCII_ZERO 48
#define NOT_INITIALIZED -1

typedef enum Boolean{False, True}Boolean;

struct RLEList_t{
    char value;
    int repetitions;
    Boolean isDummy;
    struct RLEList_t *next;
};

char* intToString(int num);
void updateList(RLEList list);
int numOfNodes(RLEList list);
int numOfDigits(RLEList list);

RLEList createNode(){
    RLEList node = malloc(sizeof (RLEList));
    if (!node){
        return NULL;
    }
    node -> value = NOT_INITIALIZED;
    node -> next = NULL;
    node -> isDummy = False;
    return node;
}

RLEList RLEListCreate(){
    //Creates a 'dummy' node followed by a real node
    RLEList dummyPtr = createNode();
    if(!dummyPtr){
        return NULL;
    }
    dummyPtr -> isDummy = True;

    //Creates the real node
    RLEList firstNode = createNode();
    if(!firstNode){
        return NULL;
    }

    dummyPtr -> next = firstNode;
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
    /*
    if(list == NULL){
        return RLE_LIST_NULL_ARGUMENT;
    }
    //Going to the last node
    while((list -> next) != NULL){
        list = list -> next;
    }
    //If last node is empty, put values in it
    if(list -> value == NOT_INITIALIZED){
        list -> value = value;
        list -> repetitions = 1;
        return RLE_LIST_SUCCESS;
    }
        //Check if we need to add a new node or not
    else if(list -> value == value){
        list -> repetitions ++;
        return RLE_LIST_SUCCESS;
    }
    //Create a new node
    RLEList newNode = malloc(sizeof(RLEList));
    if(newNode == NULL){
        return RLE_LIST_OUT_OF_MEMORY;
    }
    //Add the new node to the end of the list
    newNode -> value = value;
    newNode -> repetitions = 1;
    newNode -> next = NULL;
    list -> next = newNode;
    //Node added successfully
     */
    if (list == NULL || value <= 0){
        return RLE_LIST_NULL_ARGUMENT;
    }
    // Skips dummy
    RLEList node = list -> next;
    // Gets to last node
    while (node -> next != NULL){
        node = node -> next;
    }
    //Checks if it's the same character
    if (node -> value == value){
        node -> repetitions++;
        return RLE_LIST_SUCCESS;
    }

    RLEList newNode = createNode();
    if (!newNode){
        return RLE_LIST_OUT_OF_MEMORY;
    }
    newNode -> value = value;

    node -> next = newNode;

    return RLE_LIST_SUCCESS;
}

int RLEListSize(RLEList list){
    if(list == NULL){
        return -1;
    }
    //Skipping the 'dummy' node
    list = list -> next;
    //Going through the list and sum the number of repetitons of each node
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

    int originalListSize = RLEListSize(list);

    //Checks if the index is out of bounds
    if(index < 0 || index >= originalListSize){
        return RLE_LIST_INDEX_OUT_OF_BOUNDS;
    }

    //Make a copy of the list for later use (when reorganizing the list at the end)
    RLEList listCopy = list;

    //Looking one node ahead for us to be able to delete it
    //skipping the 'dummy' node
    RLEList currentNode = list -> next;
    //Search for the index in each node range
    int rangeBeginning = 0;
    int rangeEnd = currentNode -> repetitions;
    while(currentNode != NULL){
        //Search for index in range
        if(index >= rangeBeginning && index < rangeEnd){
            //Removes currentNode if necessary
            if(currentNode -> repetitions == 1){
                //If the size of the list is 1, dont remove the node just change it to empty value
                if(originalListSize == 1){
                    currentNode -> value = NOT_INITIALIZED;
                    currentNode -> repetitions = 0;
                    return RLE_LIST_SUCCESS;
                }
                list -> next = currentNode -> next;
                free(currentNode);
            }
            else{
                (currentNode -> repetitions)--;
            }
            //Reorganize the list before returning
            updateList(listCopy);
            return RLE_LIST_SUCCESS;
        }
        //Countinue looking in the next node
        currentNode = currentNode -> next;
        list = list -> next;
        rangeBeginning = rangeEnd;
        rangeEnd += currentNode -> repetitions;
    }

    return RLE_LIST_ERROR;
}

char RLEListGet(RLEList list, int index, RLEListResult *result){
    if(list == NULL){
        if(result != NULL){
            *result = RLE_LIST_NULL_ARGUMENT;
            return 0;
        }
        return 0;
    }
    //Checks if the index is out of bounds
    if(index < 0 || index >= RLEListSize(list)){
        if(result != NULL){
            *result = RLE_LIST_INDEX_OUT_OF_BOUNDS;
            return 0;
        }
        return 0;
    }

    //Skipping the first 'dummy' node
    list = list -> next;

    //Searching for the index in the nodes range
    int rangeBeginning = 0;
    int rangeEnd = 0;
    while(list != NULL){
        //Moving the range
        rangeBeginning = rangeEnd;
        rangeEnd += list -> repetitions;

        if(index >= rangeBeginning && index < rangeEnd){
            if(result != NULL){
                *result = RLE_LIST_SUCCESS;
                return list -> value;
            }
            return list -> value;
        }

        //Countinue searching in the next node
        list = list -> next;
    }

    //Return unreachable value
    return -1;
}

char* RLEListExportToString(RLEList list, RLEListResult* result){
    if(list == NULL){
        if(result != NULL){
            *result = RLE_LIST_NULL_ARGUMENT;
            return NULL;
        }
        return NULL;
    }

    //Allocates memory for a return string.
    //The size of the string equals to the sum of:
    //  1) num of nodes in the list multiplie by 2 -> every node donates two charcters : (node -> value) and '\n'
    //  2) sum of digits of the repetitions
    //  3) +1 for '\0' at the end of the string to make it null terminated
    int returnStringSize = (numOfNodes(list)*2 + numOfDigits(list) + 1);
    char* returnString = malloc(returnStringSize*sizeof(char));
    if(returnString == NULL){
        if(result != NULL){
            *result = RLE_LIST_OUT_OF_MEMORY;
            return NULL;
        }
        return NULL;
    }


    //Skipping the 'dummy' node
    list = list -> next;
    int index = 0;
    while(list != NULL){
        char* repetitionsString = intToString(list -> repetitions);
        if(repetitionsString == NULL){
            free(returnString);
            if(result != NULL){
                *result = RLE_LIST_OUT_OF_MEMORY;
                return NULL;
            }
            return NULL;
        }
        //Assigning values to returnString
        int repetitionsDigits = strlen(repetitionsString);
        returnString[index] = list -> value;
        for(int j = 0; j < repetitionsDigits; j++){
            returnString[index + j + 1] = repetitionsString[j];
        }
        returnString[index + repetitionsDigits + 1] = '\n';

        //Freeing used space
        free(repetitionsString);
        //Moving the index ahead
        index = index + repetitionsDigits + 2;
        list = list -> next;
    }

    //Putting '\0' at the end of the return string
    returnString[returnStringSize - 1] = '\0';
    if(result != NULL){
        *result = RLE_LIST_SUCCESS;
        return returnString;
    }
    return returnString;
}

RLEListResult RLEListMap(RLEList list, MapFunction map_function){

    if(list == NULL || map_function == NULL){
        return RLE_LIST_NULL_ARGUMENT;
    }
    //Make a copy of the list for later use (when reorganizing the list)

    RLEList listCopy = list;
    //Skipping the 'dummy' node

    list = list -> next;
    while(list != NULL){
        list -> value = map_function(list -> value);
        list = list -> next;
    }
    updateList(listCopy);
    return RLE_LIST_SUCCESS;
}

//Returns a char array with the number num as a string
char* intToString(int num){
    int numDigits = 1;
    int divisor = 10;

    //Finding the number of digits in num
    while(num >= divisor){
        numDigits++;
        divisor *= 10;
    }

    //The number of digits in (divisor/10) is uqeal to the number of digits in num
    divisor = divisor/10;

    //Creates a string the size of the number of digits in num + 1 to add '\0' at the end
    char *returnString = malloc((numDigits+1)*sizeof(char));
    if(returnString == NULL){
        return NULL;
    }

    //Put values in returnString
    for(int i = 0; i < numDigits; i++){
        //The number ((num - (num % divisor))/divisor) is equal to the i digit from the left in num
        //Add ASCII_ZERO to make it a char
        returnString[i] = ((num - (num % divisor))/divisor) + ASCII_ZERO;

        //Moving to the next digit
        //Removing the leftmost digit in num
        num -= num - (num % divisor);
        divisor = divisor/10;
    }
    //Make the string null terminated
    returnString[numDigits] = '\0';
    return returnString;
}


//This function returns the number of nodes in given RLEList list
int numOfNodes(RLEList list){
    if(list == NULL){
        return -1;
    }
    //Skipping the 'dummy' node
    list = list -> next;

    //list is not intialized yet, size is 0
    if(list -> value == -1){
        return 0;
    }
    //Going through the list and counting the nodes
    int count = 0;
    while(list != NULL){
        count++;
        list = list -> next;
    }
    return count;
}

//This function counts the sum of the number of digits in the list repetitions
int numOfDigits(RLEList list){
    if(list == NULL){
        return -1;
    }
    //Skipping the 'dummy' node
    list = list -> next;
    //return 0 if list is not intialized yet
    if(list -> value == -1){
        return 0;
    }
    //Starting to count
    int sum = 0;
    while(list != NULL){
        char *repetitonsString = intToString(list -> repetitions);
        if(repetitonsString == NULL){
            return -1;
        }
        //Adding the number of digits to sum
        sum += strlen(repetitonsString);
        free(repetitonsString);
        list = list -> next;
    }
    return sum;
}

//This function run through the list and update nodes if neccessary
void updateList(RLEList list){
    if(list == NULL){
        return;
    }
    //Skipping the 'dummy' node
    list = list -> next;
    RLEList thisNode = list;
    RLEList nextNode = thisNode -> next;
    while (nextNode != NULL){
        // Runs through all List and combines two consecutive nodes with the same value
        if ((thisNode -> value) == (nextNode -> value)){
            //If finds two consecutive nodes with the same value, updating the first node and
            //delete the second node. If the next node after deleting equal to the first node value,
            //repeat the process ...
            while((thisNode -> value) == ((nextNode -> value))){
                thisNode -> repetitions += nextNode -> repetitions;
                thisNode -> next = nextNode -> next;
                // Deletes next node
                free(nextNode);
                nextNode = thisNode -> next;
                if(nextNode == NULL){
                    //There are no more nodes to merge
                    return;
                }
            }
            continue; // Checks if values are different with new "nextNode"
        }
        thisNode = nextNode;
        nextNode = nextNode -> next;
    }
    return;
}