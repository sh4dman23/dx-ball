#include <raylib.h>
#include <raymath.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <perks.h>
#include <audio.h>
#include <paddle.h>
#include <collisions.h>
#include <stats.h>
#include <bricks.h>
#include <ball.h>
#include <coregame.h>
#include <laserpaddle.h>

const Vector2 PERK_IMG_SIZE = {32, 30};

const Vector2 PERK_SPEED = {0, 200};        // speed at which perk falls down
const double DELAY_AFTER_PERK_SPAWN = 2;    // in seconds
const double TIMED_PERK_DURATION = 10;      // base duration for perk that has timer
const int BASE_PERK_ACTIVATE_SCORE = 50;    // base score gained on perk activation

bool canSpawnPerk = true;

Perk perks[NUMBER_OF_PERKS] = {
    // Kill Paddle
    (Perk){
        "killpaddle",
        10,
        false,
    },

    // Extra Life
    (Perk){
        "extralife",
        0.5,
        false,
    },

    // Double Points
    (Perk){
        "doublepoints",
        4,
        true,
    },

    // Expand Paddle
    (Perk){
        "expandpaddle",
        4,
        // 100,
        false,
    },

    // Shrink Paddle
    (Perk){
        "shrinkpaddle",
        4,
        // 100,
        false,
    },

    // Slow Ball
    (Perk) {
        "slowball",
        4,
        false,
    },

    // Fast Ball
    (Perk) {
        "fastball",
        8,
        false,
    },

    // Laser Paddle
    (Perk) {
        "laserpaddle",
        4,
        // 100,
        false,
    },

    // Shrink Ball
    (Perk) {
        "shrinkball",
        8,
        // 100,
        false,
    },

    // Mega Ball
    (Perk) {
        "megaball",
        4,
        // 100,
        false,
    }
};


/* Function Definitions */

// Reset all perks to be deactivated and despawned
void resetPerks()
{
    for (int i = 0; i < NUMBER_OF_PERKS; i++)
    {
        perks[i].duration = 0;
        perks[i].pos = (Vector2){-1, -1};
    }
}

// Manage perk positions and durations
void updatePerks()
{
    const double dt = GetFrameTime();
    delayPerkSpawn();

    // durations
    for (int i = 0; i < NUMBER_OF_PERKS; i++)
    {
        if (perks[i].duration == 0)
            continue;

        perks[i].duration -= dt;
        if (perks[i].duration <= 0) {
            perks[i].duration = 0;
            deactivatePerk(i);
        }
    }

    // positions
    for (int i = 0; i < NUMBER_OF_PERKS; i++)
    {
        // despawn perk
        if (perks[i].pos.x <= 0 || perks[i].pos.x + PERK_IMG_SIZE.x >= GetScreenWidth() ||
            perks[i].pos.y <= 0 || perks[i].pos.y + PERK_IMG_SIZE.y >= GetScreenHeight())
        {
            perks[i].pos = (Vector2){-1, -1};
        }
        else
        {
            perks[i].pos = Vector2Add(perks[i].pos, Vector2Scale(PERK_SPEED, dt));
        }

        checkPerkAndPaddleCollision(i);
    }
}

// Spawn perk after ball hits a brick
void spawnPerk(int brickIndex)
{
    if (!canSpawnPerk)
        return;

    int *perkSequence = LoadRandomSequence(NUMBER_OF_PERKS, 0, NUMBER_OF_PERKS - 1);

    for (int *p = perkSequence, i = *p; p - perkSequence < NUMBER_OF_PERKS; p++, i = *p)
    {
        // perk already on screen => no spawn
        if (!Vector2Equals(perks[i].pos, (Vector2){-1, -1}))
            continue;

        if (!isPerkSpawnable(i))
            continue;

        // roll for rng
        double roll = ((double) rand() / RAND_MAX) * 100;
        if (roll != 0 && roll <= perks[i].spawnChance) {
            perks[i].pos = (Vector2) {
                bricks[brickIndex].rect.x + (BRICK_WIDTH - PERK_IMG_SIZE.x) / 2,
                bricks[brickIndex].rect.y};

            return;
        }
    }

    UnloadRandomSequence(perkSequence);
}

// Check if perk can be spawned
bool isPerkSpawnable(int perkIndex) {
    char *name = perks[perkIndex].filename;

    // limits on specific perks to spawn
    if (
        // has double the initial lives (max lives)
        (strcmp(name, "extralife") == 0 && lives >= STARTING_LIVES * 2) ||

        // paddle already expanded
        (strcmp(name, "expandpaddle") == 0 && currentPaddle == EXPANDED_PADDLE) ||

        // paddle already shrunk
        (strcmp(name, "shrinkpaddle") == 0 && currentPaddle == SHRUNK_PADDLE) ||

        // laser already active
        (strcmp(name, "laserpaddle") == 0 && laserActivated) ||

        // ball already mega
        (strcmp(name, "megaball") == 0 && ball.radius == MEGA_BALL_RADIUS) ||

        // ball already shrunk
        (strcmp(name, "shrinkball") == 0 && ball.radius == SHRUNK_BALL_RADIUS)
    ) {
        return false;
    }

    return true;
}

// Manage delay for perk spawn
void delayPerkSpawn()
{
    static double time = 0;
    if (canSpawnPerk)
        return;

    time += GetFrameTime();
    if (time > DELAY_AFTER_PERK_SPAWN)
    {
        time = 0;
        canSpawnPerk = true;
    }
}

// Activate effects of perk
void activatePerk(int perkIndex)
{
    canSpawnPerk = false;
    playSfx(SFX_PERK_ACTIVATION);

    // score
    increaseScore(BASE_PERK_ACTIVATE_SCORE);
    //? play sound

    char *name = perks[perkIndex].filename;
    if (strcmp(name, "killpaddle") == 0) {
        killPaddle();
    }
    else if (strcmp(name, "extralife") == 0) {
        increaseLives();
    }
    else if (strcmp(name, "doublepoints") == 0) {
        if (perks[perkIndex].duration <= 0)
            scoreMultiplier += 2;
    }
    else if (strcmp(name, "expandpaddle") == 0) {
        switchPaddle(currentPaddle == SHRUNK_PADDLE ? BASE_PADDLE : EXPANDED_PADDLE);
    }
    else if (strcmp(name, "shrinkpaddle") == 0) {
        switchPaddle(currentPaddle == EXPANDED_PADDLE ? BASE_PADDLE : SHRUNK_PADDLE);
    }
    else if (strcmp(name, "slowball") == 0) {
        double slowSpeed = min(Vector2Length(INITIAL_BALL_SPEED) / 2, Vector2Length(ball.speed));
        ball.speed = Vector2Scale(Vector2Normalize(ball.speed), slowSpeed);
    }
    else if (strcmp(name, "fastball") == 0) {
        double fastSpeed = max(Vector2Length(ACCELERATED_BALL_SPEED) * 1.2, Vector2Length(ball.speed));
        ball.speed = Vector2Scale(Vector2Normalize(ball.speed), fastSpeed);

        // limitation (based on high y velocity)
        if (fabs(ball.speed.y) > fabs(ACCELERATED_BALL_SPEED.y)) {
            ball.speed.y = (ball.speed.y > 0 ? 1 : -1) * fabs(ACCELERATED_BALL_SPEED.y);
            ball.speed.x = ball.speed.x / fabs(ball.speed.y) * fabs(ACCELERATED_BALL_SPEED.y);
        }
    }
    else if (strcmp(name, "laserpaddle") == 0) {
        activateLaser();
    }
    else if (strcmp(name, "megaball") == 0) {
        if (ball.radius == BASE_BALL_RADIUS)
            ball.radius = MEGA_BALL_RADIUS;
        else if (ball.radius == SHRUNK_BALL_RADIUS) {
            ball.radius = BASE_BALL_RADIUS;
        }
    }
    else if (strcmp(name, "shrinkball") == 0) {
        if (ball.radius == BASE_BALL_RADIUS) {
            ball.radius = SHRUNK_BALL_RADIUS;
        }
        else if (ball.radius == MEGA_BALL_RADIUS)
            ball.radius = BASE_BALL_RADIUS;
    }

    // set duration to max initial duration
    if (perks[perkIndex].timed)
        perks[perkIndex].duration = TIMED_PERK_DURATION;
}

// Remove effects of perk
void deactivatePerk(int perkIndex)
{
    perks[perkIndex].duration = 0;

    char *name = perks[perkIndex].filename;
    if (strcmp(name, "doublepoints") == 0) {
        scoreMultiplier -= 2;
    }
    else if (strcmp(name, "expandpaddle") == 0 || strcmp(name, "shrinkpaddle") == 0) {
        switchPaddle(BASE_PADDLE);
    }
    else if (strcmp(name, "laserpaddle") == 0) {
        deactivateLaser();
    }
}

// Check when falling perk hits paddle
void checkPerkAndPaddleCollision(int perkIndex)
{
    bool collision = CheckCollisionRecs(
        (Rectangle) {perks[perkIndex].pos.x, perks[perkIndex].pos.y, PERK_IMG_SIZE.x, PERK_IMG_SIZE.y},
        paddles[currentPaddle].rect
    );

    if (collision)
    {
        // despawn and activate
        perks[perkIndex].pos = (Vector2){-1, -1};
        activatePerk(perkIndex);
    }
}

