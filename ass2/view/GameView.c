// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include <string.h>
#include "Map.h"

#define TURN_CHAR_LENGTH 8
#define LOCATION_CHAR_OFFSET 2
#define TRAP_CHAR_OFFSET 3
#define ROUND_CHAR_LENGTH 40
#define MAX_TRAP_LENGTH 4
#define VAMPIRE_MATURE_OFFSET 6

// ADTs
struct gameView
{
    int currScore;
    int currRound;
    int currTurn;
    int currPlayer;
    int trail[NUM_PLAYERS][TRAIL_SIZE];
    int currHealth[NUM_PLAYERS];
    Map map;
    PlayerMessage *messages;
};

typedef struct vNode *VList;

struct vNode
{
    LocationID  v;    // ALICANTE, etc
    TransportID type; // ROAD, RAIL, BOAT
    VList       next; // link to next node
};

struct MapRep
{
    int   nV;         // #vertices
    int   nE;         // #edges
    VList connections[NUM_MAP_LOCATIONS]; // array of lists
};

// Function Prototypes (MUST BE HANDLED CORRECTLY)
int calculateScore (char *pastPlays);
int calculateHunterHealth (char *pastPlays, PlayerID player);
int calculateDraculaHealth (char *pastPlays);
int calculatePlayer (char *pastPlays);
int calculateRound (char *pastPlays);
int calculatePlayer (char *pastPlays);
int calculateTurn (char *pastPlays);
void calculateTrail (char *pastPlays, PlayerID player, LocationID trail[TRAIL_SIZE]);

// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[])
{
    //Initialise GameView
    GameView gameView = malloc(sizeof(struct gameView));
    assert(gameView != NULL);

    //Add messages to struct
    gameView->messages = messages;

    //Establish struct fields
    gameView->currScore = calculateScore(pastPlays);
    gameView->currRound = calculateRound(pastPlays);
    gameView->currTurn = calculateTurn(pastPlays);
    gameView->currPlayer = calculatePlayer(pastPlays);
    gameView->currHealth[PLAYER_LORD_GODALMING] = calculateHunterHealth(pastPlays, PLAYER_LORD_GODALMING);
    gameView->currHealth[PLAYER_DR_SEWARD] = calculateHunterHealth(pastPlays, PLAYER_DR_SEWARD);
    gameView->currHealth[PLAYER_VAN_HELSING] = calculateHunterHealth(pastPlays, PLAYER_VAN_HELSING);
    gameView->currHealth[PLAYER_MINA_HARKER] = calculateHunterHealth(pastPlays, PLAYER_MINA_HARKER);
    gameView->currHealth[PLAYER_DRACULA] = calculateDraculaHealth(pastPlays);
    int trail[TRAIL_SIZE] = {};
    int i = 0;
    calculateTrail(pastPlays, PLAYER_LORD_GODALMING, trail);
    for (i = 0; i < TRAIL_SIZE; i++)
    {
        gameView->trail[PLAYER_LORD_GODALMING][i] = trail[i];
    }
    calculateTrail(pastPlays, PLAYER_DR_SEWARD, trail);
    for (i = 0; i < TRAIL_SIZE; i++)
    {
        gameView->trail[PLAYER_DR_SEWARD][i] = trail[i];
    }
    calculateTrail(pastPlays, PLAYER_VAN_HELSING, trail);
    for (i = 0; i < TRAIL_SIZE; i++)
    {
        gameView->trail[PLAYER_VAN_HELSING][i] = trail[i];
    }
    calculateTrail(pastPlays, PLAYER_MINA_HARKER, trail);
    for (i = 0; i < TRAIL_SIZE; i++)
    {
        gameView->trail[PLAYER_MINA_HARKER][i] = trail[i];
    }
    calculateTrail(pastPlays, PLAYER_DRACULA, trail);
    for (i = 0; i < TRAIL_SIZE; i++)
    {
        gameView->trail[PLAYER_DRACULA][i] = trail[i];
    }
    gameView->map = newMap();

    return gameView;
}


// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView(GameView toBeDeleted)
{
    assert(toBeDeleted != NULL);
    free( toBeDeleted );
}

//// Functions that calculate information for the gameView

// Calculate the trail for each player
void calculateTrail (char *pastPlays, PlayerID player, LocationID trail[TRAIL_SIZE])
{

    //Setup Variables
    assert(pastPlays != NULL);
    assert(trail != NULL);
    assert(player >= 0 && player <= 4);
    int i = 0;  //Counter
    int j = 0;  //Counter
    int pastPlaysLength = strlen(pastPlays);
    char playerInitial;
    char tempLocation[2] = {'0'};

    //Determine which player initial to look for
    switch (player)
    {
    case PLAYER_LORD_GODALMING : playerInitial = 'G'; break;
    case PLAYER_DR_SEWARD : playerInitial = 'S'; break;
    case PLAYER_VAN_HELSING : playerInitial = 'H'; break;
    case PLAYER_MINA_HARKER : playerInitial = 'M'; break;
    case PLAYER_DRACULA : playerInitial = 'D'; break;
    }

    //Fill the trail array with -1 to indicate location unknown initially
    for (i = 0; i < TRAIL_SIZE; i++)
    {
        trail[i] = UNKNOWN_LOCATION;
    }

    //Loop backwards through the pastPlays string to find recent turns
    for (i = pastPlaysLength; i >= 0; i--)
    {

        //Keep moving if not the start of a turn
        if (pastPlays[i] != ' ')
        {
            continue;
        }

        //Keep moving if not the player in question
        if (pastPlays[i + 1] != playerInitial)
        {
            continue;
        }

        //Should now always have the correct player, start of turn
        //Store location initials temporarily
        tempLocation[1] = pastPlays[i + 1 + LOCATION_CHAR_OFFSET];
        tempLocation[0] = pastPlays[i + 1 + LOCATION_CHAR_OFFSET - 1];


        //Input locationID into array - special handling for non-locations
        trail[j] = abbrevToID(tempLocation);

        //Special Handling for non-locations
        //#define in Places.h, lines 106-117
        if (strncmp(tempLocation, "C?", 2) == 0)
        {
            trail[j] = CITY_UNKNOWN;
        }
        else if (strncmp(tempLocation, "S?", 2) == 0)
        {
            trail[j] = SEA_UNKNOWN;
        }
        else if (strncmp(tempLocation, "HI", 2) == 0)
        {
            trail[j] = HIDE;
        }
        else if (strncmp(tempLocation, "D1", 2) == 0)
        {
            trail[j] = DOUBLE_BACK_1;
        }
        else if (strncmp(tempLocation, "D2", 2) == 0)
        {
            trail[j] = DOUBLE_BACK_2;
        }
        else if (strncmp(tempLocation, "D3", 2) == 0)
        {
            trail[j] = DOUBLE_BACK_3;
        }
        else if (strncmp(tempLocation, "D4", 2) == 0)
        {
            trail[j] = DOUBLE_BACK_4;
        }
        else if (strncmp(tempLocation, "D5", 2) == 0)
        {
            trail[j] = DOUBLE_BACK_5;
        }
        else if (strncmp(tempLocation, "TP", 2) == 0)
        {
            trail[j] = TELEPORT;
        }

        j++;
    }

    //Special case for first turn in game
    if (pastPlays[0] != playerInitial)
    {
        //Do Nothing
    }
    else
    {
        //Store location initials temporarily
        tempLocation[1] = pastPlays[LOCATION_CHAR_OFFSET];
        tempLocation[0] = pastPlays[LOCATION_CHAR_OFFSET - 1];
        //Input locationID into array
        trail[j] = abbrevToID(tempLocation);
    }

}

// Calculate the current round
int calculateRound (char *pastPlays)
{

    //Setup Variables
    assert(pastPlays != NULL);
    int currRound = 0;

    //Divide no. of turns by no. of players to find round
    currRound = calculateTurn(pastPlays) / NUM_PLAYERS;

    return currRound;
}

// Calculate the current turn
int calculateTurn (char *pastPlays)
{

    //Setup Variables
    assert(pastPlays != NULL);
    int turn = 0;
    int i = 0;  //counter
    int pastPlaysLength = strlen(pastPlays);

    //Count the no. of spaces in pastPlays to find turn
    for (i = 0; i < pastPlaysLength; i++)
    {
        if (pastPlays[i] == ' ')
        {
            turn = turn + 1;
        }
    }

    //Add one turn because of missing space at front of string
    if (i > 0)
    {
        turn = turn + 1;
    }

    return turn;
}

// Calculate the current player
int calculatePlayer (char *pastPlays)
{

    //Setup Variables
    assert(pastPlays != NULL);
    int player = 0;

    //Case if first turn
    if (calculateTurn(pastPlays) == 0)
    {
        return PLAYER_LORD_GODALMING;
    }

    //The current player is determined by looking at the turn and round
    player = calculateTurn(pastPlays) - (calculateRound(pastPlays) * NUM_PLAYERS);

    //Adjust to ensure valid players
    if (player == PLAYER_DRACULA + 1)
    {
        player = PLAYER_LORD_GODALMING;
    }

    return player;
}

// Calculate the current score
int calculateScore (char *pastPlays)
{

    //Setup Variables
    assert(pastPlays != NULL);
    int i = 0;  //Counter
    int pastPlaysLength = strlen(pastPlays);

    //The game score starts at 366 points
    int score = GAME_START_SCORE;

    //The score decreases by 1 each time Dracula finishes a turn
    for (i = 0; i < pastPlaysLength; i = i + TURN_CHAR_LENGTH)
    {
        if (pastPlays[i] == 'D')
        {
            score = score - SCORE_LOSS_DRACULA_TURN;
        }
    }

    //The score decreases by 6 each time a Hunter is teleported to the hospital
    for (i = LOCATION_CHAR_OFFSET; i < pastPlaysLength; i = i + TURN_CHAR_LENGTH)
    {
        if (pastPlays[i] == 'M' && pastPlays[i - 1] == 'J')
        {
            score = score - SCORE_LOSS_HUNTER_HOSPITAL;
        }
    }

    //The score decreases by 13 each time a Vampire matures
    for (i = 0; i < pastPlaysLength; i = i + TURN_CHAR_LENGTH)
    {
        if (pastPlays[i] == 'D' && pastPlays[i + VAMPIRE_MATURE_OFFSET] == 'V')
        {
            score = score - SCORE_LOSS_VAMPIRE_MATURES;
        }
    }

    return score;

}

// Calculate the health of all Hunters
int calculateHunterHealth (char *pastPlays, PlayerID player)
{

    //Setup Variables
    assert(pastPlays != NULL);
    int i = 0;  //Counter
    int j = 0;  //Counter
    int pastPlaysLength = strlen(pastPlays);
    char playerInitial;

    //Each Hunter starts with 9 life points
    int health = GAME_START_HUNTER_LIFE_POINTS;

    //Determine which player initial to look for
    switch (player)
    {
    case PLAYER_LORD_GODALMING : playerInitial = 'G'; break;
    case PLAYER_DR_SEWARD : playerInitial = 'S'; break;
    case PLAYER_VAN_HELSING : playerInitial = 'H'; break;
    case PLAYER_MINA_HARKER : playerInitial = 'M'; break;
    case PLAYER_DRACULA : playerInitial = 'D'; break;
    }

    //Loop through player's turns
    for (j = 0; j < pastPlaysLength; j++)
    {

        //Keep moving if not the start of a turn
        if (pastPlays[j] != ' ')
        {
            continue;
        }

        //Keep moving if not the player in question
        if (pastPlays[j + 1] != playerInitial)
        {
            continue;
        }

        //Loop through the Action phase
        for (i = TRAP_CHAR_OFFSET + 1; i < TRAP_CHAR_OFFSET + 5; i++)
        {
            //If a hunter encounters a trap they lose 2 life points
            if (pastPlays[j + i] == 'T')
            {
                health = health - LIFE_LOSS_TRAP_ENCOUNTER;
                if (health < 0)
                {
                    health = 0;
                }
                //If a hunter encounters Dracula they lost 4 life points
            }
            else if (pastPlays[j + i] == 'D')
            {
                health = health - LIFE_LOSS_DRACULA_ENCOUNTER;
                if (health < 0)
                {
                    health = 0;
                }
            }
        }

        //Abort loop now if this is the player's first turn
        if (calculateRound(pastPlays) == 0)
        {
            continue;
        }

        //If last turn was spent in the Hospital of St Joseph & St Mary, restore health
        if (pastPlays[j + 1 - ROUND_CHAR_LENGTH + LOCATION_CHAR_OFFSET] == 'M' &&
                pastPlays[j + 1 - ROUND_CHAR_LENGTH + LOCATION_CHAR_OFFSET - 1] == 'J')
        {
            health = health + LIFE_GAIN_REST;
            if (health > GAME_START_HUNTER_LIFE_POINTS)
            {
                health = GAME_START_HUNTER_LIFE_POINTS;
            }
        }

        //If last turn was spent in the same location (i.e. player resting)
        if (pastPlays[j + 1 - ROUND_CHAR_LENGTH + LOCATION_CHAR_OFFSET] == pastPlays[j + LOCATION_CHAR_OFFSET] &&
                pastPlays[j + 1 - ROUND_CHAR_LENGTH + LOCATION_CHAR_OFFSET - 1] == pastPlays[j + LOCATION_CHAR_OFFSET - 1])
        {
            health = health + LIFE_GAIN_REST;
            if (health > GAME_START_HUNTER_LIFE_POINTS)
            {
                health = GAME_START_HUNTER_LIFE_POINTS;
            }
        }

    }

    return health;

}

// Calculate Dracula's Health
int calculateDraculaHealth (char *pastPlays)
{
    //Setup Variables
    assert(pastPlays != NULL);
    int i = 0;  //Counter
    int j = 0;  //Counter
    int pastPlaysLength = strlen(pastPlays);
    char tempLocation[2] = {'0'};
    int tempLocationID = 0;
    int trail[TRAIL_SIZE] = {'0'};

    //Dracula begins with 40 "Blood Points" (health)
    int health = GAME_START_BLOOD_POINTS;

    //Loop through player's turns
    for (j = 0; j < pastPlaysLength; j++)
    {

        //Keep moving if not the start of a turn
        if (pastPlays[j] != ' ')
        {
            continue;
        }

        //Loop through the Action phase
        for (i = TRAP_CHAR_OFFSET + 1; i < TRAP_CHAR_OFFSET + 1 + MAX_TRAP_LENGTH; i++)
        {

            //If a hunter encounters Dracula he loses 10 blood points
            if (pastPlays[j + i] == 'D')
            {
                health = health - LIFE_LOSS_HUNTER_ENCOUNTER;
                if (health < 0)
                {
                    return 0;
                }
            }
        }

        //Keep moving if not the player in question
        if (pastPlays[j + 1] != 'D')
        {
            continue;
        }

        //Dracula regains 10 blood points if he is in Castle Dracula at the end of his turn
        if (pastPlays[j + 1 + LOCATION_CHAR_OFFSET] == 'D' && pastPlays[j + 1 + LOCATION_CHAR_OFFSET - 1] == 'C')
        {
            if (health < 0)
            {
                return 0;
            }
            health = health + LIFE_GAIN_CASTLE_DRACULA;
        }

        //Determine Dracula's location
        //Store location initials temporarily
        tempLocation[1] = pastPlays[j + 1 + LOCATION_CHAR_OFFSET];
        tempLocation[0] = pastPlays[j + 1 + LOCATION_CHAR_OFFSET - 1];
        //Obtain useful LocationID
        tempLocationID = abbrevToID(tempLocation);

        //Case if tempLocation is not a place
        if (strncmp(tempLocation, "C?", 2) == 0)
        {
            tempLocationID = CITY_UNKNOWN;
        }
        else if (strncmp(tempLocation, "S?", 2) == 0)
        {
            tempLocationID = SEA_UNKNOWN;
        }
        else if (strncmp(tempLocation, "HI", 2) == 0)
        {
            tempLocationID = HIDE;
        }
        else if (strncmp(tempLocation, "D1", 2) == 0)
        {
            tempLocationID = DOUBLE_BACK_1;
        }
        else if (strncmp(tempLocation, "D2", 2) == 0)
        {
            tempLocationID = DOUBLE_BACK_2;
        }
        else if (strncmp(tempLocation, "D3", 2) == 0)
        {
            tempLocationID = DOUBLE_BACK_3;
        }
        else if (strncmp(tempLocation, "D4", 2) == 0)
        {
            tempLocationID = DOUBLE_BACK_4;
        }
        else if (strncmp(tempLocation, "D5", 2) == 0)
        {
            tempLocationID = DOUBLE_BACK_5;
        }
        else if (strncmp(tempLocation, "TP", 2) == 0)
        {
            tempLocationID = TELEPORT;
        }

        //Dracula loses 2 blood points if he is at sea at the end of his turn
        if (idToType(tempLocationID) == SEA || tempLocationID == SEA_UNKNOWN)
        {
            health = health - LIFE_LOSS_SEA;
            if (health < 0)
            {
                return 0;
            }
            //Case if Dracula hid while at sea
        }
        else if (tempLocationID == HIDE && idToType(trail[1]) == SEA)
        {
            health = health - LIFE_LOSS_SEA;
            if (health < 0)
            {
                return 0;
            }
            //Case if Dracula Doubled-Back to sea
        }
        else if (tempLocationID == DOUBLE_BACK_1 && (idToType(trail[1]) == SEA || trail[1] == SEA_UNKNOWN))
        {
            health = health - LIFE_LOSS_SEA;
            if (health < 0)
            {
                return 0;
            }
        }
        else if (tempLocationID == DOUBLE_BACK_2 && (idToType(trail[2]) == SEA || trail[2] == SEA_UNKNOWN))
        {
            health = health - LIFE_LOSS_SEA;
            if (health < 0)
            {
                return 0;
            }
        }
        else if (tempLocationID == DOUBLE_BACK_3 && (idToType(trail[3]) == SEA || trail[3] == SEA_UNKNOWN))
        {
            health = health - LIFE_LOSS_SEA;
            if (health < 0)
            {
                return 0;
            }
        }
        else if (tempLocationID == DOUBLE_BACK_4 && (idToType(trail[4]) == SEA || trail[4] == SEA_UNKNOWN))
        {
            health = health - LIFE_LOSS_SEA;
            if (health < 0)
            {
                return 0;
            }
        }
        else if (tempLocationID == DOUBLE_BACK_5 && (idToType(trail[5]) == SEA || trail[5] == SEA_UNKNOWN))
        {
            health = health - LIFE_LOSS_SEA;
            if (health < 0)
            {
                return 0;
            }
        }

    }

    return health;

}

//// Functions to return simple information about the current state of the game

// Get the current round
Round getRound(GameView currentView)
{
    assert(currentView != NULL);
    return currentView->currRound;
}

// Get the id of current player - ie whose turn is it?
PlayerID getCurrentPlayer(GameView currentView)
{
    assert(currentView != NULL);
    return currentView->currPlayer;
}

// Get the current score
int getScore(GameView currentView)
{
    assert(currentView != NULL);
    return currentView->currScore;
}

// Get the current health points for a given player
int getHealth(GameView currentView, PlayerID player)
{
    assert(currentView != NULL);
    assert(player >= 0 && player <= 4);

    return currentView->currHealth[player];
}

// Get the current location id of a given player
LocationID getLocation(GameView currentView, PlayerID player)
{
    assert(currentView != NULL);
    assert(player >= 0 && player <= 4);

    return currentView->trail[player][0];
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void getHistory(GameView currentView, PlayerID player,
                LocationID trail[TRAIL_SIZE])
{
    //Setup Variables
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

// Returns an array of LocationIDs for all directly connected locations

LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea)
{
    //Verify inputs
    assert(currentView != NULL);
    assert(numLocations != NULL);
    assert(from >= 0 && from <= 70);
    assert(player >= 0 && player <= 4);
    assert(round >= 0);
    assert(road == TRUE || road == FALSE);
    assert(rail == TRUE || rail == FALSE);
    assert(sea == TRUE || sea == FALSE);

    //Setup Variables
    LocationID *connectedLocations = malloc(NUM_MAP_LOCATIONS * sizeof(LocationID));
    assert(connectedLocations != NULL);
    connectedLocations[0] = from;
    int i = 1;  //Counter
    int railModifier = (round + currentView->currScore) % 4;

    //Navigate through the map array to desired list
    VList curr = currentView->map->connections[from];

    //Loop through the entire "from" list
    while (curr != NULL)
    {

        //Add each destination to the array if it is of the correct type
        //Case of ROAD destinations
        if (curr->type == ROAD && road == TRUE)
        {
            connectedLocations[i] = curr->v;
            i++;
        }
        //Case of RAIL destinations
        else if (curr->type == RAIL && rail == TRUE && player != PLAYER_DRACULA)
        {
            if (railModifier == 1)
            {
                connectedLocations[i] = curr->v;
                i++;
            }
            else if (railModifier == 2)
            {
                //INCOMPLETE
                connectedLocations[i] = curr->v;
                i++;
            }
            else if (railModifier == 3)
            {
                //INCOMPLETE
                connectedLocations[i] = curr->v;
                i++;
            }
        }
        //CASE of BOAT destinations
        else if (curr->type == BOAT && sea == TRUE)
        {
            connectedLocations[i] = curr->v;
            i++;
        }

        //Increment
        curr = curr->next;
    }

    //Record no. of locations
    *numLocations = i;

    return connectedLocations;
}

//INCOMPLETE
/*
connectedLocations
    Case of railModifier > 1
    multiple entries
*/

