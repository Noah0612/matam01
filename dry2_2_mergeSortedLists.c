Node mergeSortedLists(Node list1, Node list2){
    //Checking for invalid input
    //The lists should be not NULL and sorted
    if(list1 == NULL || list2 == NULL){
        return NULL;
    }
    if(isListSorted(list1) == false || isListSorted(list2) == false){
        return NULL;
    }
    
    //Initializing the list that we will return
    Node returnList;
    if(list1 -> val < list2 -> val){
        returnList = list1;
        list1 = list1 -> next;
    }
    else{
        returnList = list2;
        list2 = list2 -> next;
    }
    //Creating a copy of returnList for us to be able to run through the list and still remember the first node
    Node currentNode = returnList;
    //Going through the list until we merged both lists
    //Decide each time which node to add to the end of returnList for it to be sorted and merged
    while(list1 != NULL || list2 != NULL){
        //If we get to end of a list, we will automatically add the other list
        if(list1 == NULL){
            currentNode -> next = list2;
            currentNode = currentNode -> next;
            list2 = list2 -> next;
        }
        else if(list2 == NULL){
            currentNode -> next = list1;
            currentNode = currentNode -> next;
            list1 = list1 -> next;
        }
        //Searching for the next node with the smallest value and adding it to the end of returnList
        else if((list1 -> val) < (list2 -> val)){
            currentNode -> next = list1;
            currentNode = currentNode -> next;
            list1 = list1 -> next;
        }
        else{
            currentNode -> next = list2;
            currentNode = currentNode -> next;
            list2 = list2 -> next;
        }
    }
    //list1 == NULL and list2 == NULL, returnList is now a merged sorted list of list1 and list2!
    return returnList;
}
