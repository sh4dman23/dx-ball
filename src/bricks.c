#include <raylib.h>
#include <raymath.h>

#include <coregame.h>
#include <bricks.h>
#include <maps.h>

// values based on dimensions
int maxBrickRows = (WINDOW_HEIGHT - PADDING_ABOVE_MAP - PADDING_BELOW_MAP) / BRICK_HEIGHT;
int maxBrickCols = (WINDOW_WIDTH - PADDING_ON_MAP_SIDES * 2) / BRICK_WIDTH;
int numBricks;

// number of bricks remaining until level ends
int breakableBricksLeft = 0;

// bricks stored as: 0 1 2 3 ... -3 -2 -1
Texture2D brickTextures[NUM_BRICK_TEXTURES + 1];

Brick bricks[MAX_NUMBER_OF_BRICKS];

