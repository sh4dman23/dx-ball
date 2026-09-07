#ifndef MAINMENU_H
#define MAINMENU_H

#define MAIN_MENU_LOGO_START 1
#define MAIN_MENU_LOGO_END 31
#define MAIN_MENU_BALL_START 1
#define MAIN_MENU_BALL_END 14
#define MAIN_MENU_BUTTON_WIDTH 200
#define MAIN_MENU_BUTTON_HEIGHT 40
#define MAIN_MENU_TEXTURES_PATH "./assets/main_menu/"

extern bool exitGame;

extern int mainMenuLogoCurrentFrame;
extern const float mainMenuLogoFrameTime;
extern float mainMenuLogoTimer;
extern int mainMenuBallCurrentFrame;
extern const float mainMenuBallFrameTime;
extern const float mainMenuBallTimer;

// Texture for main menu
extern Texture2D mainMenuLogo[MAIN_MENU_LOGO_END];
extern Texture2D mainMenuBall[MAIN_MENU_BALL_END];


//* Function Prototypes
void manageMainMenuScreen();                                // Manage main menu
void createMainMenuButtons();                               // Create buttons for main menu
void checkMainMenuButtonClick(Vector2 mousePos);            // Check button clicks in main menu

#endif
