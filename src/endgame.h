#ifndef ENDGAME_H
#define ENDGAME_H
#include <stats.h>

// Game end screen victory/defeat textures
extern Texture2D victoryImage;
extern Texture2D defeatImage;

// Player name input
extern char nameInputStr[MAX_PLAYER_NAME_LENGTH + 1];

/* Function Prototypes */
void checkGameEnd();                    // Check whether conditions for game end are met
void manageGameEndUserInput();          // Manage user name input by keyboard

#endif
