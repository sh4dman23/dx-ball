#ifndef BALL_H
#define BALL_H

typedef struct Ball
{
    Vector2 pos;
    Vector2 speed;
    double radius;
} Ball;

extern Ball ball;
extern Texture2D ballImage;

extern const Vector2 INITIAL_BALL_SPEED;       // base starting speed
extern const Vector2 ACCELERATED_BALL_SPEED;   // speed after which the ball will start decelerating
extern const Vector2 BALL_ACCELERATION;        // deceleration rate for ball

extern const double BASE_BALL_RADIUS;
extern bool ballLockedToPaddle;         // makes ball stick to paddle, until player presses space
extern double lastBallLockTime;                // in seconds
extern const double BALL_OSCILLATION_FREQ;     // oscillations per second


/* Function Prototypes */
void resetBall();                       // Reset ball to starting position
void lockBall();                        // Lock ball to paddle
void updateBall();                      // Update ball position based on collisions and stuff
void manageBallAcceleration();          // Accelerate and decelerate ball

#endif
