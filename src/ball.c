#include <raylib.h>
#include <raymath.h>

#include "ball.h"
#include "collisions.h"
#include "coregame.h"
#include "paddle.h"

Ball ball;
Texture2D ballImage;

const Vector2 INITIAL_BALL_SPEED = (Vector2) {300, -350};       // base starting speed
const Vector2 ACCELERATED_BALL_SPEED = (Vector2) {350, -400};   // speed after which the ball will start decelerating
const Vector2 BALL_ACCELERATION = (Vector2) { 15, 15 };         // deceleration rate for ball

const double BASE_BALL_RADIUS = 5.0;
const double MEGA_BALL_RADIUS = 1.75 * BASE_BALL_RADIUS;
const double SHRUNK_BALL_RADIUS = 0.75 * BASE_BALL_RADIUS;

bool ballLockedToPaddle = true;                                 // makes ball stick to paddle, until player presses space
double lastBallLockTime = 0;                                    // in seconds
const double BALL_OSCILLATION_FREQ = 1.0;                       // oscillations per second

/* Function Definitions */

// Reset ball to starting position
void resetBall()
{
    lockBall();
    ball.radius = BASE_BALL_RADIUS;
    ball.pos = (Vector2){paddles[currentPaddle].rect.x + paddles[currentPaddle].rect.width / 2, paddles[currentPaddle].rect.y - ball.radius - 1};
    ball.speed = (Vector2) {fabs(INITIAL_BALL_SPEED.x), -fabs(INITIAL_BALL_SPEED.y)};
}

// Lock ball to paddle
void lockBall()
{
    ballLockedToPaddle = true;
    lastBallLockTime = GetTime();
}

// Update ball position based on collisions and stuff
void updateBall()
{
    // ball locked to paddle position
    if (ballLockedToPaddle)
    {
        // oscillate ball
        double amp = paddles[currentPaddle].rect.width / 2.0 - ball.radius;
        double timeSinceBallLock = GetTime() - lastBallLockTime;
        double delx = amp * sin(2 * 3.14159 * BALL_OSCILLATION_FREQ * timeSinceBallLock);

        ball.pos = (Vector2){paddles[currentPaddle].rect.x + paddles[currentPaddle].rect.width / 2 + delx, paddles[currentPaddle].rect.y - ball.radius - 1};

        // ball speed depends on displacement from center at the time
        ball.speed.x = (delx / amp) * INITIAL_BALL_SPEED.x;
    }
    // ball free to move across the map
    else
    {
        const double dt = GetFrameTime();
        const Vector2 initialBallPos = ball.pos;
        Vector2 displacement = Vector2Scale(ball.speed, dt);

        // store collision with brick
        bool collision = false;

        // move ball in incremental amounts (minimum 1 times) and check for collisions (emulate spherecast)
        // this is required if the ball is moving too fast (like if displacement > brick height and similar)
        for (int i = 0, divs = 10; i < divs; i++)
        {
            ball.pos = Vector2Add(ball.pos, Vector2Scale(displacement, 1.0 / divs));
            if (checkBallNBrickCollisions() || bounceBallOnPaddle())
            {
                collision = true;
                break;
            }
        }

        // set to final position (in case of inaccuracies)
        if (!collision)
            ball.pos = Vector2Add(initialBallPos, displacement);

        bounceBallOnBoundaries();
        // bounceBallOnPaddle();

        manageBallAcceleration();
    }
}

// Accelerate and decelerate ball
void manageBallAcceleration() {
    const Vector2 initialSpeed = ball.speed;
    const double dt = GetFrameTime();

    if (ballLockedToPaddle)
        return;

    Vector2 oldSpeed = ball.speed;

    // acceleration on x
    if (fabs(ball.speed.x) < fabs(INITIAL_BALL_SPEED.x)) {
        ball.speed.x = (ball.speed.x > 0 ? 1 : -1) * (fabs(ball.speed.x) + fabs(BALL_ACCELERATION.x) * dt);
        ball.speed.y = ball.speed.x * initialSpeed.y / initialSpeed.x;
        if (fabs(ball.speed.y) > fabs(INITIAL_BALL_SPEED.y))
            ball.speed = oldSpeed;
    }

    // acceleration on y
    if (fabs(ball.speed.y) < fabs(INITIAL_BALL_SPEED.y)) {
        ball.speed.y = (ball.speed.y > 0 ? 1 : -1) * (fabs(ball.speed.y) + fabs(BALL_ACCELERATION.y) * dt);
        ball.speed.x = ball.speed.y * initialSpeed.x / initialSpeed.y;
        if (fabs(ball.speed.x) > fabs(ACCELERATED_BALL_SPEED.x))
            ball.speed = oldSpeed;
    }

    // deceleration on x
    if (fabs(ball.speed.x) > fabs(ACCELERATED_BALL_SPEED.x)) {
        ball.speed.x = (ball.speed.x > 0 ? 1 : -1) * (fabs(ball.speed.x) - fabs(BALL_ACCELERATION.x) * dt);
        ball.speed.y = ball.speed.x * initialSpeed.y / initialSpeed.x;
    }

    // deceleration on y
    if (fabs(ball.speed.y) > fabs(ACCELERATED_BALL_SPEED.y)) {
        ball.speed.y = (ball.speed.y > 0 ? 1 : -1) * (fabs(ball.speed.y) - fabs(BALL_ACCELERATION.y) * dt);
        ball.speed.x = ball.speed.y * initialSpeed.x / initialSpeed.y;
    }
}
