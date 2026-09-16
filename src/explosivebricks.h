#ifndef EXPLOSIVEBRICKS_H
#define EXPLOSIVEBRICKS_H

#include <bricks.h>

//* Explosive bricks
#define EXPLOSIVE_BRICKS_TEXTURES_PATH  "./assets/bricks/explosive"
#define NUM_EXPLOSIVE_BRICK_FRAMES 5

extern const double EXPLOSIVE_BRICK_FRAMETIME;
extern int explosiveBrickFrame;

extern Texture2D explosiveBrickTextures[NUM_EXPLOSIVE_BRICK_FRAMES];    // Frames for animation of explosive bricks

//* Explosions
#define EXPLOSION_TEXTURES_PATH "./assets/bricks/explosive/explosion"
#define NUM_EXPLOSION_FRAMES 9
extern const double EXPLOSION_FRAMETIME;
extern Texture2D explosionFrames[NUM_EXPLOSION_FRAMES];                  // Frames for explosion animation

typedef struct explosionData {
    bool setToExplode;
    int currentFrame;
    double timeSinceLastUpdate;
} explosionData;

extern explosionData explosions[MAX_NUMBER_OF_BRICKS];                  // Explosion data for every brick


/* Function Prototypes */
void updateExplosiveBricks();               // Update the frame of animation for explosive bricks
void detonateBrick(int brickIndex);         // Detonate a brick
void setExplosion(int brickIndex);          // Set explosion animation to start
bool checkExplosion(int brickIndex);        // Check if brick is in explosion animation
void updateExplosions();                    // Update explosion animation frame for every brick

#endif
