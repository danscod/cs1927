// testList.c - testing List data type
// Written by John Shepherd, March 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "List.h"

int main(int argc, char *argv[])
{
	List myList;
    myList = getList(stdin);
	showList(stdout,myList);
	assert(validList(myList));
	

    //void ListBefore(List L, Item it)
    printf("Checking ListBefore\n");

    //Case if list is empty
    printf("Case if list is empty\n");
    List nullList = newList();      
    showList(stdout, nullList);
    ListBefore(nullList, "If Empty Test1");
    showList(stdout, nullList);
    assert(validList(nullList));

    //Case if current points to first  
    printf("Case if current points to first\n");      
    showList(stdout, myList);
    ListMoveTo(myList, 1);
    ListBefore(myList, "If First Test1");
    showList(stdout, myList);
    assert(validList(myList));  //Noted first failure
    ListDelete(myList);

    //Case if current points to last (shouldn't matter for ListBefore)
    printf("Case if current points to last\n");     
    showList(stdout, myList);
    ListMoveTo(myList, (ListLength(myList)));
    ListBefore(myList, "If Elsewhere Test1");
    showList(stdout, myList);
    assert(validList(myList)); 
    ListDelete(myList);

    //Case if current points elsewhere
    printf("Case if current points elsewhere\n");     
    showList(stdout, myList);
    ListMoveTo(myList, (ListLength(myList)/2));
    ListBefore(myList, "If Elsewhere Test1");
    showList(stdout, myList);
    assert(validList(myList));    
    ListDelete(myList);

    //void ListAfter(List L, Item it)
    printf("Checking ListAfter\n");
    
    //Case if list is empty
    printf("Case if list is empty\n");
    List nullList2 = newList();      
    showList(stdout, nullList2);
    ListAfter(nullList2, "If Empty Test2");
    showList(stdout, nullList2);
    assert(validList(nullList2));

    //Case if current points to first (shouldn't matter for ListAfter)
    printf("Case if current points to first\n");    
    showList(stdout, myList);
    ListMoveTo(myList, 1);
    ListAfter(myList, "If First Test2");
    showList(stdout, myList);
    assert(validList(myList));
    ListDelete(myList);

    //Case if current points to last 
    printf("Case if current points to last\n"); 
    showList(stdout, myList);
    ListMoveTo(myList, (ListLength(myList)));
    ListAfter(myList, "If Elsewhere Test2");
    showList(stdout, myList);
    assert(validList(myList)); 
    ListDelete(myList);    

    //Case if current points elsewhere
    printf("Case if current points elsewhere\n");  
    showList(stdout, myList);
    ListMoveTo(myList, (ListLength(myList)/2));
    ListAfter(myList, "If Elsewhere Test2");
    showList(stdout, myList);
    assert(validList(myList));    
    ListDelete(myList);

    //ListDelete(List L)
    printf("Checking ListDelete\n");

    //Case if list is empty
    printf("Case if list is empty\n");
    List nullList3 = newList();      
    showList(stdout, nullList3);
    ListDelete(nullList3);
    showList(stdout, nullList3);
    assert(validList(nullList3));

    //Case if current points to first 
    printf("Case if current points to first\n");
    showList(stdout, myList);
    ListMoveTo(myList, 1);
    ListDelete(myList);
    showList(stdout, myList);
    assert(validList(myList));

    //Case if current points to last 
    printf("Case if current points to last\n"); 
    showList(stdout, myList);
    ListMoveTo(myList, (ListLength(myList)));
    ListDelete(myList);
    showList(stdout, myList);
    assert(validList(myList)); 

    //Case if current points elsewhere
    printf("Case if current points elsewhere\n"); 
    showList(stdout, myList);
    ListMoveTo(myList, (ListLength(myList)/2));
    ListDelete(myList);
    showList(stdout, myList);
    assert(validList(myList)); 

    //Case if only one node in list
    printf("Case if only one node in list\n"); 
    List oneNodeDelete = newList();
    ListAfter(oneNodeDelete, "test");
    showList(stdout, oneNodeDelete);
    ListDelete(oneNodeDelete);
    showList(stdout, oneNodeDelete);
    assert(validList(oneNodeDelete));     

    //If all tests pass
    printf("VICTORY IS SECURED\n");

    return 0;
}

