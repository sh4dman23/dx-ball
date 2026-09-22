#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdlib.h>

#include "audio.h"
#include "coregame.h"
#include "debugview.h"
#include "gamestates.h"
#include "mainmenu.h"
#include "render.h"
#include "fonts.h"

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

    loadAllFonts();

    // initialize game
    initializeGame();

    // set game to not exit by escape key
    SetExitKey(KEY_NULL);

    // for crt shader
    Shader shader = LoadShader(0, "./assets/shaders/crt.fs");
    RenderTexture2D targetRenderTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

    // main loop
    while (!WindowShouldClose() && !exitGame)
    {
        manageDebugView();
        manageGameStateChanges();

        // Updates
        updateLoop();
        updateAudio();

        // Draw
        BeginTextureMode(targetRenderTexture);
        {
            ClearBackground(BLACK); // clear texture background
            drawLoop();
        }
        EndTextureMode();

        BeginDrawing();
        {
            ClearBackground(BLACK); // clear screen background
            BeginShaderMode(shader);
            DrawTextureRec(targetRenderTexture.texture, (Rectangle){ 0, 0, (float) targetRenderTexture.texture.width, (float) -targetRenderTexture.texture.height }, (Vector2){ 0, 0 }, WHITE);


            EndShaderMode();
        }
        EndDrawing();
    }

    UnloadShader(shader);
    UnloadRenderTexture(targetRenderTexture);

    unloadAllFonts();
    unloadSprites();
    unloadAllAudio();
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
