#include "raylib.h"
#include "raymath.h"
#include "stats.h"
#include "gamestates.h"
#include "ball.h"
#include "paddle.h"

// Game statistics
int playerScore = 0;
double scoreMultiplier = 1.0;
int playtime = 0;
int lives = 0;
const int STARTING_LIVES = 3;

const int BASE_BRICK_HIT_SCORE = 50;

// Player name input
char nameInputStr[MAX_PLAYER_NAME_LENGTH + 1] = {'\0'};

// Texture for life
Texture2D lifeTexture;

/* Function Definitions */

// Increase lives by +1
void increaseLives()
{
    if (lives >= STARTING_LIVES * 2)
        return;

    lives++;
    //? play +life sound
}

// Update playtime variable every second
void updatePlayTime()
{
    static double time = 0;
    const double interval = 1.0;

    if (gameState != GS_MAIN_GAME || ballLockedToPaddle)
        return;

    time += GetFrameTime();
    if (time >= interval)
    {
        playtime++;
        time = 0;
    }
}

void resetStats()
{
    playerScore = 0;
    lives = STARTING_LIVES;
    playtime = 0;
    scoreMultiplier = 1.0;
}

// Increase player score
void increaseScore(int change)
{
    // score based on speed
    // 50% more score for smaller paddle
    // 50% more score for shrunk ball
    int increase = change * (1 + 0.5 * (currentPaddle == SHRUNK_PADDLE) + 0.5 * (ball.radius == SHRUNK_BALL_RADIUS));
    increase *= scoreMultiplier;

    playerScore += increase;
}
