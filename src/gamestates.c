#include <raylib.h>
#include <raymath.h>
#include <gamestates.h>
#include <mainmenu.h>

int gameState = GS_MAIN_MENU;

/* Function Definitions */

// Manage game state changes due to ingame user interaction
void manageGameStateChanges()
{
    if (gameState == GS_MAIN_MENU)
    {
        checkMainMenuButtonClick(GetMousePosition());
    }

    // escape key pressed from any game state except main menu
    if (gameState != GS_MAIN_MENU && IsKeyPressed(KEY_ESCAPE))
    {
        switchGameState(GS_MAIN_MENU);
    }
}

// Switches game states, and does necessary changes
void switchGameState(int state)
{
    if (gameState == state)
        return;

    // main game -> any other mode
    if (gameState == GS_MAIN_GAME)
    {
        debugView = false;
        lockBall();
    }

    // main game -> main menu
    if (gameState == GS_MAIN_GAME && state == GS_MAIN_MENU)
    {
        // erase progress
        setNewGame();
    }

    // main menu / main game -> map editor
    if ((gameState == GS_MAIN_MENU || gameState == GS_MAIN_GAME) && state == GS_MAP_EDITOR)
    {
        setNewGame();
    }

    // main menu -> main game
    if (gameState == GS_MAIN_MENU && state == GS_MAIN_GAME) {
        setNewGame();
    }

    // end game -> any other mode (only main menu accessible)
    if (gameState == GS_GAME_END)
    {
        setNewGame();
    }

    // map editor -> any other mode
    if (gameState == GS_MAP_EDITOR)
    {
        // save changes
        saveCurrentMap();

        // switch to first map
        switchToMap(0);
    }

    // hide cursor for main game
    if (state == GS_MAIN_GAME)
        HideCursor();
    else
        ShowCursor();

    gameState = state;
}
