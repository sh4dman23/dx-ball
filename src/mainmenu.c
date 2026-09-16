#include <raylib.h>
#include <raymath.h>
#include <mainmenu.h>
#include <gamestates.h>
#include <render.h>
#include <coregame.h>

bool exitGame = false;
bool justMouseClicked = false;

int mainMenuLogoCurrentFrame = 0;
const float mainMenuLogoFrameTime = 0.065f;
float mainMenuLogoTimer = 0.0f;
int mainMenuBallCurrentFrame = 0;
const float mainMenuBallFrameTime = (mainMenuLogoFrameTime / MAIN_MENU_LOGO_END) * MAIN_MENU_BALL_END / 1.8;
float mainMenuBallTimer = 0.0f;

Texture2D mainMenuLogo[MAIN_MENU_LOGO_END];
Texture2D mainMenuBall[MAIN_MENU_BALL_END];


void manageMainMenuScreen()
{
    if (gameState != GS_MAIN_MENU)
        return;

    ClearBackground(BLACK);
    drawMainMenuLogoAnimations();
    createMainMenuButtons();
}

// main menu buttons for changing states
void createMainMenuButtons()
{
    const int buttonTextFontSize = 22;
    Rectangle mainMenuButtonRect = {(WINDOW_WIDTH - MAIN_MENU_BUTTON_WIDTH) / 2, WINDOW_HEIGHT / 2 - MAIN_MENU_BUTTON_HEIGHT, MAIN_MENU_BUTTON_WIDTH, MAIN_MENU_BUTTON_HEIGHT};
    for (int i = 0; i < 4; i++)
    {
        char *menuButtonText;
        switch (i)
        {
        case 0:
            menuButtonText = "New Game";
            break;
        case 1:
            menuButtonText = "Map Maker";
            break;
        case 2:
            menuButtonText = "High Scores";
            break;
        case 3:
            menuButtonText = "Exit";
            break;
        default:
            break;
        }
        int textWidth = MeasureText(menuButtonText, buttonTextFontSize);
        DrawText(menuButtonText, mainMenuButtonRect.x + (mainMenuButtonRect.width - textWidth) / 2, mainMenuButtonRect.y + (mainMenuButtonRect.height - buttonTextFontSize) / 2, buttonTextFontSize, WHITE);
        mainMenuButtonRect.y += (10 + mainMenuButtonRect.height);
    }
}

// Check button clicks in main menu
void checkMainMenuButtonClick(Vector2 mousePos)
{
    int x = (WINDOW_WIDTH - MAIN_MENU_BUTTON_WIDTH) / 2;
    int y = WINDOW_HEIGHT / 2 - MAIN_MENU_BUTTON_HEIGHT;
    bool insideRectX_Axis = (mousePos.x >= x && mousePos.x <= (x + MAIN_MENU_BUTTON_WIDTH));

    if (insideRectX_Axis && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && (mousePos.y >= y && mousePos.y <= y + MAIN_MENU_BUTTON_HEIGHT))
    {
        justMouseClicked = true;
        switchGameState(GS_MAIN_GAME);
    }
    else if (insideRectX_Axis && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && (mousePos.y >= y + MAIN_MENU_BUTTON_HEIGHT * 1 + 10 * 1 && mousePos.y <= y + MAIN_MENU_BUTTON_HEIGHT * 2 + 10 * 1))
    {
        justMouseClicked = true;
        switchGameState(GS_MAP_EDITOR);
    }
    else if (insideRectX_Axis && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && (mousePos.y >= y + MAIN_MENU_BUTTON_HEIGHT * 2 + 10 * 2 && mousePos.y <= y + MAIN_MENU_BUTTON_HEIGHT * 3 + 10 * 2))
    {
        switchGameState(GS_HIGH_SCORES);
    }
    else if (insideRectX_Axis && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && (mousePos.y >= y + MAIN_MENU_BUTTON_HEIGHT * 3 + 10 * 3 && mousePos.y <= y + MAIN_MENU_BUTTON_HEIGHT * 4 + 10 * 3))
    {
        exitGame = true;
    }
}
