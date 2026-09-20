#include <raylib.h>
#include <raymath.h>

#include <audio.h>
#include <ball.h>
#include <bricks.h>
#include <collisions.h>
#include <coregame.h>
#include <paddle.h>
#include <perks.h>
#include <stats.h>

void checkAllCollisions()
{
    bounceBallOnPaddle();
    bounceBallOnBoundaries();
    checkBallNBrickCollisions();
}

// Check collision between ball and bricks
bool checkBallNBrickCollisions()
{
    bool collision = false;

    for (int i = 0; i < numBricks; i++)
    {

        // no collisions for empty bricks
        if (bricks[i].type == BRICK_EMPTY || !CheckCollisionCircleRec(ball.pos, ball.radius, bricks[i].rect))
            continue;

        collision = true;

        // distance between centers of rectangle and ball
        double dX = ball.pos.x - (bricks[i].rect.x + BRICK_WIDTH / 2);
        double dY = ball.pos.y - (bricks[i].rect.y + BRICK_HEIGHT / 2);

        // overlaps between ball and rectangle
        double overlapX = (BRICK_WIDTH / 2 + ball.radius) - fabs(dX);
        double overlapY = (BRICK_HEIGHT / 2 + ball.radius) - fabs(dY);

        if (overlapX <= overlapY)
        {
            if (dX > 0)
            {
                ball.speed.x *= -1;
                ball.pos.x = bricks[i].rect.x + BRICK_WIDTH + ball.radius + 1;
            }
            else
            {
                ball.speed.x *= -1;
                ball.pos.x = bricks[i].rect.x - ball.radius - 1;
            }
        }

        if (overlapY <= overlapX)
        {
            if (dY > 0)
            {
                ball.speed.y *= -1;
                ball.pos.y = bricks[i].rect.y + BRICK_HEIGHT + ball.radius + 1;
            }
            else
            {
                ball.speed.y *= -1;
                ball.pos.y = bricks[i].rect.y - ball.radius - 1;
            }
        }

        degradeBrick(i);

        // score based on ball speed
        if (isBrickBreakable(i))
            increaseScore(BASE_BRICK_HIT_SCORE * (1 + (fabs(ball.speed.x) / fabs(ACCELERATED_BALL_SPEED.x) / 2)));
    }

    return collision;
}

// [Deprecated - DO NOT USE] Check collision between ball and bricks
bool checkBallNBrickCollisions2()
{
    bool collision = false;
    for (int i = 0; i < numBricks; i++)
    {
        bool collisionNow = false;
        // no collisions for empty bricks
        if (bricks[i].type == 0 || !CheckCollisionCircleRec(ball.pos, ball.radius, bricks[i].rect))
            continue;

        double bX = bricks[i].rect.x, bY = bricks[i].rect.y;

        Vector2 leftTop = {bX, bY};
        Vector2 leftBottom = {bX, bY + BRICK_HEIGHT};
        Vector2 rightTop = {bX + BRICK_WIDTH, bY};
        Vector2 rightBottom = {bX + BRICK_WIDTH, bY + BRICK_HEIGHT};

        // ball to the left of brick
        if (ball.pos.y >= bY && ball.pos.y <= bY + BRICK_HEIGHT && distancePointLine(ball.pos, leftTop, leftBottom) <= ball.radius + 0.5)
        {
            collisionNow = true;
            ball.speed.x *= -1;
            ball.pos.x = bX - ball.radius - 1;
        }
        // ball to the right of brick
        else if (ball.pos.y >= bY && ball.pos.y <= bY + BRICK_HEIGHT && distancePointLine(ball.pos, rightTop, rightBottom) <= ball.radius + 0.5)
        {
            collisionNow = true;
            ball.speed.x *= -1;
            ball.pos.x = bX + BRICK_WIDTH + ball.radius + 1;
        }

        // ball above brick
        if (ball.pos.x >= bX && ball.pos.x <= bX + BRICK_WIDTH && distancePointLine(ball.pos, leftTop, rightTop) <= ball.radius + 0.5)
        {
            collisionNow = true;
            ball.speed.y *= -1;
            ball.pos.y = bY - ball.radius - 1;
        }
        // ball below brick
        else if (ball.pos.x >= bX && ball.pos.x <= bX + BRICK_WIDTH && distancePointLine(ball.pos, leftBottom, rightBottom) <= ball.radius + 0.5)
        {
            collisionNow = true;
            ball.speed.y *= -1;
            ball.pos.y = bY + BRICK_HEIGHT + ball.radius + 1;
        }

        //* fallback detection (in case ball gets completely inside the brick)
        if (!collisionNow)
        {
            // ball to the left of brick
            if (ball.pos.x <= bX && ball.pos.y - ball.radius < bY + BRICK_HEIGHT && ball.pos.y + ball.radius > bY)
            {
                ball.speed.x *= -1;
                ball.pos.x = bX - ball.radius - 1;
            }
            // ball to the right of brick
            else if (ball.pos.x >= bX + BRICK_WIDTH && ball.pos.y - ball.radius < bY + BRICK_HEIGHT && ball.pos.y + ball.radius > bY)
            {
                ball.speed.x *= -1;
                ball.pos.x = bX + BRICK_WIDTH + ball.radius + 1;
            }
            // ball above brick
            else if (ball.pos.y <= bY && ball.pos.x - ball.radius < bX + BRICK_WIDTH && ball.pos.x + ball.radius > bX)
            {
                ball.speed.y *= -1;
                ball.pos.y = bY - ball.radius - 1;
            }
            // ball below brick
            else if (ball.pos.y >= bY + BRICK_HEIGHT && ball.pos.x - ball.radius < bX + BRICK_WIDTH && ball.pos.x + ball.radius > bX)
            {
                ball.speed.y *= -1;
                ball.pos.y = bY + BRICK_HEIGHT + ball.radius + 1;
            }

            collisionNow = true;
        }

        //? NOTE: change this to account for ball velocity later
        if (bricks[i].type > 0)
        {
            increaseScore(BASE_BRICK_HIT_SCORE);

            // degrade brick
            bricks[i].type--;
        }

        collision = true;
    }

    return collision;
}

// Reflect ball off of the walls and ceiling, but cause death upon falling below
void bounceBallOnBoundaries()
{
    // ball bounces off walls
    if (ball.pos.x - ball.radius < 0 || ball.pos.x + ball.radius > GetScreenWidth())
    {
        ball.speed.x *= -1;
        if (ball.pos.x - ball.radius < 0)
            ball.pos.x = ball.radius;
        else
            ball.pos.x = GetScreenWidth() - ball.radius;
        playSfx(SFX_WALL_COLLIDE);
    }

    // ball bounces off ceiling
    if (ball.pos.y - ball.radius < 0)
    {
        ball.speed.y *= -1;
        ball.pos.y = ball.radius;
        playSfx(SFX_WALL_COLLIDE);
    }
    else if (ball.pos.y + ball.radius > GetScreenHeight())
    {
        killPaddle();
    }
}

// Reflect ball off of the paddle
bool bounceBallOnPaddle()
{
    bool collision = false;

    // 4 pixels of extra length on both sides to allow for fairer jump
    if (ball.pos.x >= paddles[currentPaddle].rect.x - 2 && ball.pos.x <= paddles[currentPaddle].rect.x + paddles[currentPaddle].rect.width + 2 &&
        ball.pos.y + ball.radius > paddles[currentPaddle].rect.y)
    {
        ball.speed.y *= -1;
        ball.pos.y = paddles[currentPaddle].rect.y - ball.radius - 1;
        collision = true;
    }
    else if (CheckCollisionCircleRec(ball.pos, ball.radius, (Rectangle){paddles[currentPaddle].rect.x - 2, paddles[currentPaddle].rect.y, paddles[currentPaddle].rect.width + 4, paddles[currentPaddle].rect.height}))
    {
        ball.speed.y *= -1;
        // ball.pos.y = paddles[currentPaddle].rect.y - ball.radius;
        collision = true;
    }

    // change ball angle using bounce distance from center
    if (collision) {
        playSfx(SFX_BOING);

        // signed distance between ball center and paddle center
        double delx = ball.pos.x - (paddles[currentPaddle].rect.x + paddles[currentPaddle].rect.width / 2);

        // accelerated speed based on distance from center
        ball.speed.x = (ball.speed.x > 0 ? 1 : -1) * (fabs(delx) / (paddles[currentPaddle].rect.width / 2)) * max(fabs(ball.speed.x), INITIAL_BALL_SPEED.x) * 2;

        // if dx and vx have opposite signs, flip vx
        if (delx > 0 != ball.speed.x > 0)
            ball.speed.x *= -1;

        // max speed from bouncing is accelerated speed
        if (fabs(ball.speed.x) > fabs(ACCELERATED_BALL_SPEED.x))
            ball.speed.x = (ball.speed.x > 0 ? 1 : -1) * fabs(ACCELERATED_BALL_SPEED.x);
    }

    return collision;
}
