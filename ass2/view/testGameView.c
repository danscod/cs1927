// testGameView.c ... test the GameView ADT

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "GameView.h"

int main()
{
    int i;
    GameView gv;
    
    printf("Test basic empty initialisation\n");
    PlayerMessage messages1[] = {};
    gv = newGameView("", messages1);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getRound(gv) == 0);
    assert(getHealth(gv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    assert(getScore(gv) == GAME_START_SCORE);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == UNKNOWN_LOCATION);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for Dracula trail and basic functions\n");
    PlayerMessage messages2[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    gv = newGameView("GST.... SAO.... HZU.... MBB.... DC?....", messages2);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getRound(gv) == 1);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == STRASBOURG);
    assert(getLocation(gv,PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
    assert(getLocation(gv,PLAYER_VAN_HELSING) == ZURICH);
    assert(getLocation(gv,PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
    assert(getLocation(gv,PLAYER_DRACULA) == CITY_UNKNOWN);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for encountering Dracula and hunter history\n");
    PlayerMessage messages3[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!"};
    gv = newGameView("GST.... SAO.... HCD.... MAO.... DGE.... GGED...", messages3);
    assert(getLocation(gv,PLAYER_DRACULA) == GENEVA);
    assert(getHealth(gv,PLAYER_LORD_GODALMING) == 5);
    assert(getHealth(gv,PLAYER_DRACULA) == 30);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == GENEVA);
    LocationID history[TRAIL_SIZE];
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == GENEVA);
    assert(history[1] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_LORD_GODALMING,history);
    assert(history[0] == GENEVA);
    assert(history[1] == STRASBOURG);
    assert(history[2] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_DR_SEWARD,history);
    assert(history[0] == ATLANTIC_OCEAN);
    assert(history[1] == UNKNOWN_LOCATION);
    printf("passed\n");        
    disposeGameView(gv);

    printf("Test for Dracula doubling back at sea, and losing blood points (Hunter View)\n");
    PlayerMessage messages4[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DS?.... "
                     "GST.... SST.... HST.... MST.... DD1....", messages4);
    assert(getLocation(gv,PLAYER_DRACULA) == DOUBLE_BACK_1);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_1);
    assert(history[1] == SEA_UNKNOWN);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 4);
    assert(getCurrentPlayer(gv) == 0);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for Dracula doubling back at sea, and losing blood points (Drac View)\n");
    PlayerMessage messages5[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DEC.... "
                     "GST.... SST.... HST.... MST.... DD1....", messages5);
    assert(getLocation(gv,PLAYER_DRACULA) == DOUBLE_BACK_1);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_1);
    assert(history[1] == ENGLISH_CHANNEL);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 4);
    assert(getCurrentPlayer(gv) == 0);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for connections\n");
    int size, seen[NUM_MAP_LOCATIONS], *edges;
    gv = newGameView("", messages1);    
    printf("Checking Galatz road connections\n");
    edges = connectedLocations(gv,&size,GALATZ,PLAYER_LORD_GODALMING,0,1,0,0);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i< size ; i++) seen[edges[i]] = 1;
    assert(size == 5); assert(seen[GALATZ]); assert(seen[CONSTANTA]);
    assert(seen[BUCHAREST]); assert(seen[KLAUSENBURG]); assert(seen[CASTLE_DRACULA]);
    free(edges);
    printf("Checking Ionian Sea sea connections\n");
    edges = connectedLocations(gv,&size,IONIAN_SEA,PLAYER_LORD_GODALMING,0,0,0,1);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i < size; i++) seen[edges[i]] = 1;
    assert(size == 7); assert(seen[IONIAN_SEA]); assert(seen[BLACK_SEA]);
    assert(seen[ADRIATIC_SEA]); assert(seen[TYRRHENIAN_SEA]);
    assert(seen[ATHENS]); assert(seen[VALONA]); assert(seen[SALONICA]);
    free(edges);
    printf("Checking Athens rail connections (none)\n");
    edges = connectedLocations(gv,&size,ATHENS,PLAYER_LORD_GODALMING,0,0,1,0);
    assert(size == 1);
    assert(edges[0] == ATHENS);
    free(edges);
    printf("passed\n");
    disposeGameView(gv);

    //Custom Tests

    //Test for extended rail connections (Dan Scodellaro)
    printf("Test for extended rail connections\n");
    //int size, seen[NUM_MAP_LOCATIONS], *edges;
    gv = newGameView("GAL.... SLO.... HLO.... ", messages1);    
    printf("Checking London rail connections (extended)\n");
    edges = connectedLocations(gv,&size,LONDON,PLAYER_VAN_HELSING,getRound(gv),0,1,0);
    //printf("Round is %d, PlayerID is %d, railModifier is %d\n", getRound(gv), PLAYER_VAN_HELSING, (getRound(gv) + PLAYER_VAN_HELSING) % 4);
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
    disposeGameView(gv);

    //Test for duplicates in connectedLocations (Dan Scodellaro)
    printf("Test for duplicated connections\n");
    //int size, seen[NUM_MAP_LOCATIONS], *edges;
    gv = newGameView("GAL.... SLO.... HLO.... ", messages1);    
    printf("Checking London rail connections (extended)\n");
    edges = connectedLocations(gv,&size,LONDON,PLAYER_VAN_HELSING,getRound(gv),1,1,1);
    //printf("Round is %d, PlayerID is %d, railModifier is %d\n", getRound(gv), PLAYER_VAN_HELSING, (getRound(gv) + PLAYER_VAN_HELSING) % 4);
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
    disposeGameView(gv);

    // ADDED TESTS ---------------- (Christine)


    //test for traps 
    printf("Test for Dracula Placing Trap and Hunter Encountering Trap\n");
    PlayerMessage messages6[] = {
    "This","Is","So","Annoying","To",
    "Make","Random","Player","Messages","MUAHAH",
    "Last", "Line", "Thank", "Goodness","DONE"};
    gv = newGameView("GBD.... SBD.... HBD.... MBD.... DPRT... "
                     "GVI.... SVI.... HSZ.... MSZ.... DBRT... "
                     "GPRT... SPR.... HBD.... MBD.... DNU....", messages6);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS - 2);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    assert(getScore(gv) == GAME_START_SCORE - 3);
    printf("passed\n");
    disposeGameView(gv);


//  //test for immature vampire killed
    printf("Test for Immature Vampire Killed\n");
    PlayerMessage messages7[] = {
    "This","Is","So","Annoying","To",
    "Make","Random","Player","Messages","MUAHAH",
    "Last", "Line", "Thank", "Goodness","DONE"};
    gv = newGameView("GBD.... SBD.... HBD.... MBD.... DPR.V.. "
                     "GVI.... SVI.... HSZ.... MSZ.... DBR.... "
                     "GPRV...", messages7);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    assert(getScore(gv) == GAME_START_SCORE - 2);
    printf("passed\n");
    disposeGameView(gv);

//  //test for vampire matured
    printf("Test for Vampire Maturing\n");
    PlayerMessage messages8[19*5]={""};
    gv = newGameView(
        "GBD.... SBD.... HBD.... MBD.... DPR.... "
        "GVI.... SVI.... HSZ.... MSZ.... DBR.... "
        "GBD.... SBD.... HBD.... MBD.... DPR.... "
        "GVI.... SVI.... HSZ.... MSZ.... DBR.... "
        "GBD.... SBD.... HBD.... MBD.... DPR.... "
        "GVI.... SVI.... HSZ.... MSZ.... DBR.... "
        "GBD.... SBD.... HBD.... MBD.... DPR.... "
        "GVI.... SVI.... HSZ.... MSZ.... DBR.... "
        "GBD.... SBD.... HBD.... MBD.... DPR.... "
        "GVI.... SVI.... HSZ.... MSZ.... DBR.... "
        "GBD.... SBD.... HBD.... MBD.... DPR.... "
        "GVI.... SVI.... HSZ.... MSZ.... DBR.... "
        "GBD.... SBD.... HBD.... MBD.... DPR.V.. " //round 13 vampire put
        "GVI.... SVI.... HSZ.... MSZ.... DBR.... "
        "GBD.... SBD.... HBD.... MBD.... DPR.... "
        "GVI.... SVI.... HSZ.... MSZ.... DBR.... "
        "GBD.... SBD.... HBD.... MBD.... DPR.... "
        "GVI.... SVI.... HSZ.... MSZ.... DBR.... "
        "GBD.... SBD.... HBD.... MBD.... DPR..V.", messages8); //round 19 
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    assert(getScore(gv) == GAME_START_SCORE - 19 - 13);
    printf("passed\n");
    disposeGameView(gv);

//  //test for hunter recovering health in city
    printf("Test for Hunter Recovering Health in city\n");
    PlayerMessage messages9[] = {
    "This","Is","So","Annoying","To",
    "Make","Random","Player","Messages","MUAHAH",
    "Last", "Line", "Thank", "Goodness","DONE",
    "one","more","line","till","REVOLUTIONWEWILLNIPITINTHEBUD",
    "jks","another","line","till","REVOLUTIONWEWILLNIPITINTHEBUD"};
    gv = newGameView("GBD.... SBD.... HBD.... MBD.... DPRT... "
                     "GVI.... SVI.... HVI.... MSZ.... DBRT... "
                     "GBD.... SPR.... HPR.... MBD.... DNU.... " // -2  health
                     "GSZ.... SBR.... HBR.... MBD.... DMU.... " // -2  health
                     "GBD.... SBD.... HNU.... MBD.... DZA...." // +3 health
                     , messages9);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS -1);
    assert(getHealth(gv, PLAYER_MINA_HARKER) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    assert(getScore(gv) == GAME_START_SCORE - 5);
    printf("passed\n");
    disposeGameView(gv);    

//  //test for hunter dying
    printf("Test for Hunter dying\n");
    PlayerMessage messages10[] = {
    "This","Is","So","Annoying","To",
    "Make","Random","Player","Messages","MUAHAH",
    "Last", "Line", "Thank", "Goodness","DONE",
    "one","more","line"};
    gv = newGameView("GBD.... SBD.... HBD.... MBD.... DPRT... "
                     "GVI.... SVI.... HVI.... MSZ.... DD1T... "
                     "GBD.... SBD.... HPR.... MBD.... DBET... " 
                     "GVI.... SVI.... HBE...." 
                     , messages10);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == 0);
    assert(getScore(gv) == GAME_START_SCORE -3-6);
    printf("passed\n");
    disposeGameView(gv); 

//  //test for hunter dying, transported to hospital, recovered
    printf("Test for Hunter dying, transported to hospital, recovering\n");
    PlayerMessage messages11[] = {
    "This","Is","So","Annoying","To",
    "Make","Random","Player","Messages","MUAHAH",
    "Last", "Line", "Thank", "Goodness","DONE",
    "one","more","line","minoy","draculo",
    "whensuddenly","VOILA","RESURRECTION"};
    gv = newGameView("GBD.... SBD.... HBD.... MBD.... DPRT... "
                     "GVI.... SVI.... HVI.... MSZ.... DD1T... "
                     "GBD.... SBD.... HPR.... MBD.... DBET... " 
                     "GVI.... SVI.... HBE.... MSZ.... DPR.... " 
                     "GBD.... SBD.... HPI...."
                     , messages11);
    assert(getHealth(gv, PLAYER_VAN_HELSING) == 9);
    assert(getLocation(gv, PLAYER_VAN_HELSING) == 60);
    printf("passed\n");
    disposeGameView(gv); 


//----------------------------

    return 0;
}

