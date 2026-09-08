#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdlib.h>

#include <audio.h>
#include <ball.h>
#include <bricks.h>
#include <collisions.h>
#include <coregame.h>
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

int main(void)
{
    SetTraceLogLevel(LOG_ALL);
    SetExitKey(KEY_NULL);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_WINDOW_TITLE);
    InitAudioDevice();

    SetTargetFPS(60);

    loadSprites();

    // initialize game
    initializeGame();

    // main loop
    while (!WindowShouldClose() && !exitGame)
    {
        manageDebugView();
        manageGameStateChanges();

        // updates
        updateLoop();
        updateAudio();

        // draw
        BeginDrawing();
        ClearBackground(BLACK);
        drawLoop();
        EndDrawing();
    }

    unloadSprites();
    unloadAudio();
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
