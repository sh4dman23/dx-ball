#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#include <bricks.h>
#include <explosivebricks.h>
#include <stats.h>

const double EXPLOSIVE_BRICK_FRAMETIME = 0.03;
int explosiveBrickFrame = 0;

// Animation frames for explosive bricks
Texture2D explosiveBrickTextures[NUM_EXPLOSIVE_BRICK_FRAMES];

// Frames for explosion animation
const double EXPLOSION_FRAMETIME = 0.025;
Texture2D explosionFrames[NUM_EXPLOSION_FRAMES];

// Explosion data for every brick
explosionData explosions[MAX_NUMBER_OF_BRICKS];

// Keep track of explosions still happening
int explosionsLeft = 0;

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

// Detonate a brick
void detonateBrick(int brickIndex) {
    //* unbreakable bricks CAN be destroyed by explosions in reference (https://dx-ball.ru)
    if (!isBrickBreakable(brickIndex) && bricks[brickIndex].type != BRICK_UNBREAKABLE)
        return;

    bool wasExplosive = (bricks[brickIndex].type == BRICK_EXPLOSIVE);
    static int i = 0;
    // printf("%d: row: %d, col: %d, type %d\n", ++i, brickIndex / maxBrickCols + 1, brickIndex % maxBrickCols + 1, bricks[brickIndex].type);

    // update count on remaining bricks left
    if (isBrickBreakable(brickIndex))
        breakableBricksLeft--;

    // set explosion animation
    setExplosion(brickIndex);

    // score
    increaseScore(BASE_BRICK_HIT_SCORE);

    // detonate bricks adjacent to it
    if (wasExplosive) {
        int r = brickIndex / maxBrickCols, c = brickIndex % maxBrickCols;
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (i == 0 && j == 0)
                    continue;

                int bi2 = (r + i) * maxBrickCols + (c + j);
                if (r + i < 0 || r + i >= maxBrickRows || c + j < 0 || c + j >= maxBrickCols)
                    continue;
                if (isBrickBreakable(bi2) || bricks[bi2].type == BRICK_UNBREAKABLE)
                    detonateBrick(bi2);
            }
        }
    }
}

// Set explosion animation to start
void setExplosion(int brickIndex) {
    explosionsLeft++;

    bricks[brickIndex].type = BRICK_EXPLODING;

    explosions[brickIndex].setToExplode = true;
    explosions[brickIndex].currentFrame = 0;
    explosions[brickIndex].timeSinceLastUpdate = 0;
}

// Check if brick is in explosion animation
bool checkExplosion(int brickIndex) {
    if (brickIndex < 0 || brickIndex >= numBricks)
        return false;
    return explosions[brickIndex].setToExplode;
}

// Update explosion animation frame for every brick
void updateExplosions() {
    const double dt = GetFrameTime();
    for (int i = 0; i < numBricks; i++) {
        if (explosions[i].setToExplode == false)
            continue;

        explosions[i].timeSinceLastUpdate += dt;
        if (explosions[i].timeSinceLastUpdate >= EXPLOSION_FRAMETIME) {
            explosions[i].currentFrame++;
            explosions[i].timeSinceLastUpdate = 0;

            // explosion animation done
            if (explosions[i].currentFrame >= NUM_EXPLOSION_FRAMES) {
                explosionsLeft--;
                bricks[i].type = BRICK_EMPTY;

                explosions[i].setToExplode = false;
                explosions[i].currentFrame = 0;
            }
        }
    }
}

// Clear explosion data
void clearExplosions() {
    for (int i = 0; i < numBricks; i++) {
        explosions[i] = (explosionData) {0, 0, 0};
    }
    explosionsLeft = 0;
}
