// DracView.c ... DracView ADT implementation

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "DracView.h"
#include "Map.h" //... if you decide to use the Map ADT

#define LOCATION_CHAR_OFFSET 2

struct dracView
{
    int draclocation;
    int Vamps[TRAIL_SIZE]; //Temporary Values
    int Traps[TRAIL_SIZE]; //Temporary Values
    int currScore;
    int currRound;
    int currTurn;
    int currPlayer;
    int trail[NUM_PLAYERS][TRAIL_SIZE];
    int currHealth[NUM_PLAYERS];
    GameView gameView;
    Map map;
};


// Creates a new DracView to summarise the current state of the game
DracView newDracView(char *pastPlays, PlayerMessage messages[])
{
    assert(messages != NULL);
    assert(pastPlays != NULL);

    GameView gv = newGameView(pastPlays, messages);
    DracView dracView = malloc(sizeof(struct dracView));
    int temptrail[TRAIL_SIZE];
    int i;
    int j;

    dracView->gameView = gv;
    dracView->currScore = getScore(gv);
    dracView->currRound = getRound(gv);
    dracView->currTurn = getCurrentPlayer(gv);
    for (i = 0; i < NUM_PLAYERS ; i++)
    {
        dracView->currHealth[i] = getHealth(gv, i);
    }
    for (i = 0; i < NUM_PLAYERS ; i++)
    {
        getHistory(gv, i, temptrail);
        for (j = 0 ; j < TRAIL_SIZE ; j++)
        {
            dracView->trail[i][j] = temptrail[j];
        }
    }
    dracView->draclocation = temptrail[0];
    dracView->map = newMap();

    char tempLocation[2] = {'0'};
    int tempLocationID = 0;
    int pastPlaysLength = strlen(pastPlays);
    for (i = pastPlaysLength, j = 0; i >= 0 && j < TRAIL_SIZE; i--)
    {

        //Keep moving if not the start of a turn
        if (pastPlays[i] != ' ')
        {
            continue;
        }

        //Keep moving if not the player in question
        if (pastPlays[i + 1] != 'D')
        {
            continue;
        }

        //Should now always have the correct player, start of turn
        //Store location initials temporarily
        tempLocation[1] = pastPlays[i + 1 + LOCATION_CHAR_OFFSET];
        tempLocation[0] = pastPlays[i + 1 + LOCATION_CHAR_OFFSET - 1];

        //Input locationID into array - special handling for non-locations
        tempLocationID = abbrevToID(tempLocation);

        //Check if a Vampire was placed
        if (pastPlays[i + 5] == 'V')
        {
            dracView->Vamps[j] = tempLocationID;
        }

        //Check if a Trap was placed
        if (pastPlays[i + 4] == 'T')
        {
            dracView->Traps[j] = tempLocationID;
        }

        j++;
    }

    return dracView;
}


// Frees all memory previously allocated for the DracView toBeDeleted
void disposeDracView(DracView toBeDeleted)
{
    assert(toBeDeleted != NULL);
    free( toBeDeleted );
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round giveMeTheRound(DracView currentView)
{
    assert(currentView != NULL);
    return currentView->currRound;
}

// Get the current score
int giveMeTheScore(DracView currentView)
{
    assert(currentView != NULL);
    return currentView->currScore;
}

// Get the current health points for a given player
int howHealthyIs(DracView currentView, PlayerID player)
{
    assert(currentView != NULL);
    return currentView->currHealth[player];
}

// Get the current location id of a given player
LocationID whereIs(DracView currentView, PlayerID player)
{
    assert(currentView != NULL);
    return currentView->trail[player][0];
}

// Get the most recent move of a given player
void lastMove(DracView currentView, PlayerID player,
              LocationID *start, LocationID *end)
{
    assert(currentView != NULL);
    *start = currentView->trail[player][1];
    *end = currentView->trail[player][0];
    return;
}

// Find out what minions are placed at the specified location
void whatsThere(DracView currentView, LocationID where,
                int *numTraps, int *numVamps)
{
    assert(currentView != NULL);
    int i = 0;
    *numVamps = 0;
    *numTraps = 0;
    for (i = 0 ; i < TRAIL_SIZE; i++)
    {
        if (currentView->Vamps[i] == where)
        {
            *numVamps = 1;
        }
        if (currentView->Traps[i] == where)
        {
            *numTraps = 1;
        }
    }

    LocationID trail[TRAIL_SIZE] = {};
    giveMeTheTrail(currentView, PLAYER_DRACULA, trail);
    for (i = 0; i < TRAIL_SIZE; i++)
    {

    }
    return;
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void giveMeTheTrail(DracView currentView, PlayerID player,
                    LocationID trail[TRAIL_SIZE])
{
    assert(currentView != NULL);
    assert(trail != NULL);
    assert(player >= 0 && player <= 4);
    int i = 0;  //Counter

    //Loop through currentView trail and fill given trail
    for (i = 0; i < TRAIL_SIZE; i++)
    {
        trail[i] = currentView->trail[player][i];
    }

}

//// Functions that query the map to find information about connectivity

// What are my (Dracula's) possible next moves (locations)
LocationID *whereCanIgo(DracView currentView, int *numLocations, int road, int sea)
{
    LocationID from = whereIs(currentView, PLAYER_DRACULA);
    Round round = giveMeTheRound(currentView);
    int rail = 0;
    return connectedLocations(currentView->gameView, numLocations, from, PLAYER_DRACULA, round, road, rail, sea);
}

// What are the specified player's next possible moves
LocationID *whereCanTheyGo(DracView currentView, int *numLocations,
                           PlayerID player, int road, int rail, int sea)
{
    LocationID from = whereIs(currentView, player);
    Round round = giveMeTheRound(currentView);
    return connectedLocations(currentView->gameView, numLocations, from, player, round, road, rail, sea);
}
