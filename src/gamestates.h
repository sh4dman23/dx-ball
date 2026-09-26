#ifndef GAMESTATES_H
#define GAMESTATES_H

// Game States
typedef enum {
    GS_MAIN_MENU = 0,
    GS_MAIN_GAME,
    GS_GAME_END,
    GS_MAP_EDITOR,
    GS_HIGH_SCORES,
    GS_CONTROLS,
} GameState;

// #define GS_MAIN_MENU 0                  // 0 = main menu
// #define GS_MAIN_GAME 1                  // 1 = main game
// #define GS_GAME_END 2                   // 2 = game end
// #define GS_MAP_EDITOR 3                 // 3 = map editor
// #define GS_HIGH_SCORES 4                // 4 = high scores

extern int gameState;

/* Function Prototypes */
void manageGameStateChanges();          // Manage game state changes due to ingame user interaction
void switchGameState(int state);        // Switches game states, and does necessary changes

#endif
