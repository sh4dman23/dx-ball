#include <raylib.h>
#include <raymath.h>
#include <stats.h>

//* Core game statistics
int playerScore = 0;
double scoreMultiplier = 1.0;
int playtime = 0;
int lives = 0;
const int STARTING_LIVES = 3;

const int BASE_BRICK_HIT_SCORE = 50;
