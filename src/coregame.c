#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <coregame.h>
#include <audio.h>
#include <ball.h>
#include <bricks.h>
#include <explosivebricks.h>
#include <collisions.h>
#include <debugview.h>
#include <endgame.h>
#include <gamestates.h>
#include <highscores.h>
#include <mainmenu.h>
#include <mapeditor.h>
#include <maps.h>
#include <paddle.h>
#include <perks.h>
#include <render.h>
#include <stats.h>

// Icon for the window
Image iconImage;

/* Function Definitions */

// Function called at start of program to initialize everything
void initializeGame()
{
    srand(time(NULL));
    SetRandomSeed(time(NULL));

    initializeAllMaps();
    setNewGame();
    setMapEditor();

    readHighScores();

    switchMusic(currMusicIndex);
}

// Reset everything in memory to prepare for new game
void setNewGame()
{
    numBricks = maxBrickCols * maxBrickRows;
    currentMap = 0;

    initializeCurrentMap();

    resetBall();
    resetPaddle();
    resetPerks();

    resetStats();
    resetAllInput();
}

// Manage all updates based on game state
void updateLoop()
{
    // main menu
    if (gameState == GS_MAIN_MENU)
    {
        manageMainMenuScreen();
    }

    // core game
    if (gameState == GS_MAIN_GAME)
    {
        updatePlayTime();

        updatePerks();
        updatePaddle();
        updateBall();

        updateExplosiveBricks();
        updateExplosions();

        // collisions
        checkAllCollisions();

        // check if lives are over
        checkGameEnd();

        // check whether level is beaten
        checkLevelEnd();
    }

    // end game screen
    else if (gameState == GS_GAME_END)
    {
        manageGameEndUserInput();
    }

    // map editor
    else if (gameState == GS_MAP_EDITOR)
    {
        updateExplosiveBricks();
        checkMapEdit();
    }
}

// Transition to next level
void setNewLevel()
{
    resetPaddle();
    resetBall();
    resetAllInput();

    // start new level with starting lives
    lives = STARTING_LIVES;

    switchToMap(currentMap);
}

// Check whether all breakable bricks in current map are destroyed
void checkLevelEnd()
{
    if (breakableBricksLeft > 0 || lives <= 0)
        return;

    // more levels left
    if (currentMap + 1 < numberOfMaps)
    {
        currentMap++;
        setNewLevel();
    }

    // all levels finished
    else
    {
        lockBall();
        switchGameState(GS_GAME_END);
    }
}

// Resets everything related to player input in memory
void resetAllInput()
{
    sprintf(nameInputStr, "");
}

// Calculate perpendicular distance between point and a straight line
// (NOT line segment)
double distancePointLine(Vector2 point, Vector2 lPoint1, Vector2 lPoint2)
{
    // slope
    double num = fabs((lPoint2.y - lPoint1.y) * point.x - (lPoint2.x - lPoint1.x) * point.y + lPoint2.x * lPoint1.y - lPoint2.y * lPoint1.x);
    double den = sqrt(pow(lPoint2.x - lPoint1.x, 2) + pow(lPoint2.y - lPoint1.y, 2));
    double dist = 0;

    // if lPoint1 and lPoint2 are same
    if (den == 0)
        dist = Vector2Distance(point, lPoint1);
    else
        dist = num / den;
    return dist;
}
