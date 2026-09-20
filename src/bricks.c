#include <raylib.h>
#include <raymath.h>

#include "coregame.h"
#include "bricks.h"
#include "maps.h"
#include "stats.h"
#include "perks.h"
#include "ball.h"
#include "explosivebricks.h"
#include "audio.h"

// values based on dimensions
int maxBrickRows = (WINDOW_HEIGHT - PADDING_ABOVE_MAP - PADDING_BELOW_MAP) / BRICK_HEIGHT;
int maxBrickCols = (WINDOW_WIDTH - PADDING_ON_MAP_SIDES * 2) / BRICK_WIDTH;
int numBricks;

// number of bricks remaining until level ends
int breakableBricksLeft = 0;

// bricks stored as: 0 1 2 3 ... -3 -2 -1
Texture2D brickTextures[NUM_BRICK_TEXTURES + 1];

Brick bricks[MAX_NUMBER_OF_BRICKS];


/* Function Definitions */

// Check if this brick can be broken
//! PARAMETER IS BRICK INDEX, NOT BRICK TYPE
bool isBrickBreakable(int brickIndex) {
    if (brickIndex < 0 || brickIndex >= numBricks)
        return false;

    int type = bricks[brickIndex].type;
    return (
        type == BRICK_STD1 || type == BRICK_STD2 ||
        type == BRICK_STD3 || type == BRICK_EXPLOSIVE
    );
}

// Degrade brick, based on its type
void degradeBrick(int brickIndex)
{
    // play brick hit sfx
    if (bricks[brickIndex].type == BRICK_UNBREAKABLE)
        playSfx(SFX_UNBREAKABLE_BRICK_COLLIDE);
    else if (bricks[brickIndex].type == BRICK_EXPLOSIVE)
        playSfx(SFX_BRICK_EXPLOSION);
    else
        playSfx(SFX_BRICK_COLLIDE);

    // empty, unbreakable, exploding
    if (!isBrickBreakable(brickIndex))
        return;

    // explosive bricks
    if (bricks[brickIndex].type == BRICK_EXPLOSIVE) {
        detonateBrick(brickIndex);
    }

    // standard bricks
    else if (bricks[brickIndex].type > 0)
    {
        // degrade brick
        bricks[brickIndex].type--;

        // brick destroyed completely
        if (bricks[brickIndex].type == 0)
            breakableBricksLeft--;

        // for now only standard bricks give perks
        spawnPerk(brickIndex);
    }

}
