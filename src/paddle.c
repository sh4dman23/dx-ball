#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#include <audio.h>
#include <mainmenu.h>
#include <coregame.h>
#include <collisions.h>
#include <paddle.h>
#include <ball.h>
#include <stats.h>
#include <perks.h>
#include <endgame.h>
#include <gamestates.h>
#include <laserpaddle.h>

Paddle paddles[NUMBER_OF_PADDLES];

int currentPaddle = BASE_PADDLE;

const Vector2 PADDLE_SPEED = (Vector2) { 10 * 60, 0 };  //* unit: pixels per second
const int SPACE_BELOW_PADDLE = 5;                       // pixels below paddle

/* Function Definitions */

// Reset paddle to starting position and base type
void resetPaddle()
{
    resetLasers();
    switchPaddle(BASE_PADDLE);

    paddles[currentPaddle].rect = (Rectangle) {
        (GetScreenWidth() - paddles[currentPaddle].image.width) / 2,
        GetScreenHeight() - paddles[currentPaddle].image.height - SPACE_BELOW_PADDLE,
        paddles[currentPaddle].image.width,
        paddles[currentPaddle].image.height
    };

    // reset mouse position
    if (gameState == GS_MAIN_GAME)
        SetMousePosition(paddles[currentPaddle].rect.x + paddles[currentPaddle].rect.width / 2, WINDOW_HEIGHT / 2);

    paddles[currentPaddle].speed = PADDLE_SPEED;
}

// Update paddle position based on player input
void updatePaddle()
{
    lockMouseToPaddle();

    // required for controlling paddle by mouse
    static Vector2 prevMouse = (Vector2) {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};

    // unlock ball from paddle
    if (ballLockedToPaddle && (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT))) {
        if (justMouseClicked) {
            prevMouse = GetMousePosition();
            justMouseClicked = false;
        }
        else
            ballLockedToPaddle = false;
    }

    // check collisions with ball since its fast moving
    bounceBallOnPaddle();

    const double dt = GetFrameTime();

    // move paddle (AS WELL AS THE MOUSE CURSOR) sideways
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        paddles[currentPaddle].rect.x -= paddles[currentPaddle].speed.x * dt;
        SetMousePosition(paddles[currentPaddle].rect.x + paddles[currentPaddle].rect.width / 2, prevMouse.y);
    }
    else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        paddles[currentPaddle].rect.x += paddles[currentPaddle].speed.x * dt;
        SetMousePosition(paddles[currentPaddle].rect.x + paddles[currentPaddle].rect.width / 2, prevMouse.y);
    }
    // mouse in window and mouse moved
    else if (IsWindowFocused() && CheckCollisionPointRec(GetMousePosition(), (Rectangle) {0, 0, GetScreenWidth(), GetScreenHeight()}) && GetMousePosition().x != prevMouse.x) {
        prevMouse = GetMousePosition();
        paddles[currentPaddle].rect.x = prevMouse.x - paddles[currentPaddle].rect.width / 2;
    }

    // bound within the walls
    if (paddles[currentPaddle].rect.x < 0)
    {
        paddles[currentPaddle].rect.x = 0;
    }
    else if (paddles[currentPaddle].rect.x + paddles[currentPaddle].rect.width > GetScreenWidth())
    {
        paddles[currentPaddle].rect.x = GetScreenWidth() - paddles[currentPaddle].rect.width;
    }
}

// Make it so that mouse cannot exit window while in main game
void lockMouseToPaddle() {
    if (gameState != GS_MAIN_GAME)
        return;

    if (!IsCursorHidden())
        DisableCursor();

    Vector2 mousePos = GetMousePosition();
    if (!CheckCollisionPointRec(mousePos, (Rectangle) {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT})) {
        SetMousePosition(paddles[currentPaddle].rect.x + paddles[currentPaddle].rect.width / 2, GetScreenHeight() / 2);
    }
}

// Switch paddle at current position
void switchPaddle(int type)
{
    if (type < 0 || type > NUMBER_OF_PADDLES)
        return;

    Rectangle oldRec = paddles[currentPaddle].rect;
    currentPaddle = type;

    paddles[type].rect = (Rectangle) {
        oldRec.x + (oldRec.width - paddles[type].image.width) / 2,
        oldRec.y + (oldRec.height - paddles[type].image.height) / 2,
        paddles[type].image.width,
        paddles[type].image.height
    };
    paddles[type].speed = PADDLE_SPEED;
}

// Reduce lives when ball falls below paddle
void killPaddle()
{
    if (lives > 0) {
        lives--;
        playSfx(SFX_BALL_DROP);
    }
    scoreMultiplier = 1.0;

    resetPaddle();
    resetBall();

    resetPerks();
    checkGameEnd();
}
