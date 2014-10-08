// testDracView.c ... test the DracView ADT

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "DracView.h"

int main()
{
    int i;
    DracView dv;


    printf("Test for basic functions, just before Dracula's first move\n");
    PlayerMessage messages1[] = {"Hello","Rubbish","Stuff",""};
    dv = newDracView("GST.... SAO.... HZU.... MBB....", messages1);
    assert(giveMeTheRound(dv) == 0);
    assert(whereIs(dv,PLAYER_LORD_GODALMING) == STRASBOURG);
    assert(whereIs(dv,PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
    assert(whereIs(dv,PLAYER_VAN_HELSING) == ZURICH);
    assert(whereIs(dv,PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
    assert(whereIs(dv,PLAYER_DRACULA) == UNKNOWN_LOCATION);
    assert(howHealthyIs(dv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    printf("passed\n");
    disposeDracView(dv);

    printf("Test for encountering Dracula and hunter history\n");
    PlayerMessage messages2[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","",""};
    dv = newDracView("GST.... SAO.... HCD.... MAO.... DGE.... "
                     "GGED... SAO.... HCD.... MAO....", messages2);
    assert(giveMeTheRound(dv) == 1);
    assert(whereIs(dv,PLAYER_DRACULA) == GENEVA);
    assert(howHealthyIs(dv,PLAYER_LORD_GODALMING) == 5);
    assert(howHealthyIs(dv,PLAYER_DRACULA) == 30);
    assert(whereIs(dv,PLAYER_LORD_GODALMING) == GENEVA);
    LocationID history[TRAIL_SIZE];
    giveMeTheTrail(dv,PLAYER_DRACULA,history);
    assert(history[0] == GENEVA);
    assert(history[2] == UNKNOWN_LOCATION);
    giveMeTheTrail(dv,PLAYER_LORD_GODALMING,history);
    assert(history[0] == GENEVA);
    assert(history[1] == STRASBOURG);
    assert(history[2] == UNKNOWN_LOCATION);
    giveMeTheTrail(dv,PLAYER_DR_SEWARD,history);
    assert(history[0] == ATLANTIC_OCEAN);
    assert(history[1] == ATLANTIC_OCEAN);
    assert(history[2] == UNKNOWN_LOCATION);
    printf("passed\n");        
    disposeDracView(dv);

    printf("Test for Dracula leaving minions\n");
    PlayerMessage messages3[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Drop a V","Party in Strasbourg","Party","Party","Party"};
    dv = newDracView("GGE.... SGE.... HGE.... MGE.... DED.V.. "
                     "GST.... SST.... HST.... MST.... DMNT... "
                     "GST.... SST.... HST.... MST....", messages3);
    int nT, nV;
    whatsThere(dv,EDINBURGH,&nT,&nV);
    assert(nT == 0 && nV == 1);
    whatsThere(dv,MANCHESTER,&nT,&nV);
    assert(nT == 1 && nV == 0);
    assert(whereIs(dv,PLAYER_DRACULA) == MANCHESTER);
    giveMeTheTrail(dv,PLAYER_DRACULA,history);
    assert(history[0] == MANCHESTER);
    assert(history[1] == EDINBURGH);
    assert(history[2] == UNKNOWN_LOCATION);
    giveMeTheTrail(dv,PLAYER_MINA_HARKER,history);
    assert(history[0] == STRASBOURG);
    assert(history[1] == STRASBOURG);
    assert(history[2] == GENEVA);
    assert(history[3] == UNKNOWN_LOCATION);
    printf("passed\n");
    disposeDracView(dv);

    printf("Test for connections\n");
    int size, seen[NUM_MAP_LOCATIONS], *edges;

    printf("Checking Galatz road connections\n");
    PlayerMessage messages5[] = {"Gone to Galatz"};
    dv = newDracView("GGA....", messages5);
    edges = whereCanTheyGo(dv,&size,PLAYER_LORD_GODALMING,1,0,0);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i< size ; i++) seen[edges[i]] = 1;
    assert(size == 5); assert(seen[GALATZ]); assert(seen[CONSTANTA]);
    assert(seen[BUCHAREST]); assert(seen[KLAUSENBURG]); assert(seen[CASTLE_DRACULA]);
    free(edges);
    disposeDracView(dv);

    printf("Checking Ionian Sea sea connections\n");
    PlayerMessage messages6[] = {"Sailing the Ionian"};
    dv = newDracView("GIO....", messages6);
    edges = whereCanTheyGo(dv,&size,PLAYER_LORD_GODALMING,0,0,1);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i < size; i++) seen[edges[i]] = 1;
    assert(size == 7); assert(seen[IONIAN_SEA]); assert(seen[BLACK_SEA]);
    assert(seen[ADRIATIC_SEA]); assert(seen[TYRRHENIAN_SEA]);
    assert(seen[ATHENS]); assert(seen[VALONA]); assert(seen[SALONICA]);
    free(edges);
    disposeDracView(dv);

    printf("Checking Athens rail connections (none)\n");
    PlayerMessage messages7[] = {"Leaving Athens by train"};
    dv = newDracView("GAT....", messages7);
    edges = whereCanTheyGo(dv,&size,PLAYER_LORD_GODALMING,0,1,0);
    assert(size == 1);
    assert(edges[0] == ATHENS);
    free(edges);
    disposeDracView(dv);

    printf("passed\n");

    //Custom Tests
    
    //Test for extended rail connections (Dan Scodellaro)
    printf("Test for extended rail connections\n");
    //int size, seen[NUM_MAP_LOCATIONS], *edges;
    dv = newDracView("GAL.... SLO.... HLO.... ", messages1);    
    printf("Checking London rail connections (extended)\n");
    edges = whereCanTheyGo(dv,&size,PLAYER_VAN_HELSING,0,1,0);
    for (i = 0; i < size; i++) seen[edges[i]] = 1;
    int flag = 0;
    //Testing
    if (size != 5) {
        printf("The test 'assert(size == 5)' has failed (Size is: %d)\n", size);
        flag = 1;
    }
    if (seen[LONDON] == 0) {
        printf("The test 'assert(seen[LONDON])' has failed\n");
        flag = 1;
    }
    if (seen[SWANSEA] == 0) {
        printf("The test 'assert(seen[SWANSEA])' has failed\n");
        flag = 1;
    }
    if (seen[MANCHESTER] == 0) {
        printf("The test 'assert(seen[MANCHESTER])' has failed\n");
        flag = 1;
    }
    if (seen[LIVERPOOL] == 0) {
        printf("The test 'assert(seen[LIVERPOOL])' has failed\n");
        flag = 1;
    }
    if (seen[EDINBURGH] == 0) {
        printf("The test 'assert(seen[EDINBURGH])' has failed\n");
        flag = 1;
    } 
    //New Content
    free(edges);
    if (flag == 0) {    
        printf("passed\n");
    } else {
        printf("failed\n");
    }
    disposeDracView(dv);

    //Test for duplicates in connectedLocations (Dan Scodellaro)
    printf("Test for duplicated connections\n");
    //int size, seen[NUM_MAP_LOCATIONS], *edges;
    dv = newDracView("GAL.... SLO.... HLO.... ", messages1);    
    printf("Checking London connections (extended)\n");
    edges = whereCanTheyGo(dv,&size,PLAYER_VAN_HELSING,1,1,1);
    for (i = 0; i < size; i++) seen[edges[i]] = 1;
    flag = 0;
    //Testing
    if (size != 7) {
        printf("The test 'assert(size == 7)' has failed (Size is: %d)\n", size);
        flag = 1;
    }
    if (seen[LONDON] == 0) {
        printf("The test 'assert(seen[LONDON])' has failed\n");
        flag = 1;
    }
    if (seen[SWANSEA] == 0) {
        printf("The test 'assert(seen[SWANSEA])' has failed\n");
        flag = 1;
    }
    if (seen[MANCHESTER] == 0) {
        printf("The test 'assert(seen[MANCHESTER])' has failed\n");
        flag = 1;
    }
    if (seen[LIVERPOOL] == 0) {
        printf("The test 'assert(seen[LIVERPOOL])' has failed\n");
        flag = 1;
    }
    if (seen[EDINBURGH] == 0) {
        printf("The test 'assert(seen[EDINBURGH])' has failed\n");
        flag = 1;
    } 
    if (seen[ENGLISH_CHANNEL] == 0) {
        printf("The test 'assert(seen[ENGLISH_CHANNEL])' has failed\n");
        flag = 1;
    }
    if (seen[PLYMOUTH] == 0) {
        printf("The test 'assert(seen[PLYMOUTH])' has failed\n");
        flag = 1;
    }
    //New Content
    free(edges);
    if (flag == 0) {    
        printf("passed\n");
    } else {
        printf("failed\n");
        printf("Take a look to see if there are any duplicates (duplicates should not exist)\n");
        for (i = 0; i < size; i++) {
            printf("edges[%d] evaluates to %d\n", i, edges[i]);
        }
    }
    disposeDracView(dv);

    return 0;
}

