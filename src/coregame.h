#ifndef COREGAME_H
#define COREGAME_H

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

// Game window
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define GAME_WINDOW_TITLE "DXBall"

/* Function Prototypes */
void initializeGame();      // Initialize everything at the start of the game
void setNewGame();          // Reset everything in memory for new game

void updateLoop();          // Manage all updates based on game state

void setNewLevel();         // Transition to next level
void checkLevelEnd();       // Check whether all breakable bricks in current map are destroyed

void resetAllInput();       // Resets everything related to player input in memory

double distancePointLine(Vector2 point, Vector2 lPoint1, Vector2 lPoint2);
#endif
