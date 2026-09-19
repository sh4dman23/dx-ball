#ifndef RENDER_H
#define RENDER_H

//* Function Prototypes

// Images
void loadAllImages();                   // Load all images (called before initwindow)
void unloadAllImages();                 // Unload all images

// Sprites
void loadSprites();                     // Load all textures
void unloadSprites();                   // Inverse function to loadSprites(); unloads all sprites

// Draw Functions
void drawLoop();                        // Contains all draw calls; func called inside game loop
void drawMainGame();                    // Draw main game
void drawDebugView();                   // Draw debug view
void drawMapEditor();                   // Draw map editor on screen
void drawHighScoresScreen();            // Draw leaderboards
void drawGameEnd();                     // Draw victory/defeat screen

void drawMainGameUI();                  // Draw ui for main game
void drawBall();                        // Draw ball
void drawPaddle();                      // Draw paddle
void drawBricks();                      // Draw bricks
void drawPerks();                       // Draw perks
void drawLasers();                      // Draw laser particles

void drawMainMenuLogoAnimations();      // Main menu animations


#endif
