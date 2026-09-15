#include <raylib.h>
#include <raymath.h>

#include <bricks.h>
#include <explosivebricks.h>

const double EXPLOSIVE_BRICK_FRAMETIME = 0.02;
int explosiveBrickFrame = 0;

// stores frame for explosive bricks
Texture2D explosiveBrickTextures[NUM_EXPLOSIVE_BRICK_FRAMES];

/* Function Definitions */

// Update the frame of animation for explosive bricks
void updateExplosiveBricks() {
    static double time = 0;
    time += GetFrameTime();

    if (time >= EXPLOSIVE_BRICK_FRAMETIME) {
        explosiveBrickFrame = (explosiveBrickFrame + 1) % NUM_EXPLOSIVE_BRICK_FRAMES;
        time = 0;
    }
}
