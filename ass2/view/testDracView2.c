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



//NEW SET

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



//NEW SET

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



//NEW SET

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



//NEW SET

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



//NEW SET

    printf("test for mature vampire (drac view)\n");

    PlayerMessage messages8[] = {"im sorry i'm lazy", "copy paste is a great thing"};

    dv= newDracView("GST.... SAO.... HCD.... MBO.... DLET... " //0

                    "GST.... SAO.... HCD.... MNA.... DPAT... " //1

                    "GST.... SAO.... HCD.... MCF.... DSTT... " //2

                    "GSTT... SAO.... HCD.... MGE.... DZUT... " //3 -2 LG

                    "GST.... SAO.... HCD.... MZUTD.. DCDT... " //4 trap at castle dracula??? -10hp drac, -6hp mina harker, +2LG, + 10hp drac

                    "GST.... SAO.... HCDTD.. MZU.... DGAT... " //5 + 3hp mina harker -6hp van helsing, -10 hp drac

                    "GST.... SAO.... HGATD.. MZU.... DCNT.M. " //6 -6hp vh (van helsing dies), -10hp drac, +3 mina marker

                    "GST.... SAO.... HSZ.... MMU.... DBS..M. " //7 -2hp dracula, helsing been in hospital

                    "GST.... SAO.... HKL.... MZA.... DCDT.M. " //8 +10hp drac

                    "GST.... SAO.... HKL.... MZA.... DCDT.M. " //9 +10hp drac

                    "GST.... SAO.... HCDTTD. MZA.... DCDT.M. " //10 -8hp vn, drac is constant

                    "GST.... SAO.... HGA.... MZA.... DKLT.M. " //11 +3hp vh

                    "GST.... SAO.... HGA.... MZA.... DBE.VM. " //12 +3hp vh

                    "GST.... SAO.... HGA.... MZA.... DSOT... " //13 +2hp vh

                    "GST.... SAO.... HCDTTT. MZA.... DSJT.M. " //14 -6hp vh

                    "GST.... SAO.... HCD.... MZA.... DVET.M. " //15 +3hp vh

                    "GST.... SAO.... HCD.... MZA.... DZAT.M. " //16 +3hp vh

                    "GMU.... SAO.... HCD.... MZA.... DVIT.M. " //17 

                    "GVITD.. SAO.... HCD.... MZA.... DPRT... " //18 -6LG, -10 drac

                    "GVI.... SAO.... HCD.... MZA.... DCDT.V.", messages8); //19 +3 LG

    //basically copied from testGameView...

    //making sure the basics of drac view works

    assert(giveMeTheRound(dv)==20);

    assert(giveMeTheScore(dv)==GAME_START_SCORE -20 -6 -13);

    assert(howHealthyIs(dv, PLAYER_LORD_GODALMING)==GAME_START_HUNTER_LIFE_POINTS-3);

    assert(howHealthyIs(dv, PLAYER_DR_SEWARD)==GAME_START_HUNTER_LIFE_POINTS);

    assert(howHealthyIs(dv, PLAYER_VAN_HELSING)==GAME_START_HUNTER_LIFE_POINTS);

    assert(howHealthyIs(dv, PLAYER_MINA_HARKER)==GAME_START_HUNTER_LIFE_POINTS);

    assert(whereIs(dv, PLAYER_LORD_GODALMING)==VIENNA);

    assert(whereIs(dv, PLAYER_DR_SEWARD)==ATLANTIC_OCEAN);

    assert(whereIs(dv, PLAYER_VAN_HELSING)==CASTLE_DRACULA);

    assert(whereIs(dv, PLAYER_MINA_HARKER)==ZAGREB);

    

    giveMeTheTrail(dv,PLAYER_DRACULA,history);

    assert(history[0] == CASTLE_DRACULA);

    assert(history[1] == PRAGUE); 

    assert(history[2] == VIENNA);  

    assert(history[3] == ZAGREB);   

    assert(history[4] == VENICE);  

    assert(history[5] == SARAJEVO); 

    giveMeTheTrail(dv,PLAYER_LORD_GODALMING,history);

    assert(history[0] == VIENNA);

    assert(history[1] == VIENNA); 

    assert(history[2] == MUNICH);  

    assert(history[3] == STRASBOURG);

    giveMeTheTrail(dv,PLAYER_DR_SEWARD,history);

    assert(history[0] == ATLANTIC_OCEAN);

    assert(history[1] == ATLANTIC_OCEAN);

    assert(history[2] == ATLANTIC_OCEAN);

    assert(history[3] == ATLANTIC_OCEAN);

    assert(history[4] == ATLANTIC_OCEAN);

    assert(history[5] == ATLANTIC_OCEAN);



    //start and end are the same for all

    int start, end;

    lastMove(dv, PLAYER_LORD_GODALMING, &start, &end);

    assert(start==VIENNA && end==VIENNA); 

    lastMove(dv, PLAYER_DR_SEWARD, &start, &end);

    assert(start==ATLANTIC_OCEAN && end==ATLANTIC_OCEAN); 

    lastMove(dv, PLAYER_VAN_HELSING, &start, &end);

    assert(start==CASTLE_DRACULA && end==CASTLE_DRACULA); 

    lastMove(dv, PLAYER_MINA_HARKER, &start, &end);

    assert(start==ZAGREB && end==ZAGREB); 

    disposeDracView(dv);

    printf("passed\n");



//NEW SET

    printf("test more many traps\n"); 

    PlayerMessage messages9[] = {"hey look!, something new..."};

    dv= newDracView("GST.... SAO.... HCD.... MBO.... DLET... "

                    "GCO.... SIR.... HGA.... MNA.... DD1T...",messages9 );

    int nnT, nnV;

    whatsThere(dv,LE_HAVRE,&nnT,&nnV);

    assert(nnT == 2 && nnV == 0);

    assert(giveMeTheRound(dv)==2);

    assert(giveMeTheScore(dv)==GAME_START_SCORE -2);

    

    lastMove(dv, PLAYER_LORD_GODALMING, &start, &end);

    assert(start==STRASBOURG && end==COLOGNE); 

    lastMove(dv, PLAYER_DR_SEWARD, &start, &end);

    assert(start==ATLANTIC_OCEAN && end==IRISH_SEA); 

    lastMove(dv, PLAYER_VAN_HELSING, &start, &end);

    assert(start==CASTLE_DRACULA && end==GALATZ); 

    lastMove(dv, PLAYER_MINA_HARKER, &start, &end);

    assert(start==BORDEAUX && end==NANTES);



    //more basics

    giveMeTheTrail(dv,PLAYER_LORD_GODALMING,history);

    assert(history[0] == COLOGNE);

    assert(history[1] == STRASBOURG); 

    assert(history[2] == UNKNOWN_LOCATION); 

    giveMeTheTrail(dv,PLAYER_DR_SEWARD,history);

    assert(history[0] == IRISH_SEA);

    assert(history[1] == ATLANTIC_OCEAN); 

    assert(history[2] == UNKNOWN_LOCATION); 

    giveMeTheTrail(dv,PLAYER_VAN_HELSING,history);

    assert(history[0] == GALATZ);

    assert(history[1] == CASTLE_DRACULA); 

    assert(history[2] == UNKNOWN_LOCATION); 

    giveMeTheTrail(dv,PLAYER_MINA_HARKER,history);

    assert(history[0] == NANTES);

    assert(history[1] == BORDEAUX); 

    assert(history[2] == UNKNOWN_LOCATION);    

    assert(history[3] == UNKNOWN_LOCATION);

    assert(history[4] == UNKNOWN_LOCATION);

    assert(history[5] == UNKNOWN_LOCATION);             

    disposeDracView(dv);

    printf("passed\n"); 



//NEW SET

    printf("test for dracula paths (where can i go)\n");

    //int size, seen[NUM_MAP_LOCATIONS], *edges; -->declared at top    

    PlayerMessage messages10[] = {"going from le havre"};

    dv= newDracView("GST.... SAO.... HCD.... MBO.... DLET...", messages10);

    printf("Checking Le Havre Road connections\n");

    edges = whereCanIgo(dv, &size, 1,0);

    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));

    for (i = 0; i< size ; i++) seen[edges[i]] = 1;

    assert(size==4);

    assert(seen[LE_HAVRE]); assert(seen[NANTES]); assert(seen[PARIS]); 

    assert(seen[BRUSSELS]);

    free(edges);

    printf("Checking Le Havre boat connections\n");

    edges = whereCanIgo(dv, &size, 0,1);

    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));

    for (i = 0; i< size ; i++) seen[edges[i]] = 1;    

    assert(size==2); //only one boat

    assert(seen[LE_HAVRE]); assert(seen[ENGLISH_CHANNEL]);

    free(edges);

    disposeDracView(dv);



    PlayerMessage messages11[] = {"going from bucharest"};

    dv= newDracView("GST.... SAO.... HCD.... MBO.... DBCT...", messages11);

    printf("Checking bucharest road connections\n");

    edges = whereCanIgo(dv, &size, 1,0);

    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));

    for (i = 0; i< size ; i++) seen[edges[i]] = 1;

    assert(size==6); 

    assert(seen[BUCHAREST]); assert(seen[BELGRADE]); assert(seen[CONSTANTA]); 

    assert(seen[GALATZ]); assert(seen[KLAUSENBURG]); assert(seen[SOFIA]);

    free(edges);

    disposeDracView(dv);



    PlayerMessage messages12[] = {"going from galway"};

    dv= newDracView("GST.... SAO.... HCD.... MBO.... DGWT...", messages12);

    printf("Checking galway road AND boat connections\n");

    edges = whereCanIgo(dv, &size, 1,1);

    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));

    for (i = 0; i< size ; i++) seen[edges[i]] = 1;

    assert(size==3); 

    assert(seen[GALWAY]); assert(seen[DUBLIN]); assert(seen[ATLANTIC_OCEAN]);

    free(edges);

    disposeDracView(dv);

    

    PlayerMessage messages13[] = {"going from cologne"};

    dv= newDracView("GST.... SAO.... HCD.... MBO.... DCOT...", messages13);

    printf("Checking cologne boat connections (none)\n");

    edges = whereCanIgo(dv, &size, 0,1);

    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));

    for (i = 0; i< size ; i++) seen[edges[i]] = 1;

    assert(size==1); assert(seen[COLOGNE]);

    free(edges);

    disposeDracView(dv);

    printf("passed\n");



//NEW SET

    printf("test for double back in train when seeing where dracula goes \n");

    PlayerMessage messages14[] = {"going from cologne","to cologne lol"};

    dv= newDracView("GST.... SAO.... HCD.... MBO.... DCOT... " 

                    "GST.... SAO.... HCD.... MBO.... DD1T...", messages14);

    edges = whereCanIgo(dv, &size, 0,1);

    //cannot have another double back so cannot stay in cologne via boat       

    assert(size==1); //cant hide at sea

    free(edges);

      

    edges = whereCanIgo(dv, &size, 1,0);

    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));

    for (i = 0; i< size ; i++) seen[edges[i]] = 1;

    assert(size==7);

    assert(seen[COLOGNE]); //can hide in city again

    assert(seen[AMSTERDAM]); assert(seen[BRUSSELS]); assert(seen[FRANKFURT]);

    assert(seen[HAMBURG]); assert(seen[LEIPZIG]); assert(seen[STRASBOURG]);

    free(edges);

    disposeDracView(dv);    

    printf("passed\n");



//NEW SET

    printf("check for hide AND double back\n");

    dv= newDracView("GST.... SAO.... HCD.... MBO.... DCOT... " 

                    "GST.... SAO.... HCD.... MBO.... DD1T... "

                    "GST.... SAO.... HCD.... MBO.... DHI....", messages14);

    edges = whereCanIgo(dv, &size, 0,1);

    //cannot have another double back so cannot stay in cologne via boat       

    assert(size==0); //cant hide at sea

    free(edges);

      

    edges = whereCanIgo(dv, &size, 1,0);

    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));

    for (i = 0; i< size ; i++) seen[edges[i]] = 1;

    assert(size==6);

    assert(seen[AMSTERDAM]); assert(seen[BRUSSELS]); assert(seen[FRANKFURT]);

    assert(seen[HAMBURG]); assert(seen[LEIPZIG]); assert(seen[STRASBOURG]);

    free(edges);

    disposeDracView(dv);    

    printf("passed\n");

    

    return 0;

}



