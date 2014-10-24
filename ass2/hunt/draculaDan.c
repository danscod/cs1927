// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "DracView.h"
#include <time.h>

LocationID randomMove(DracView gameState);

void decideDraculaMove(DracView gameState)
{
	// TODO ...
	// Replace the line below by something better
	registerBestPlay("CD","Mwuhahahaha");
    // Original Code below

    //Determine character abbreviation
    //PlayerID characterID = whoAmI(gameState);
    
    //Make stupid move
    LocationID destinationID = randomMove(gameState);

    registerBestPlay(idToAbbrev(destinationID), "This was a stupid move...");
    
}
    
LocationID randomMove(DracView gameState) 
{
    int numLocations;
    //LocationID possibleDestinations[NUM_MAP_LOCATIONS]; 
    LocationID *possibleDestinations; 
    possibleDestinations = whereCanIgo(gameState, &numLocations, 1, 1);

    srand(time(NULL));
    int randomDestination = rand() % numLocations;
    return possibleDestinations[randomDestination];
}
    