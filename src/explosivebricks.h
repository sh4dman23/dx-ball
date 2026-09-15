#ifndef EXPLOSIVEBRICKS_H
#define EXPLOSIVEBRICKS_H

#define EXPLOSIVE_BRICKS_TEXTURES_PATH  "./assets/bricks/explosive"     // textures for frames of explosive bricks
#define NUM_EXPLOSIVE_BRICK_FRAMES 5                                    // number of frames for animation

extern const double EXPLOSIVE_BRICK_FRAMETIME;
extern int explosiveBrickFrame;

extern Texture2D explosiveBrickTextures[NUM_EXPLOSIVE_BRICK_FRAMES];    // stores frame for explosive bricks

/* Function Prototypes */
void updateExplosiveBricks();

#endif
