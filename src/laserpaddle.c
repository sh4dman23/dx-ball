#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#include <laserpaddle.h>
#include <paddle.h>
#include <perks.h>
#include <bricks.h>
#include <audio.h>

Texture2D laserTexture;
Laser lasers[MAX_LASERS_PER_SCREEN] = {};

Vector2 laserSpeed = (Vector2) {0, -350};
const double INTERVAL_BETWEEN_LASERS = 0.35;

bool laserActivated = false;

/* Function Definitions */

// Reset everything related to lasers
void resetLasers() {
    laserActivated = false;
    for (int i = 0; i < MAX_LASERS_PER_SCREEN; i++) {
        lasers[i] = (Laser) {
            false, false,
            (Vector2) {-1, -1}, (Vector2) {-1, -1},
        };
    }
}

// Activate laser
void activateLaser() {
    laserActivated = true;
}

// Deactivate laser
void deactivateLaser() {
    laserActivated = false;
}

void updateLasers() {
    const double dt = GetFrameTime();
    Vector2 displacement = Vector2Scale(laserSpeed, dt);
    for (int i = 0; i < MAX_LASERS_PER_SCREEN; i++) {
        if (lasers[i].leftOnScreen)
            lasers[i].left = Vector2Add(lasers[i].left, displacement);
        if (lasers[i].rightOnScreen)
            lasers[i].right = Vector2Add(lasers[i].right, displacement);
    }

    checkLaserCollisions();
    checkLaserControls();
}

// Check input to shoot laser
void checkLaserControls() {
    static double time = 0;
    if (!laserActivated) {
        time = 0;
        return;
    }

    time += GetFrameTime();
    if (time < INTERVAL_BETWEEN_LASERS)
        return;

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        // Reset timer if laser spawned
        if (spawnLaser()) {
            time = 0;
        }
    }
}

// Spawns a laser if it can be spawned and returns true if spawned
bool spawnLaser() {
    for (int i = 0; i < MAX_LASERS_PER_SCREEN; i++) {
        if (lasers[i].leftOnScreen || lasers[i].rightOnScreen)
            continue;

        // spawn lasers above the paddle
        lasers[i] = (Laser) {
            .leftOnScreen = true,
            .rightOnScreen = true,
            .left = (Vector2) {
                paddles[currentPaddle].rect.x,
                paddles[currentPaddle].rect.y - laserTexture.height
            },
            .right = (Vector2) {
                paddles[currentPaddle].rect.x + paddles[currentPaddle].rect.width - laserTexture.width,
                paddles[currentPaddle].rect.y - laserTexture.height
            }
        };

        playSfx(SFX_LASER);
        return true;
    }

    return false;
}

// Check when laser collides with stuff
void checkLaserCollisions() {
    for (int i = 0; i < MAX_LASERS_PER_SCREEN; i++) {
        // hits the ceiling
        if (lasers[i].leftOnScreen && lasers[i].left.y < 0) {
            lasers[i].leftOnScreen = false;
            lasers[i].left = (Vector2) {-1, -1};
        }
        if (lasers[i].rightOnScreen && lasers[i].right.y < 0) {
            lasers[i].rightOnScreen = false;
            lasers[i].right = (Vector2) {-1, -1};
        }

        Rectangle lrec = (Rectangle) {
            lasers[i].left.x,
            lasers[i].left.y,
            laserTexture.width,
            laserTexture.height
        };

        Rectangle rrec = (Rectangle) {
            lasers[i].right.x,
            lasers[i].right.y,
            laserTexture.width,
            laserTexture.height
        };

        // hits brick
        for (int j = 0; j < numBricks; j++) {
            if (bricks[j].type == BRICK_EMPTY)
                continue;

            if (lasers[i].leftOnScreen && CheckCollisionRecs(bricks[j].rect, lrec)) {
                if (bricks[j].type != BRICK_UNBREAKABLE)
                    playSfx(SFX_BRICK_COLLIDE);
                degradeBrick(j);
                lasers[i].leftOnScreen = false;
                lasers[i].left = (Vector2) {-1, -1};
            }
            if (lasers[i].rightOnScreen && CheckCollisionRecs(bricks[j].rect, rrec)) {
                if (bricks[j].type != BRICK_UNBREAKABLE)
                    playSfx(SFX_BRICK_COLLIDE);
                degradeBrick(j);
                lasers[i].rightOnScreen = false;
                lasers[i].right = (Vector2) {-1, -1};
            }
        }
    }
}
