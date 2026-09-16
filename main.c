#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdlib.h>

#include <audio.h>
#include <coregame.h>
#include <debugview.h>
#include <gamestates.h>
#include <mainmenu.h>
#include <render.h>

int main(void)
{
    SetTraceLogLevel(LOG_ALL);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_WINDOW_TITLE);
    InitAudioDevice();

    // icon
    iconImage = LoadImage(GAME_WINDOW_ICON_FILEPATH);
    SetWindowIcon(iconImage);
    UnloadImage(iconImage);

    SetTargetFPS(120);

    // load sounds and sprites
    loadAllAudio();
    loadSprites();

    // initialize game
    initializeGame();

    // set game to not exit by escape key
    SetExitKey(KEY_NULL);

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
    unloadAllAudio();
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
