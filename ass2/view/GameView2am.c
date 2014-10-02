// GameView.c ... GameView ADT implementation

#include <stdlib.h>
#include <assert.h>
#include "Globals.h"
#include "Game.h"
#include "GameView.h"
#include "Map.h"
#include <string.h>

#define MOVE_LENGTH 7 
#define D_TRAP 3 //used only for dracula
#define D_VAMP 4 //used only for dracula
#define D_ACTION 5 //used only for dracula
#define H_ACTION 3 //used only for hunter


     
struct gameView {
    int currScore;
    int currRound;
    int currPlayer;
    int **trails; //NUM_PLAYERS of TRACK_SIZE sized arrays
    int currHealth[NUM_PLAYERS]; //initialised and set in newGame
    PlayerMessage messages; 
};
     

// Creates a new GameView to summarise the current state of the game
GameView newGameView(char *pastPlays, PlayerMessage messages[]){
    
    //init basic game struct
    GameView g = malloc(sizeof(struct gameView));

    g->currScore = GAME_START_SCORE;
    g->currPlayer = 0;
    g->currRound = 0;
//    strcpy(g->messages, messages);
    int i;
    for (i=0; i<NUM_PLAYERS-2; i++){
        g->currHealth[i] = GAME_START_HUNTER_LIFE_POINTS; //hunter health
    }
    g->currHealth[++i] = GAME_START_BLOOD_POINTS; //dracula health

    int rounds = 0;
    int score = GAME_START_SCORE;
    
    //set up trails array
    int **trails = malloc(NUM_PLAYERS);
    assert(trails != NULL);
    for(i=0; i<NUM_PLAYERS; i++){
        trails[i] = malloc(TRAIL_SIZE);
        assert(trails[i] != NULL);
    }
    int j;
    for(i=0;i<NUM_PLAYERS; i++){
        for(j=0;j<TRAIL_SIZE;j++){
            trails[i][j] = UNKNOWN_LOCATION;
        }
    }

    //read data from pastPlays to find current state
    if(strlen(pastPlays) > 0){
        assert((strlen(pastPlays)+1) % (MOVE_LENGTH+1) == 0);

        //note there is no space char after the final move
        int numMoves = (strlen(pastPlays)+1)/(MOVE_LENGTH+1);
        rounds = numMoves/NUM_PLAYERS + 1;
        int whosTurn = numMoves%NUM_PLAYERS;
        g->currPlayer = whosTurn; 

        int player = 0;
//        char tempLoc[3] = {'\0'}; //store strings to convert using places.c
//        int tempLocID = UNKNOWN_LOCATION;

        /* Complete this part only if there's extra time
        int trailsStored = FALSE;
        // Else if there is enough rounds, it will calculate the last 6 
        // moves from the back of the string, instead of the big shifting 
        // method when passing through pastPlays.
        // remember to add if(!trailsStored) in the big for loop if using
        if(rounds > TRAIL_SIZE ){
            //for each player
                //store trails
            trailsStored = TRUE; 
        } 
        */

        for (i=0; i<(strlen(pastPlays)-MOVE_LENGTH +1); i+=(MOVE_LENGTH+1)){
            assert(pastPlays[i] == 'G'||pastPlays[i] =='S'
                ||pastPlays[i] =='H'||pastPlays[i] =='M'
                || pastPlays[i]=='D');
            
            if (pastPlays[i] == 'D'){
                /*
//              //store location  ---- TODO ----
                tempLoc[0] = pastPlays[i+1];
                tempLoc[1] = pastPlays[i+2];
                // shift trail down one spot to make room
                // check if its a normal location
                // if yes, store abbrevToID(tempLoc) in trails[PLAYER_DRACULA]
                // else handle special cases, numbers are in global.h
                */

                // Change blood points
                // -2 if at sea
//is this it?   // +10 if at castle dracula (and not yet defeated???)
                if(getLocation(g, PLAYER_DRACULA) == SEA){
                    g->currHealth[PLAYER_DRACULA] -= LIFE_LOSS_SEA;
                } else if(getLocation(g, PLAYER_DRACULA) == SEA){
                    g->currHealth[PLAYER_DRACULA] += LIFE_GAIN_CASTLE_DRACULA;
                }
                

                //change score
                // if a vampire matures
                if(pastPlays[i+D_ACTION] == 'V'){
                    score -= SCORE_LOSS_VAMPIRE_MATURES;
                }
                // at end of turn
                score -= SCORE_LOSS_DRACULA_TURN;   

            } else {
                if(g->currHealth[player] == 0){
                    // health regained
                    g->currHealth[player] = GAME_START_HUNTER_LIFE_POINTS;
                }

                /*
//              // store location  ---- TODO ----
                tempLoc[0] = pastPlays[i+1];
                tempLoc[1] = pastPlays[i+2];
                // assert its a normal location
                // shift trails[player]
                // store abbrevToID(tempLoc) in trails[player]
                */
//                 // +3 health if rest in city (same city as last turn)
                if (getLocation(g,player) == g->trails[player][1]){
                    g->currHealth[player] += LIFE_GAIN_REST;
                }
                
                // max 9 health
                if (g->currHealth[player]>GAME_START_HUNTER_LIFE_POINTS){
                    g->currHealth[player] = GAME_START_HUNTER_LIFE_POINTS;
                }

                for(j = i+H_ACTION; j < TRAIL_SIZE-H_ACTION-1; j++){
                    // -2 health if encounters trap
                    // -4 health if encounters dracula
                    // -10 blood if drac encounters hunter
                    if(pastPlays[i+j] == 'T'){
                        g->currHealth[player] -= LIFE_LOSS_TRAP_ENCOUNTER;
                    } else if(pastPlays[i+j] == 'D') {
                        g->currHealth[player] -= LIFE_LOSS_DRACULA_ENCOUNTER;
                        g->currHealth[PLAYER_DRACULA] -= LIFE_LOSS_HUNTER_ENCOUNTER;
                    }
                    
                    //if hunter 'dies' decrease score, move to hospital
                    if (g->currHealth[player] == 0){
                        score -= SCORE_LOSS_HUNTER_HOSPITAL;
//                      //move the hunter location ---- TODO ----
                    }
                }
            }

            player ++;
            if (player == 5){
                player = 0;
            }
        }
    }

    g->currScore = score;
    g->currRound = rounds;

    return g;
}
     
     
// Frees all memory previously allocated for the GameView toBeDeleted
void disposeGameView(GameView toBeDeleted)
{
    free( toBeDeleted );
}


//// Functions to return simple information about the current state of the game

// Get the current round
Round getRound(GameView currentView)
{
    return currentView->currRound;
}

// Get the id of current player - ie whose turn is it?
PlayerID getCurrentPlayer(GameView currentView)
{
    return currentView->currPlayer;
}

// Get the current score
int getScore(GameView currentView)
{
    return currentView->currScore;
}

// Get the current health points for a given player
int getHealth(GameView currentView, PlayerID player)
{
    return currentView->currHealth[player];
}

// Get the current location id of a given player
LocationID getLocation(GameView currentView, PlayerID player)
{
    return currentView->trails[player][0];
}

//// Functions that return information about the history of the game

// Fills the trail array with the location ids of the last 6 turns
void getHistory(GameView currentView, PlayerID player,
                            LocationID trail[TRAIL_SIZE])
{
    int i;
    for(i=0;i<TRAIL_SIZE;i++){
        trail[i] = currentView->trails[player][i];
    }
}

//// Functions that query the map to find information about connectivity

// Returns an array of LocationIDs for all directly connected locations

LocationID *connectedLocations(GameView currentView, int *numLocations,
                               LocationID from, PlayerID player, Round round,
                               int road, int rail, int sea)
{
    //REPLACE THIS WITH YOUR OWN IMPLEMENTATION ---- TODO ----
    return NULL;
}
