#include <raylib.h>
#include <raymath.h>

#include <coregame.h>
#include <endgame.h>
#include <stats.h>
#include <gamestates.h>
#include <string.h>
#include <ctype.h>
#include <ball.h>
#include <highscores.h>
#include <maps.h>
#include <explosivebricks.h>

//* Game End Screen
Texture2D victoryImage;
Texture2D defeatImage;

/* Function Definitions */

// Check whether conditions for game end are met
void checkGameEnd()
{
    // death
    if (lives <= 0)
    {
        lockBall();
        switchGameState(GS_GAME_END);
    }
}

// Manage user name input by keyboard
void manageGameEndUserInput()
{
    char ch = GetCharPressed();

    // add character to name
    if (isalnum(ch) && strlen(nameInputStr) < MAX_PLAYER_NAME_LENGTH)
    {
        strncat(nameInputStr, &ch, 1);
    }

    // backspace
    else if (IsKeyPressed(KEY_BACKSPACE) && strlen(nameInputStr) > 0)
    {
        nameInputStr[strlen(nameInputStr) - 1] = '\0';
    }

    //! add space

    // save score
    else if (IsKeyPressed(KEY_ENTER))
    {
        saveNewScore();

        // switch to main menu
        setNewGame();
        switchGameState(GS_MAIN_MENU);
    }

    // delete score (no save)
    else if (IsKeyPressed(KEY_ESCAPE))
    {
        setNewGame();
        switchGameState(GS_MAIN_MENU);
    }
}
