// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "HunterView.h"
#include <time.h>

LocationID randomMove(HunterView gameState, *LocationID possibleDestinations, int numLocations);

void decideHunterMove(HunterView gameState)
{
    // TODO ...
    // Replace the line below by something better
    //registerBestPlay("GE","I'm on holiday in Geneva");

    // Original Code below

    //Determine character abbreviation
    //PlayerID characterID = whoAmI(gameState);

    //Determine all possible moves
    int numLocations;
    LocationID *possibleDestinations; 
    possibleDestinations = whereCanIgo(gameState, &numLocations, 1, 1, 1);

    //Make stupid move
    randomMove(gameState, possibleDestinations, numLocations);

    //Make a single-step move



    
}
    
LocationID randomMove(HunterView gameState, *LocationID possibleDestinations, int numLocations) 
{

    srand(time(NULL));
    int randomDestination = rand() % numLocations;

    //Register Play
    registerBestPlay(idToAbbrev(possibleDestinations[randomDestination]), "Random Move...");

    return possibleDestinations[randomDestination];
}


//Make a move if Dracula's current location is within the Hunter's possible moves    
LocationID singleMove(HunterView gameState)
{
    //Determine where Dracula is
    LocationID draculaLocation;
    draculaLocation = whereIs(gameState, PLAYER_DRACULA);

    int i;
    for (i = 0; i < numLocations; i++) {
        if (draculaLocation = possibleDestinations[i]) {
            registerBestPlay(idToAbbrev(possibleDestinations[i]), "Single Move...");
            return possibleDestinations[i];
        }
    }

    return UNKNOWN_LOCATION;
}