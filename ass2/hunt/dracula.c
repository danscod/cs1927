// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "DracView.h"
#include "Places.h"


void decideDraculaMove(DracView gameState)
{
	//Grabs the locations of all 4 hunters
	int player0 = whereIs(gameState, 0);
	int player1 = whereIs(gameState, 1);
	int player2 = whereIs(gameState, 2);
	int player3 = whereIs(gameState, 3);

	int *numLocations = malloc(sizeof(int)*NUM_PLAYERS);
	//assert(numLocations!=NULL);

	/*//arrays of places the hunters can go
	LocationID *p0can = whereCanTheyGo(gameState, &numLocations[0], 0, TRUE, TRUE, TRUE);
	LocationID *p1can = whereCanTheyGo(gameState, &numLocations[1], 0, TRUE, TRUE, TRUE);
	LocationID *p2can = whereCanTheyGo(gameState, &numLocations[2], 0, TRUE, TRUE, TRUE);
	LocationID *p3can = whereCanTheyGo(gameState, &numLocations[3], 0, TRUE, TRUE, TRUE);
	
	//the hunter cannot move to this place: TRUE/FALSE
	int p0clear;
	int p1clear;
	int p2clear;
	int p3clear;*/
	//if the location is in the trail
	int inTrail = FALSE;

	//contains the array of Dracula possible moves
	LocationID *possible = whereCanIgo(gameState, &numLocations[PLAYER_DRACULA], TRUE, FALSE);
	
	int i = 0;
	int j = 0;


	// move where no hunter is and no hunter can get to
	int move = UNKNOWN_LOCATION;
	// move where no hunter is
	int secondbest = UNKNOWN_LOCATION; 
	
	LocationID *trail = malloc(TRAIL_SIZE*sizeof(int));
	giveMeTheTrail(gameState, PLAYER_DRACULA, trail);
	
	
	
	while(move == UNKNOWN_LOCATION && i < numLocations[PLAYER_DRACULA]){
		/*p0clear = TRUE;
		p1clear = TRUE;
		p2clear = TRUE;
		p3clear = TRUE;*/
		inTrail = FALSE;
		if (possible[i]!= player0 && possible[i]!= player1 && possible[i]!= player2 && possible[i]!=player3){
			for(j=0;j<TRAIL_SIZE;j++){
				if (possible[i] == trail[j]){
					inTrail = TRUE;
				}
			}
			
			if (!inTrail){
				int type = idToType(possible[i]);
				if (type == 2){
					i++;
				}
				secondbest = possible[i];
				move = secondbest; //comment this out later and nest the for loops
			}
			/*for (j=0; j<numLocations[0]; j++){
				if(possible[i] == p0can[j]){
					p0clear = FALSE;
					break;
				}
			}
			if(p0clear){
				for (j=0; j<numLocations[0]; j++){
					if(possible[i] == p1can[j]){
						p1clear = FALSE;
						break;
					}
				}
				if (p1clear){
					for (j=0; j<numLocations[0]; j++){
						if(possible[i] == p2can[j]){
							p2clear = FALSE;
							break;
						}
					}
					if (p2clear){
						for (j=0; j<numLocations[0]; j++){
							if(possible[i] == p3can[j]){
								p3clear = FALSE;
								break;
							}
						}
						if (p3clear){
							move = possible[i];
						}
					}
				}
			}*/
		}
		printf("%d\n",i);
		i++;
	}

	
//-----------------------------LOADING BEST MOVE INTO VAR------------------------	
	//Initialises the randomisation
	srand(time(NULL));
	int r = rand()%2;
	
	if (move == UNKNOWN_LOCATION){
		move = secondbest;
		if (secondbest == UNKNOWN_LOCATION){
			if (r == 1){ 
				registerBestPlay("TP","Hello my delicious");
			} else if (r == 0 && inTrail == 0){
				registerBestPlay("D5","Hello my ");		
			} else if (r == 0 && inTrail == 1){
				registerBestPlay("TP","Hello my delicious");
			}
		} else {
			char *moveAbbrev = idToAbbrev(move);
			registerBestPlay(moveAbbrev,"Hello my darlings");
		}
	} else {
		char *moveAbbrev = idToAbbrev(move);
		registerBestPlay(moveAbbrev,"Hello my dears");
	}

}