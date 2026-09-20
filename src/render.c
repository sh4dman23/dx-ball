#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <render.h>
#include <coregame.h>
#include <mainmenu.h>
#include <gamestates.h>
#include <highscores.h>
#include <stats.h>
#include <ball.h>
#include <paddle.h>
#include <endgame.h>
#include <bricks.h>
#include <explosivebricks.h>
#include <maps.h>
#include <mapeditor.h>
#include <perks.h>
#include <debugview.h>
#include <laserpaddle.h>

// Core game UI
const double PADDING_ABOVE_UI = 20;
const double PADDING_SIDES_UI = 20;

/* Function Definitions */

void loadAllImages() {
    iconImage = LoadImage(GAME_WINDOW_ICON_FILEPATH);
}

void unloadAllImages() {
    UnloadImage(iconImage);
}

// Load all textures
void loadSprites()
{
    // main menu logo
    for (int i = MAIN_MENU_LOGO_START - 1; i < MAIN_MENU_LOGO_END; i++)
    {
        char mainMenuLogoTextureFilePath[50];
        sprintf(mainMenuLogoTextureFilePath, "%slogo/%d.png", MAIN_MENU_TEXTURES_PATH, i + 1);
        mainMenuLogo[i] = LoadTexture(mainMenuLogoTextureFilePath);
    }

    // main menu ball
    for (int i = MAIN_MENU_BALL_START - 1; i < MAIN_MENU_BALL_END; i++)
    {
        char mainMenuBallTextureFilePath[50];
        sprintf(mainMenuBallTextureFilePath, "%sball/%d.png", MAIN_MENU_TEXTURES_PATH, i + 1);
        mainMenuBall[i] = LoadTexture(mainMenuBallTextureFilePath);
    }

    // map editor buttons
    mapEditorButtonTextures[1] = LoadTexture("./assets/ui/larrow.png");
    mapEditorButtonTextures[2] = LoadTexture("./assets/ui/rarrow.png");
    mapEditorButtonTextures[3] = LoadTexture("./assets/ui/plus.png");
    mapEditorButtonTextures[4] = LoadTexture("./assets/ui/delete.png");

    // victory or defeat title
    victoryImage = LoadTexture("./assets/ui/victory.png");
    defeatImage = LoadTexture("./assets/ui/defeat.png");

    // main game ui
    lifeTexture = LoadTexture("./assets/ui/life.png");
    ballImage = LoadTexture("./assets/ball.png");

    // paddles
    for (int i = 0; i < NUMBER_OF_PADDLES; i++) {
        char filename[50];
        sprintf(filename, "./assets/paddles/%d.png", i);
        paddles[i].image = LoadTexture(filename);
    }

    // high score title
    highScoresTitleImage = LoadTexture("./assets/ui/highScores.png");

    //* explosive bricks (must be before bricks)
    for (int i = 0; i < NUM_EXPLOSIVE_BRICK_FRAMES; i++) {
        char filepath[50];
        sprintf(filepath, "%s/%d.png", EXPLOSIVE_BRICKS_TEXTURES_PATH, i);
        explosiveBrickTextures[i] = LoadTexture(filepath);
    }

    //* bricks : 0 1 2 3 ... -3 -2 -1
    for (int i = MIN_BRICK_TYPE; i <= MAX_BRICK_TYPE; i++)
    {
        char brickTextureFilePath[50];
        int brickTextureIndex = 0;

        if (i == 0)
            continue;
        else if (i > 0)
            brickTextureIndex = i;
        else if (i < 0)
            brickTextureIndex = NUM_BRICK_TEXTURES + i + 1;

        if (i == BRICK_EXPLOSIVE) {
            brickTextures[brickTextureIndex] = explosiveBrickTextures[0];
            continue;
        }

        sprintf(brickTextureFilePath, "%s/%d.png", BRICK_TEXTURES_PATH, i);
        brickTextures[brickTextureIndex] = LoadTexture(brickTextureFilePath);
    }

    // explosion frames
    for (int i = 0; i < NUM_EXPLOSION_FRAMES; i++) {
        char filePath[100];
        sprintf(filePath, "%s/%d.png", EXPLOSION_TEXTURES_PATH, i);
        explosionFrames[i] = LoadTexture(filePath);
    }

    // perks
    for (int i = 0; i < NUMBER_OF_PERKS; i++)
    {
        char filename[50];
        sprintf(filename, "%s%s.png", PERKS_IMG_PATH, perks[i].filename);
        perks[i].img = LoadTexture(filename);
    }

    // laser particle
    laserTexture = LoadTexture(LASER_TEXTURE_FILEPATH);
}

// Inverse function to loadSprites(); unloads all sprites
void unloadSprites()
{
    for (int i = 0; i < NUM_MAP_EDITOR_BUTTONS; i++)
        UnloadTexture(mapEditorButtonTextures[i]);

    UnloadTexture(victoryImage);
    UnloadTexture(defeatImage);

    UnloadTexture(lifeTexture);
    UnloadTexture(ballImage);

    // paddles
    for (int i = 0; i < NUMBER_OF_PADDLES; i++)
        UnloadTexture(paddles[i].image);

    UnloadTexture(highScoresTitleImage);

    // explosive bricks
    for (int i = 0; i <= NUM_EXPLOSIVE_BRICK_FRAMES; i++)
        UnloadTexture(explosiveBrickTextures[i]);

    // bricks
    for (int i = 0; i <= NUM_BRICK_TEXTURES; i++)
        UnloadTexture(brickTextures[i]);

    // explosion frames
    for (int i = 0; i < NUM_EXPLOSION_FRAMES; i++) {
        UnloadTexture(explosionFrames[i]);
    }

    // perks
    for (int i = 0; i < NUMBER_OF_PERKS; i++)
        UnloadTexture(perks[i].img);

    // main menu logo and ball
    for (int i = MAIN_MENU_LOGO_START - 1; i < MAIN_MENU_LOGO_END; i++)
        UnloadTexture(mainMenuLogo[i]);
    for (int i = MAIN_MENU_BALL_START - 1; i < MAIN_MENU_BALL_END; i++)
        UnloadTexture(mainMenuBall[i]);

    // laser particle
    UnloadTexture(laserTexture);
}

// Contains all draw calls; func called inside game loop
void drawLoop()
{
    // core game
    if (gameState == GS_MAIN_GAME)
    {
        drawMainGame();
    }
    // game end
    else if (gameState == GS_GAME_END)
    {
        drawGameEnd();
    }
    // map editor
    else if (gameState == GS_MAP_EDITOR)
    {
        drawMapEditor();
    }
    // high scores
    else if (gameState == GS_HIGH_SCORES)
    {
        drawHighScoresScreen();
    }
}

// Draw main game
void drawMainGame()
{
    drawPaddle();
    drawBall();
    drawBricks();
    drawPerks();
    drawLasers();
    if (debugView)
        drawDebugView();
    else
        drawMainGameUI();
}

// Draw ui for main game
void drawMainGameUI()
{
    // time (left)
    DrawText("Time", PADDING_SIDES_UI, PADDING_ABOVE_UI, 16, RAYWHITE);
    char timeText[20];
    sprintf(timeText, "%02d : %02d", playtime / 60, playtime % 60);
    DrawText(timeText, PADDING_SIDES_UI, PADDING_ABOVE_UI + 16 * 1.4, 16, RAYWHITE);

    // score (middle)
    DrawText("Score", (GetScreenWidth() - MeasureText("Score", 18)) / 2, PADDING_ABOVE_UI, 18, RAYWHITE);
    char scoreText[20];
    sprintf(scoreText, "%d", playerScore);
    DrawText(scoreText, (GetScreenWidth() - MeasureText(scoreText, 18)) / 2, PADDING_ABOVE_UI + 18 * 1.4, 18, RAYWHITE);

    // lives (right)
    for (int i = 0; i < lives; i++)
    {
        DrawTextureRec(
            lifeTexture,
            (Rectangle){0, 0, lifeTexture.width, lifeTexture.height},
            (Vector2){
                GetScreenWidth() - PADDING_SIDES_UI - (i + 1) * lifeTexture.width - i * 5,
                PADDING_ABOVE_UI + 20
            },
            WHITE
        );
    }
}

// Ball
void drawBall()
{
    DrawTextureEx(ballImage, (Vector2){ball.pos.x - ball.radius, ball.pos.y - ball.radius}, 0.0f, 1.0f, WHITE);
}

// Paddle
void drawPaddle()
{
    if (!laserActivated) {
        // normal paddle
        DrawTextureEx(paddles[currentPaddle].image, (Vector2){paddles[currentPaddle].rect.x, paddles[currentPaddle].rect.y}, 0.0f, 1.0f, WHITE);
    }
    else {
        // laser paddle
        int currLaserPaddle = BASE_LASER_PADDLE;
        if (currentPaddle == EXPANDED_PADDLE)
            currLaserPaddle = EXPANDED_LASER_PADDLE;
        else if (currentPaddle == SHRUNK_PADDLE)
            currLaserPaddle = SHRUNK_LASER_PADDLE;

            DrawTextureEx(
                paddles[currLaserPaddle].image,
                (Vector2) {
                    paddles[currentPaddle].rect.x,
                    paddles[currentPaddle].rect.y + paddles[currentPaddle].rect.height - paddles[currLaserPaddle].image.height
                },
                0.0f, 1.0f, WHITE
            );
    }
}

// Bricks
void drawBricks()
{
    for (int i = 0; i < numBricks; i++)
    {
        // pick texture
        Texture2D brickImage;
        if (bricks[i].type == BRICK_EMPTY)
            continue;
        else if (bricks[i].type == BRICK_EXPLOSIVE)
            brickImage = explosiveBrickTextures[explosiveBrickFrame];               // animation frame for explosive brick
        else if (bricks[i].type == BRICK_EXPLODING)
            brickImage = explosionFrames[explosions[i].currentFrame];               // animation frame for explosion
        else if (bricks[i].type > 0)
            brickImage = brickTextures[bricks[i].type];                             // standard bricks
        else if (bricks[i].type < 0)
            brickImage = brickTextures[NUM_BRICK_TEXTURES + bricks[i].type + 1];    // special bricks (except explosives)

        // draw brick
        DrawTextureEx(brickImage, (Vector2){bricks[i].rect.x, bricks[i].rect.y}, 0.0f, 1.0f, WHITE);
    }
}

// Perks
void drawPerks()
{
    for (int i = 0; i < NUMBER_OF_PERKS; i++)
    {
        if (perks[i].pos.x != -1 && perks[i].pos.y != -1)
        {
            // DrawRectangle(perks[i].pos.x, perks[i].pos.y, 32, 30, WHITE);
            DrawTextureEx(perks[i].img, perks[i].pos, 0, 1, WHITE);
        }
    }
}

// Lasers
void drawLasers() {
    for (int i = 0; i < MAX_LASERS_PER_SCREEN; i++) {
        if (lasers[i].leftOnScreen) {
            DrawTextureV(laserTexture, lasers[i].left, WHITE);
            // DrawRectangleV(lasers[i].left, (Vector2){laserTexture.width, laserTexture.height}, WHITE);
        }
        if (lasers[i].rightOnScreen) {
            DrawTextureV(laserTexture, lasers[i].right, WHITE);
            // DrawRectangleV(lasers[i].left, (Vector2){laserTexture.width, laserTexture.height}, WHITE);
        }
    }
}

// Draw map editor on screen
void drawMapEditor()
{
    const int fontSize = 16;

    // box for showing # of current selected map
    DrawRectangleLinesEx(mapEditorButtons[0], 1, WHITE);

    char mapText[20];
    sprintf(mapText, "Map %d", currentMap + 1);
    DrawText(mapText, mapEditorButtons[0].x + 10, mapEditorButtons[0].y + (mapEditorButtons[0].height - fontSize) / 2, fontSize, WHITE);

    // buttons to change current selected map
    for (int i = 1; i <= 4; i++)
    {
        Color buttonColor = GRAY;

        // grey out map addition and deletion buttons
        if ((i == 4 && numberOfMaps <= 1) || (i == 3 && numberOfMaps >= MAX_NUMBER_OF_MAPS))
            buttonColor = GRAY;

        // highlight buttons on hover
        else if (CheckCollisionPointRec(GetMousePosition(), mapEditorButtons[i]))
        {
            buttonColor = WHITE;
        }

        DrawRectangleLinesEx(mapEditorButtons[i], 1, WHITE);

        DrawTexturePro(
            mapEditorButtonTextures[i],
            (Rectangle){0, 0, mapEditorButtonTextures[i].width, mapEditorButtonTextures[i].height},
            (Rectangle){mapEditorButtons[i].x + (mapEditorButtons[i].width - fontSize) / 2, mapEditorButtons[i].y + fontSize / 2, fontSize, fontSize},
            (Vector2){0, 0},
            0.0f,
            buttonColor);
    }

    // color based on whether mouse points to corresponding brick
    Color notHighlighted = (Color){255, 255, 255, .35 * 255}, highlighted = RAYWHITE;
    Vector2 mousePos = GetMousePosition();

    // row and column numbers
    for (int i = 0; i < maxBrickCols || i < maxBrickRows; i++)
    {
        char numText[20];
        sprintf(numText, "%d", i + 1);
        Vector2 textSize = MeasureTextEx(GetFontDefault(), numText, 15, 0);

        // check if mouse is on ANY brick at all
        bool mouseOnBricks = CheckCollisionPointRec(
            mousePos,
            (Rectangle){
                bricks[0].rect.x,
                bricks[0].rect.y,
                maxBrickCols * BRICK_WIDTH,
                maxBrickRows * BRICK_HEIGHT
            }
        );

        Color numColor;
        if (i < maxBrickCols)
        {
            if (mousePos.x >= bricks[i].rect.x && mousePos.x <= bricks[i].rect.x + BRICK_WIDTH && mouseOnBricks)
                numColor = highlighted;
            else
                numColor = notHighlighted;

            DrawText(numText, bricks[i].rect.x + (BRICK_WIDTH - textSize.x) / 2, bricks[i].rect.y - textSize.y - 5, 15, numColor);
            DrawText(numText, bricks[i].rect.x + (BRICK_WIDTH - textSize.x) / 2, bricks[maxBrickRows * maxBrickCols - 1].rect.y + BRICK_HEIGHT + 5, 15, numColor);
        }

        if (i < maxBrickRows)
        {
            if (mousePos.y >= bricks[i * maxBrickCols].rect.y && mousePos.y <= bricks[i * maxBrickCols].rect.y + BRICK_HEIGHT && mouseOnBricks)
                numColor = highlighted;
            else
                numColor = notHighlighted;

            DrawText(numText, bricks[i * maxBrickCols].rect.x - textSize.x - 10, bricks[i * maxBrickCols].rect.y + (BRICK_HEIGHT - 15) / 2, 15, numColor);
            DrawText(numText, bricks[(i + 1) * maxBrickCols - 1].rect.x + BRICK_WIDTH + 10, bricks[(i + 1) * maxBrickCols - 1].rect.y + (BRICK_HEIGHT - 15) / 2, 15, numColor);
        }
    }

    // map selection area
    DrawRectangleRec(mapSelectionRegion, (Color) {255, 255, 255, 150});
    for (int i = 0; i < numBricks; i++) {
        if (CheckCollisionRecs(bricks[i].rect, mapSelectionRegion)) {
             DrawRectangleLinesEx(bricks[i].rect, 2, highlighted);
        }
    }

    // bricks
    drawBricks();

    // brick outlines
    for (int i = 0; i < numBricks; i++)
    {
        // check if mouse inside brick
        if (CheckCollisionPointRec(mousePos, bricks[i].rect))
            DrawRectangleLinesEx(bricks[i].rect, 2, highlighted);
        else
            DrawRectangleLinesEx(bricks[i].rect, 1, notHighlighted);
    }

    // selected bricks
    for (int i = 0; i < numBricks; i++) {
        if (brickSelected[i]) {
            if (bricks[i].type == BRICK_EMPTY)
                DrawRectangleRec(bricks[i].rect, (Color) {255, 255, 255, 0.2 * 255});
            else
                DrawRectangleRec(bricks[i].rect, (Color) {255, 255, 255, 0.4 * 255});
            DrawRectangleLinesEx(bricks[i].rect, 1, highlighted);
        }
    }
}

// Draw leaderboards
void drawHighScoresScreen()
{
    // size and spacings for table
    const int fontSize = 16;
    const double rowHeight = 18 * 2;

    const int NUM_HEADERS = 4;
    const char *colHeaders[4] = {
        "#",
        "Name",
        "Score",
        "Time"
    };

    // 65% of screen width
    const double tableWidth = 0.65 * GetScreenWidth();
    const double colWidth[4] = {
        0.10 * tableWidth,
        0.45 * tableWidth,
        0.25 * tableWidth,
        0.20 * tableWidth};

    // variables to keep track of position to draw stuff
    double px = (GetScreenWidth() - highScoresTitleImage.width) / 2;
    double py = PADDING_ABOVE_MAP - 20;

    // draw high scores title
    DrawTexture(highScoresTitleImage, px, py, WHITE);
    py += highScoresTitleImage.height + 20;

    // draw table headers
    px = (GetScreenWidth() - tableWidth) / 2;
    for (int i = 0; i < NUM_HEADERS; i++)
    {
        // cell outline
        DrawRectangleLines(px, py, colWidth[i], rowHeight, RAYWHITE);

        DrawText(colHeaders[i], px + (colWidth[i] - MeasureText(colHeaders[i], fontSize + 2)) / 2, py + (rowHeight - fontSize - 2) / 2, fontSize + 2, RAYWHITE);

        px += colWidth[i];
    }

    px = (GetScreenWidth() - tableWidth) / 2;
    py += rowHeight;

    // draw cells and values inside them
    for (int i = 0; i < MAX_HIGH_SCORES; i++)
    {
        for (int j = 0; j < NUM_HEADERS; j++)
        {
            // cell outline
            DrawRectangleLines(px, py, colWidth[j], rowHeight, RAYWHITE);

            char cellText[50] = {'\0'};

            if (j == 0)
            { // #
                sprintf(cellText, "%d", i + 1);
            }
            else if (j == 1)
            { // name
                sprintf(cellText, "%s", highScores[i].name);
            }
            else if (j == 2)
            { // score
                sprintf(cellText, "%d", highScores[i].score);
            }
            else if (j == 3)
            { // playtime
                sprintf(cellText, "%02d : %02d", highScores[i].playtime / 60, highScores[i].playtime % 60);
            }

            // empty cell
            if (i >= numHighScores && j != 0)
            {
                sprintf(cellText, "-");
            }

            DrawText(
                cellText,
                px + (colWidth[j] - MeasureText(cellText, fontSize + 2)) / 2,
                py + (rowHeight - fontSize - 2) / 2,
                fontSize + 2,
                i == 0 ? (Color){211, 175, 55, 255} : i == 1 ? (Color){187, 194, 204, 255}
                                                  : i == 2   ? (Color){228, 149, 60, 255}
                                                             : RAYWHITE);

            px += colWidth[j];
        }

        px = (GetScreenWidth() - tableWidth) / 2;
        py += rowHeight;
    }
}

// Draw victory/defeat screen
void drawGameEnd()
{
    const int fontSize = 18;

    double px = 0, py = PADDING_ABOVE_MAP;
    Texture2D statusImage;

    // final map finished
    if (lives > 0)
        statusImage = victoryImage;
    // lives ran out
    else
        statusImage = defeatImage;

    DrawTexture(statusImage, (GetScreenWidth() - statusImage.width) / 2, py, WHITE);
    py += 25 + statusImage.height;

    // score
    char scoreText[50];
    sprintf(scoreText, "Score: %d", playerScore);
    DrawText(scoreText, (GetScreenWidth() - MeasureText(scoreText, fontSize)) / 2, py, fontSize, RAYWHITE);
    py += fontSize * 1.5;

    // time played
    char timeText[50];
    sprintf(timeText, "Time Played: ");
    if (playtime / 60 > 0)
    {
        char minText[20];
        sprintf(minText, "%d minutes, ", playtime / 60);
        strcat(timeText, minText);
    }
    char secText[20];
    sprintf(secText, "%d seconds", playtime % 60);
    strcat(timeText, secText);

    DrawText(timeText, (GetScreenWidth() - MeasureText(timeText, fontSize)) / 2, py, fontSize, WHITE);
    py += fontSize * 2;

    // show name prompt
    char *namePromptText = "Enter your name to save score: ";
    DrawText(namePromptText, (GetScreenWidth() - MeasureText(namePromptText, fontSize)) / 2, py, fontSize, WHITE);
    py += fontSize * 1.4;

    // show currently entered name
    char nameDisplayStr[MAX_PLAYER_NAME_LENGTH + 2];
    sprintf(nameDisplayStr, "%s%c", nameInputStr, time(NULL) % 2 ? '_' : ' ');
    DrawText(nameDisplayStr, (GetScreenWidth() - MeasureText(nameDisplayStr, fontSize)) / 2, py, fontSize, WHITE);
    py += fontSize * 1.4;
}

// Draw debug view
void drawDebugView()
{
    if (!debugView)
        return;

    // ball and paddle outline
    DrawCircleLinesV(ball.pos, ball.radius, RED);
    DrawRectangleLinesEx((Rectangle){paddles[currentPaddle].rect.x - 4, paddles[currentPaddle].rect.y, paddles[currentPaddle].rect.width + 8, paddles[currentPaddle].rect.height}, 1, RED);

    // ball direction
    DrawLineEx(ball.pos, Vector2Add(ball.pos, Vector2Scale(Vector2Normalize(ball.speed), 10 * ball.radius)), 2, WHITE);

    // stats
    char textStr[50] = {'\0'};
    sprintf(textStr, "FPS: %d", GetFPS());
    DrawText(textStr, 10, 10, 15, RAYWHITE);

    sprintf(textStr, "Score: %d", playerScore);
    DrawText(textStr, 10, 30, 15, RAYWHITE);

    sprintf(textStr, "PlayTime: %d", playtime);
    DrawText(textStr, 10, 50, 15, RAYWHITE);

    sprintf(textStr, "Lives: %d", lives);
    DrawText(textStr, 10, 70, 15, RAYWHITE);

    sprintf(textStr, "SpeedX: %.2f; SpeedY: %.2f", ball.speed.x, ball.speed.y);
    DrawText(textStr, GetScreenWidth() - 250, 10, 15, RAYWHITE);

    // map area
    // DrawRectangle(PADDING_ON_MAP_SIDES, PADDING_ABOVE_MAP, GetScreenWidth() - PADDING_ON_MAP_SIDES * 2, GetScreenHeight() - PADDING_ABOVE_MAP - PADDING_BELOW_MAP, (Color){255, 0, 0, 50});

    // brick outlines
    for (int i = 0; i < numBricks; i++)
        DrawRectangleLinesEx(bricks[i].rect, 1, RAYWHITE);
}

// Main menu animations
void drawMainMenuLogoAnimations()
{
    float frameTime = GetFrameTime();
    mainMenuLogoTimer += frameTime;
    mainMenuBallTimer += frameTime;
    if (mainMenuLogoTimer >= mainMenuLogoFrameTime)
    {
        mainMenuLogoTimer = 0.0f;
        mainMenuLogoCurrentFrame = (mainMenuLogoCurrentFrame + 1) % MAIN_MENU_LOGO_END;
    }
    if (mainMenuBallTimer >= mainMenuBallFrameTime)
    {
        mainMenuBallTimer = 0.0f;
        mainMenuBallCurrentFrame = (mainMenuBallCurrentFrame + 1) % MAIN_MENU_BALL_END;
    }

    for (int i = MAIN_MENU_LOGO_START - 1; i < MAIN_MENU_LOGO_END; i++)
    {
        DrawTexture(mainMenuLogo[mainMenuLogoCurrentFrame], (WINDOW_WIDTH - 600) / 2, WINDOW_HEIGHT / 2 - 230, WHITE);
    }
    for (int i = MAIN_MENU_BALL_START - 1; i < MAIN_MENU_BALL_END; i++)
    {
        DrawTexture(mainMenuBall[mainMenuBallCurrentFrame], WINDOW_WIDTH - 180, WINDOW_HEIGHT - 180, WHITE);
    }
}
