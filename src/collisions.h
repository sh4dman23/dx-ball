#ifndef COLLISIONS_H
#define COLLISIONS_H

// Collisions
void checkAllCollisions();
bool checkBallNBrickCollisions();           // Check collision between ball and bricks

void bounceBallOnBoundaries();              // Reflect ball off of the walls and ceiling, but cause death upon falling below
bool bounceBallOnPaddle();                  // Reflect ball off of the paddle

#endif
