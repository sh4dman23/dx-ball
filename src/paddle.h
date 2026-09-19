#ifndef PADDLE_H
#define PADDLE_H

// Paddle
typedef struct Paddle
{
    Rectangle rect;                 // posx, posy, width, height
    Vector2 speed;                  // paddle speed
    Texture2D image;                // paddle texture
} Paddle;

// #define NUMBER_OF_PADDLES 3
// #define BASE_PADDLE 0
// #define EXPANDED_PADDLE 1
// #define SHRUNK_PADDLE 2

typedef enum {
    BASE_PADDLE = 0,
    EXPANDED_PADDLE,
    SHRUNK_PADDLE,
    // add here
    NUMBER_OF_PADDLES
} PaddleType;

extern Paddle paddles[NUMBER_OF_PADDLES];

extern int currentPaddle;

/* Function Prototypes */
void resetPaddle();                 // Reset paddle to starting position and base type
void updatePaddle();                // Update paddle position based on player input
void lockMouseToPaddle();           // Make it so that mouse cannot exit window while in main game

void switchPaddle(int type);        // Switch paddle at current position

void killPaddle();                  // Reduce lives when ball falls below paddle

#endif
